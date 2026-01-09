#include <lightwave.hpp>

namespace lightwave {
class Sphere : public Shape {
    Point2 sphericalCoordinates(const Vector &v) const {
        float x = atan2(-v.z(), v.x());
        if (x < 0)
            x += 2.f * Pi;
        x *= Inv2Pi;
        float y = acos(v.y()) * InvPi;
        return Point2(x, y);
    }

    inline void populate(SurfaceEvent &surf, const Point &position) const {
        surf.position = position;

        Vector n           = Vector(surf.position).normalized();
        surf.tangent = Vector(-n.z(), 0, n.x()).normalized();
        surf.shadingNormal = surf.geometryNormal = n;

        surf.uv = sphericalCoordinates(n);

        surf.pdf = Inv4Pi;
    }

public:
    Sphere(const Properties &properties) {}

    bool intersect(const Ray &ray, Intersection &its,
                   Sampler &rng) const override {
        PROFILE("Sphere")

        auto d = ray.direction;
        auto o = ray.origin;
        Point center(0.f);
        float radius = 1.f;

        float a = d.dot(d);
        float b = 2.f * (o - center).dot(d);
        float c = (o - center).dot(o - center) - radius * radius;

        float delta = b * b - 4.f * a * c;
        if (delta < 0)
            return false;
        float sqrtDelta = sqrt(delta);
        float t1        = (-b - sqrtDelta) * 0.5f / a;
        float t2        = (-b + sqrtDelta) * 0.5f / a;
        if (t1 > t2)
            std::swap(t1, t2);

        float t   = Infinity;
        bool flag = false;
        if (t1 >= Epsilon) {
            t    = t1;
            flag = true;
        }
        if (t2 >= Epsilon && t2 < t) {
            t    = t2;
            flag = true;
        }
        if (!flag)
            return false;

        if (t <= its.t) {
            its.t = t;
            populate(its, ray(t));
            return true;
        }

        return false;
    }
    Bounds getBoundingBox() const override {
        return Bounds(Point{ -1, -1, -1 }, Point{ +1, +1, +1 });
    }

    Point getCentroid() const override { return Point(0.f); }

    AreaSample sampleArea(Sampler &rng) const override{
        // area sampling
        float u = rng.next();
        float v = rng.next();

        float z = 1.0f - 2.0f * u; // cos_theta
        float sin_theta = sqrt(std::max(0.f, 1.0f - z * z));
        float phi = 2.f * Pi * v;

        float x = sin_theta * cos(phi);
        float y = sin_theta * sin(phi);

        Point position(x, y, z);

        AreaSample sample;
        populate(sample, position);

        return sample;
    } 

    AreaSample sampleArea(const Point &origin, Sampler &rng) const override {
        float u = rng.next();
        float v = rng.next();

        Vector d = getCentroid() - origin;
        float dist2 = d.lengthSquared();
        float dist = std::sqrt(dist2);
        d /= dist;

        float sin_theta_max2 = 1.f / dist2;
        float cos_theta_max = std::sqrt(std::max(0.f, 1.f - sin_theta_max2));
        float cos_theta = 1.f - u * (1.f - cos_theta_max);

        // ---------- intersect method ----------
        float sin_theta = std::sqrt(std::max(0.f, 1.f - cos_theta * cos_theta));
        float phi = 2.f * Pi * v;

        Frame frame(d);

        float x = sin_theta * cos(phi);
        float y = sin_theta * sin(phi);
        Vector sample_dir(x, y, cos_theta);
        sample_dir = frame.toWorld(sample_dir);

        Intersection its;
        Ray ray(origin, sample_dir);
        // fix floating precision error at edge
        if (!intersect(ray, its, rng))
            its.t = d.dot(sample_dir);

        Point position = ray(its.t);

        // ---------- Fred Akalin's method ----------
        /*float sin_theta_max = std::sqrt(sin_theta_max2);
        float sin_theta_2 = 1.f - cos_theta * cos_theta;

        float cos_alpha = sin_theta_2 / sin_theta_max + cos_theta * std::sqrt(1 - sin_theta_2 / (sin_theta_max * sin_theta_max));
        float sin_alpha = std::sqrt(1 - cos_alpha * cos_alpha);
	    float phi = 2.f * Pi * v;

        Frame frame(-d);

        float x = sin_alpha * cos(phi);
        float y = sin_alpha * sin(phi);
        Vector sample_dir(x, y, cos_alpha);
        sample_dir = frame.toWorld(sample_dir);

        Point position = getCentroid() + sample_dir;*/

        AreaSample sample;
        populate(sample, position);
        Vector w = sample.position - origin;
        dist2 = w.lengthSquared();
        float cosTheta = (-w / sqrt(dist2)).dot(sample.shadingNormal);
        sample.pdf = Inv2Pi / (1.f - cos_theta_max) * cosTheta / dist2; // area pdf

        return sample;
    }
    
    std::string toString() const override {
        return "Sphere[]";
    }
};
} // namespace lightwave
REGISTER_SHAPE(Sphere, "sphere")