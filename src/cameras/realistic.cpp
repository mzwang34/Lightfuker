#include <lightwave.hpp>
#include "lowdiscrepancy.h"
#include <numeric>

namespace lightwave {

class Realistic : public Camera {

public:
    Realistic(const Properties &properties) : Camera(properties) {
        m_lensFile = properties.get<std::string>("lensFile");
        m_simpleWeighting = properties.get<bool>("simpleWeighting", true);
        m_apertureDiameter = properties.get<float>("apertureDiameter", 1.f);
        m_focusDistance = properties.get<float>("focusDistance", 10.f);
        m_filmDiagonal = properties.get<float>("filmDiagonal", 35.0f) * 0.001f;
        m_shutterOpen = properties.get<float>("shutterOpen", 0.f);
        m_shutterClose = properties.get<float>("shutterClose", 1.f);
        
        if (m_lensFile == "") 
            lightwave_throw("No lens description file supplied!");
        
        std::vector<float> lensData;
        if (!readFloatFile(m_lensFile.c_str(), &lensData)) 
            lightwave_throw("Failed to read \"%s\"", m_lensFile);
        
        if ((lensData.size() % 4) != 0) 
            lightwave_throw("Must be multiple-of-four values!");

        for (int i = 0; i < lensData.size(); i += 4) {
            if (lensData[i] == 0) {
                if (m_apertureDiameter > lensData[i + 3]) {
                    lightwave_throw("Specified aperture diameter is greater than maximum possible.");
                }
                else {
                    lensData[i + 3] = m_apertureDiameter;
                }
            }
            elementInterfaces.push_back(LensElementInterface(
                {lensData[i] * 0.001f, lensData[i + 1] * 0.001f, lensData[i + 2], 
                 lensData[i + 3] * 0.001f * 0.5f}));
        }
        elementInterfaces.back().thickness = focusThickLens(m_focusDistance);
        
        int nSamples = 64;
        exitPupilBounds.resize(nSamples);
        std::vector<int> indices(nSamples);
        std::iota(indices.begin(), indices.end(), 0);

        for_each_parallel(indices.begin(), indices.end(), [&](int i) {
            float r0 = (float) i / nSamples * m_filmDiagonal / 2;
            float r1 = (float) (i + 1) / nSamples * m_filmDiagonal / 2;
            exitPupilBounds[i] = boundExitPupil(r0, r1);
        });
    }

    CameraSample sample(const Point2 &normalized, Sampler &rng) const override {
        float aspect = (float)m_resolution.y() / m_resolution.x();
        float filmWidth = m_filmDiagonal / std::sqrt(1.0f + aspect * aspect);
        float filmHeight = filmWidth * aspect;
        float pFilm2_x = normalized.x() * 0.5f * filmWidth;
        float pFilm2_y = -normalized.y() * 0.5f * filmHeight;
        Point pFilm(-pFilm2_x, pFilm2_y, 0.0f);
        
        float exitPupilBoundsArea;
        Point pRear = sampleExitPupil(Point2(pFilm.x(), pFilm.y()), rng.next2D(), &exitPupilBoundsArea);
        const Ray rFilm(pFilm, pRear - pFilm);
        Ray r;
        if (!traceLensesFromFilm(rFilm, &r))
            return CameraSample{Ray{Point(0.f), Vector(0, 0, 1.f)}, Color(0.f)};
        
        r = m_transform->apply(r);
        float weight;
        float cosTheta = rFilm.direction.normalized().z();
        float cos4Theta = pow(cosTheta, 4);

        Vector2 d = exitPupilBounds[0].max() - exitPupilBounds[0].min();
        float area0 = d.x() * d.y();
        if (m_simpleWeighting) weight = cos4Theta * exitPupilBoundsArea / area0;
        else weight = (m_shutterClose - m_shutterOpen) * cos4Theta * exitPupilBoundsArea / (lensRearZ() * lensRearZ());

        return CameraSample{ r.normalized(), Color(weight) };
    }

    std::string toString() const override {
        return tfm::format(
            "Realistic[\n"
            "  width = %d,\n"
            "  height = %d,\n"
            "  transform = %s,\n"
            "]",
            m_resolution.x(),
            m_resolution.y(),
            indent(m_transform));
    }

private:
    struct LensElementInterface {
        float curvatureRadius;
        float thickness;
        float eta;
        float apertureRadius;
    };

    std::string m_lensFile;
    bool m_simpleWeighting;
    float m_apertureDiameter;
    float m_focusDistance;
    float m_filmDiagonal;
    float m_shutterOpen;
    float m_shutterClose;
    std::vector<LensElementInterface> elementInterfaces;
    std::vector<Bounds2f> exitPupilBounds;

    bool readFloatFile(const char *filename, std::vector<float> *values) const {
        FILE *f = fopen(filename, "r");
        if (!f) {
            lightwave_throw("Unable to open file \"%s\"", filename);
            return false;
        }

        int c;
        bool inNumber = false;
        char curNumber[32];
        int curNumberPos = 0;
        int lineNumber = 1;
        while ((c = getc(f)) != EOF) {
            if (c == '\n') ++lineNumber;
            if (inNumber) {
                if (isdigit(c) || c == '.' || c == 'e' || c == '-' || c == '+')
                    curNumber[curNumberPos++] = c;
                else {
                    curNumber[curNumberPos++] = '\0';
                    values->push_back(atof(curNumber));
                    inNumber = false;
                    curNumberPos = 0;
                }
            } else {
                if (isdigit(c) || c == '.' || c == '-' || c == '+') {
                    inNumber = true;
                    curNumber[curNumberPos++] = c;
                } else if (c == '#') {
                    while ((c = getc(f)) != '\n' && c != EOF)
                        ;
                    ++lineNumber;
                } else if (!isspace(c)) {
                    lightwave_throw("Unexpected text found at line %d of float file \"%s\"", lineNumber, filename);
                }
            }
        }
        fclose(f);
        return true;
    }

    float lensFrontZ() const {
        float zSum = 0;
        for (const LensElementInterface &element : elementInterfaces)
            zSum += element.thickness;
        return zSum;
    }

    inline bool Quadratic(float a, float b, float c, float *t0, float *t1) const {
        // Find quadratic discriminant
        double discrim = (double)b * (double)b - 4 * (double)a * (double)c;
        if (discrim < 0) return false;
        double rootDiscrim = std::sqrt(discrim);

        // Compute quadratic _t_ values
        double q;
        if (b < 0)
            q = -.5 * (b - rootDiscrim);
        else
            q = -.5 * (b + rootDiscrim);
        *t0 = q / a;
        *t1 = c / q;
        if (*t0 > *t1) std::swap(*t0, *t1);
        return true;
    }

    Vector faceforward(const Vector &v, const Vector &v2) const {
        return (v.dot(v2) < 0.f) ? -v : v;
    }

    bool intersectSphericalElement(float radius, float zCenter, const Ray &ray, float *t, Vector *n) const {
        Point o = ray.origin - Vector(0, 0, zCenter);
        float A = sqr(ray.direction.x()) + sqr(ray.direction.y()) + sqr(ray.direction.z());
        float B = 2.f * (ray.direction.x() * o.x() + ray.direction.y() * o.y() + ray.direction.z() * o.z());
        float C = sqr(o.x()) + sqr(o.y()) + sqr(o.z()) - sqr(radius);
        float t0, t1;
        if (!Quadratic(A, B, C, &t0, &t1))
            return false;
        
        bool useCloserT = (ray.direction.z() > 0) ^ (radius < 0);
        *t = useCloserT ? std::min(t0, t1) : std::max(t0, t1);
        if (*t < 0) return false;

        *n = Vector(o + *t * ray.direction);
        *n = faceforward((*n).normalized(), -ray.direction);

        return true;
    }

    bool traceLensesFromScene(const Ray &rCamera, Ray *rOut) const {
        float elementZ = -lensFrontZ();
        Transform cameraToLens;
        cameraToLens.scale(Vector(1, 1, -1));
        Ray rLens = cameraToLens.apply(rCamera);
        for (size_t i = 0; i < elementInterfaces.size(); ++i) {
            const LensElementInterface &element = elementInterfaces[i];
            // Compute intersection of ray with lens element
            float t;
            Vector n;
            bool isStop = (element.curvatureRadius == 0);
            if (isStop)
                t = (elementZ - rLens.origin.z()) / rLens.direction.z();
            else {
                float radius = element.curvatureRadius;
                float zCenter = elementZ + element.curvatureRadius;
                if (!intersectSphericalElement(radius, zCenter, rLens, &t, &n))
                    return false;
            }
            if (t < 0)
                return false;

            // Test intersection point against element aperture
            Point pHit = rLens(t);
            float r2 = sqr(pHit.x()) + sqr(pHit.y());
            if (r2 > element.apertureRadius * element.apertureRadius) 
                return false;
            rLens.origin = pHit;

            // Update ray path for from-scene element interface interaction
            if (!isStop) {
                float etaI = (i == 0 || elementInterfaces[i - 1].eta == 0) ? 1 : elementInterfaces[i - 1].eta;
                float etaT = (elementInterfaces[i].eta != 0) ? elementInterfaces[i].eta : 1;
                Vector w = refract(-rLens.direction.normalized(), n, etaI / etaT);
                if (w.isZero()) return false;
                rLens.direction = w.normalized();
            }
            elementZ += element.thickness;
        }
        // Transform _rLens_ from lens system space back to camera space
        if (rOut != nullptr) {
            Transform lensToCamera;
            lensToCamera.scale(Vector(1, 1, -1));
            *rOut = lensToCamera.apply(rLens);
        }
        return true;
    }

    bool traceLensesFromFilm(const Ray &rCamera, Ray *rOut) const {
        float elementZ = 0;
        Transform cameraToLens;
        cameraToLens.scale(Vector(1, 1, -1));
        Ray rLens = cameraToLens.apply(rCamera);
        for (int i = elementInterfaces.size() - 1; i >= 0; --i) {
            const LensElementInterface &element = elementInterfaces[i];
            elementZ -= element.thickness;
            float t;
            Vector n;
            bool isStop = (element.curvatureRadius == 0);
            if (isStop) {
                if (rLens.direction.z() >= 0)
                    return false;
                t = (elementZ - rLens.origin.z()) / rLens.direction.z();
            } else {
                float radius = element.curvatureRadius;
                float zCenter = elementZ + element.curvatureRadius;
                if (!intersectSphericalElement(radius, zCenter, rLens, &t, &n))
                    return false;
            }
            Point pHit = rLens(t);
            float r2 = sqr(pHit.x()) + sqr(pHit.y());
            if (r2 > element.apertureRadius * element.apertureRadius)
                return false;
            rLens.origin = pHit;

            if (!isStop) {
                float etaI = element.eta;
                float etaT = (i > 0 && elementInterfaces[i - 1].eta != 0) ? elementInterfaces[i - 1].eta : 1;
                Vector w = refract(-rLens.direction.normalized(), n, etaI / etaT);
                if (w.isZero()) return false;
                rLens.direction = w.normalized();
            }
        }
        if (rOut != nullptr) {
            Transform lensToCamera;
            lensToCamera.scale(Vector(1, 1, -1));
            *rOut = lensToCamera.apply(rLens);
        }
        return true;
    }

    void computeCardinalPoints(const Ray &rIn, const Ray &rOut, float *pz, float *fz) const {
        float tf = -rOut.origin.x() / rOut.direction.x();
        *fz = -rOut(tf).z();
        float tp = (rIn.origin.x() - rOut.origin.x()) / rOut.direction.x();
        *pz = -rOut(tp).z();
    }

    void computeThickLensApproximation(float pz[2], float fz[2]) const {
        float x = 0.001f * m_filmDiagonal;
        
        // compute cardinal points for film side
        Ray rScene(Point(x, 0, lensFrontZ() + 1), Vector(0, 0, -1));
        Ray rFilm;
        traceLensesFromScene(rScene, &rFilm);
        computeCardinalPoints(rScene, rFilm, &pz[0], &fz[0]);
        
        // compute cardianl points for scene side
        rFilm = Ray(Point(x, 0, lensRearZ() - 1), Vector(0, 0, 1));
        traceLensesFromFilm(rFilm, &rScene);
        computeCardinalPoints(rFilm, rScene, &pz[1], &fz[1]);
    }

    float focusThickLens(float focusDistance) const {
        float pz[2], fz[2];
        computeThickLensApproximation(pz, fz);
        float f = fz[0] - pz[0];
        float z = -m_focusDistance;
        float delta = 0.5f * (pz[1] - z + pz[0] - std::sqrt((pz[1] - z - pz[0]) * (pz[1] - z - 4 * f - pz[0])));
        return elementInterfaces.back().thickness + delta;
    }

    float rearElementRadius() const {
        return elementInterfaces.back().apertureRadius;
    }

    float lerp(float t, float v1, float v2) const {
        return (1 - t) * v1 + t * v2;
    }

    float lensRearZ() const {
        return elementInterfaces.back().thickness;
    }

    bool isInside(const Point2 &p, const Bounds2f &b) const {
        return (p.x() >= b.min().x() && p.x() <= b.max().x() &&
                p.y() >= b.min().y() && p.y() <= b.max().y());
    }

    Bounds2f boundExitPupil(float pFilmX0, float pFilmX1) const {
        Bounds2f pupilBounds;
        // Sample a collection of points on the rear lens to find exit pupil
        const int nSamples = 1024 * 1024;
        int nExitingRays = 0;

        // Compute bounding box of projection of rear element on sampling plane
        float rearRadius = rearElementRadius();
        Bounds2f projRearBounds(Point2(-1.5f * rearRadius, -1.5f * rearRadius),
                                Point2(1.5f * rearRadius, 1.5f * rearRadius));
        for (int i = 0; i < nSamples; ++i) {
            // Find location of sample points on x segment and rear lens element
            Point pFilm(lerp((i + 0.5f) / nSamples, pFilmX0, pFilmX1), 0, 0);
            float u[2] = {radicalInverse(0, i), radicalInverse(1, i)};
            Point pRear(lerp(u[0], projRearBounds.min().x(), projRearBounds.max().x()),
                        lerp(u[1], projRearBounds.min().y(), projRearBounds.max().y()),
                        lensRearZ());

            // Expand pupil bounds if ray makes it through the lens system
            if (isInside(Point2(pRear.x(), pRear.y()), pupilBounds) ||
                traceLensesFromFilm(Ray(pFilm, pRear - pFilm), nullptr)) {
                pupilBounds.extend(Point2(pRear.x(), pRear.y()));
                ++nExitingRays;
            }
        }

        // Return entire element bounds if no rays made it through the lens system
        if (nExitingRays == 0) {
            return projRearBounds;
        }

        // Expand bounds to account for sample spacing
        pupilBounds.expand(2 * projRearBounds.diagonal().length() / std::sqrt(nSamples));
        return pupilBounds;
    }

    Point sampleExitPupil(const Point2 &pFilm, const Point2 &lensSample, float *sampleBoundsArea) const {
        float rFilm = std::sqrt(pFilm.x() * pFilm.x() + pFilm.y() * pFilm.y());
        int rIndex = rFilm / (m_filmDiagonal / 2) * exitPupilBounds.size();
        rIndex = std::min((int)exitPupilBounds.size() - 1, rIndex);
        Bounds2f pupilBounds = exitPupilBounds[rIndex];
        if (sampleBoundsArea) {
            Vector2 d = pupilBounds.max() - pupilBounds.min();
            *sampleBoundsArea = d.x() * d.y();
        }

        Point2 pLens(
            pupilBounds.min().x() + lensSample.x() * (pupilBounds.max().x() - pupilBounds.min().x()),
            pupilBounds.min().y() + lensSample.y() * (pupilBounds.max().y() - pupilBounds.min().y())
        );

        float sinTheta = (rFilm != 0) ? pFilm.y() / rFilm : 0;
        float cosTheta = (rFilm != 0) ? pFilm.x() / rFilm : 1;
        return Point(cosTheta * pLens.x() - sinTheta * pLens.y(),
                        sinTheta * pLens.x() + cosTheta * pLens.y(),
                        lensRearZ());
    }
};

} // namespace lightwave

REGISTER_CAMERA(Realistic, "realistic")