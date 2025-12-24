#include <lightwave.hpp>

#include <vector>

namespace lightwave {

    struct Distribution1D {
    Distribution1D(const float *f, int n) : func(f, f + n), cdf(n + 1) {
        cdf[0] = 0;
        for (int i = 1; i < n + 1; ++i)
            cdf[i] = cdf[i - 1] + func[i - 1] / n;
        
        funcInt = cdf[n];
        if (funcInt == 0) {
            for (int i = 1; i < n + 1; ++i)
                cdf[i] = float(i) / float(n);
        } else {
            for (int i = 1; i < n + 1; ++i)
                cdf[i] /= funcInt;
        }
    }

    int Count() const { return func.size(); }

    template <typename Predicate> int findInterval(int size, const Predicate &pred) const {
        int first = 0, len = size;
        while (len > 0) {
            int half = len >> 1, middle = first + half;
            if (pred(middle)) {
                first = middle + 1;
                len -= half + 1;
            } else {
                len = half;
            }
        }
        return std::clamp(first - 1, 0, size - 2);
    }

    float sampleContinuous(float u, float *pdf, int *off = nullptr) const {
        int offset = findInterval(cdf.size(), [&](int index) {
            return cdf[index] <= u;
        });
        if (off) *off = offset;
        float du = u - cdf[offset];
        if ((cdf[offset + 1] - cdf[offset]) > 0)
            du /= (cdf[offset + 1] - cdf[offset]);
        
        if (pdf) *pdf = func[offset] / funcInt;
        
        return (offset + du) / Count();
    }

    int sampleDiscrete(float u, float *pdf = nullptr, float *uRemapped = nullptr) const {
        int offset = findInterval(cdf.size(), [&](int index) {
            return cdf[index] <= u;
        });

        if (pdf) *pdf = func[offset] / (funcInt * Count());
        if (uRemapped)
            *uRemapped = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
        return offset;
    }

    float discretePDF(int index) const {
        return func[index] / (funcInt * Count());
    }

    std::vector<float> func, cdf;
    float funcInt;
};

class Distribution2D {
public:
    Distribution2D(const float *func, int nu, int nv) {
        for (int v = 0; v < nv; ++v) {
            pConditionalV.emplace_back(new Distribution1D(&func[v * nu], nu));
        }
        std::vector<float> marginalFunc;
        for (int v = 0; v < nv; ++v)
            marginalFunc.push_back(pConditionalV[v]->funcInt);
        pMarginal.reset(new Distribution1D(&marginalFunc[0], nv));
    }

    Point2 sampleContinuous(const Point2 &u, float *pdf) const {
        float pdfs[2];
        int v;
        float d1 = pMarginal->sampleContinuous(u[1], &pdfs[1], &v);
        float d0 = pConditionalV[v]->sampleContinuous(u[0], &pdfs[0]);
        *pdf = pdfs[0] * pdfs[1];
        return Point2(d0, d1);
    }

    float pdf(const Point2 &p) const {
        int iu = clamp(int(p[0] * pConditionalV[0]->Count()), 0, pConditionalV[0]->Count() - 1);
        int iv = clamp(int(p[1] * pMarginal->Count()), 0, pMarginal->Count() - 1);
        return pConditionalV[iv]->func[iu] / pMarginal->funcInt;
    }

private:
    std::vector<std::unique_ptr<Distribution1D>> pConditionalV;
    std::unique_ptr<Distribution1D> pMarginal;
};

class EnvironmentMap final : public BackgroundLight {
    /// @brief The texture to use as background
    ref<Texture> m_texture;
    /// @brief An optional transform from local-to-world space
    ref<Transform> m_transform;

    std::unique_ptr<Distribution2D> m_distribution;
    bool m_importanceSampling;

public:
    EnvironmentMap(const Properties &properties) : BackgroundLight(properties) {
        m_texture   = properties.getChild<Texture>();
        m_transform = properties.getOptionalChild<Transform>();
        m_importanceSampling = properties.get<bool>("importanceSampling", true);

        if (m_importanceSampling) {
            auto imageTex = std::dynamic_pointer_cast<ImageTexture>(m_texture);
            Point2i res = imageTex->getImage()->resolution();
            int width = res.x(), height = res.y();
            std::unique_ptr<float[]> img(new float[width * height]);
            for (int v = 0; v < height; ++v) {
                float vp = (float)v / (float)height;
                float sinTheta = std::sin(Pi * float(v + .5f) / float(height));
                for (int u = 0; u < width; ++u) {
                    float up = (float)u / (float)width;
                    img[u + v * width] = m_texture->evaluate(Point2(up, vp)).luminance() * sinTheta;
                } 
            }
            m_distribution.reset(new Distribution2D(img.get(), width, height));
        }
    }

    EmissionEval evaluate(const Vector &direction) const override {
        // Point2 warped = Point2(0);
        //  hints:
        //  * if (m_transform) { transform direction vector from world to local
        //  coordinates }
        //  * find the corresponding pixel coordinate for the given local
        //  direction
        Vector local_direction =
            m_transform ? m_transform->inverse(direction) : direction;
        float phi   = atan2(-local_direction.z(), local_direction.x());
        float theta = acos(local_direction.y());
        float u     = phi * Inv2Pi + 0.5;
        float v     = theta * InvPi;
        float pdf = m_importanceSampling ? m_distribution->pdf(Point2(u, v)) / (2 * Pi * Pi * std::sin(theta)) : Inv4Pi;
        return {
            .value = m_texture->evaluate(Point2(u, v)), .pdf = pdf, 
        };
    }

    DirectLightSample sampleDirect(const Point &origin,
                                   Sampler &rng) const override {
        if (!m_importanceSampling) {
            Point2 warped    = rng.next2D();
            Vector direction = squareToUniformSphere(warped);
            auto E           = evaluate(direction);
            return {
                .wi       = direction,
                .weight   = E.value * FourPi,
                .distance = Infinity,
                .pdf = Inv4Pi,
            };
        } else {
            float mapPdf;
            Point2 uv = m_distribution->sampleContinuous(rng.next2D(), &mapPdf);
            if (mapPdf == 0) return DirectLightSample::invalid();

            float theta = uv.y() * Pi;
            float phi = (1.f - 2.f * uv.x()) * Pi;
            float sinTheta = std::sin(theta);
            Vector wi = Vector(cos(phi) * sinTheta, cos(theta), sin(phi) * sinTheta);
            if (m_transform) wi = m_transform->apply(wi).normalized();

            float pdf = mapPdf / (2 * Pi * Pi * sinTheta);
            if (sinTheta == 0) return DirectLightSample::invalid();

            auto E = m_texture->evaluate(uv);

            return {
                .wi       = wi,
                .weight   = E / pdf,
                .distance = Infinity,
                .pdf      = pdf,
            };
        }
    }

    std::string toString() const override {
        return tfm::format(
            "EnvironmentMap[\n"
            "  texture = %s,\n"
            "  transform = %s\n"
            "]",
            indent(m_texture),
            indent(m_transform));
    }
};

} // namespace lightwave

REGISTER_LIGHT(EnvironmentMap, "envmap")
