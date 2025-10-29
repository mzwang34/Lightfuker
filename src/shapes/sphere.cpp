#include <lightwave.hpp>

namespace lightwave {
class Sphere : public Shape {
    Point2 sphericalCoordinates(const Vector &v) const {
        Point2 result;
        result.x() = atan2(v.z(), v.x()) * Inv2Pi + 0.5f;
        result.y() = acos(v.y()) * InvPi;
        return result;
    }

    inline void populate(SurfaceEvent &surf, const Point &position) const {
        surf.position = position;

        Vector n = Vector(surf.position).normalized();
        surf.tangent = Frame(n).tangent;
        surf.shadingNormal = surf.geometryNormal = n;

        surf.uv = sphericalCoordinates(n);

        surf.pdf = 0.f; // TODO
    }

public:
    Sphere(const Properties &properties) {}

    bool intersect(const Ray &ray, Intersection &its, Sampler &rng) const override {
        PROFILE("Sphere")

        auto d = ray.direction;
        auto o = ray.origin;
        Point center(0.f);
        float radius = 1.f;

        float a = d.dot(d);
        float b = 2.f * (o - center).dot(d);
        float c = (o - center).dot(o - center) - radius * radius;

        float delta = b * b - 4.f * a * c;
        if (delta < 0) return false;
        float sqrtDelta = sqrt(delta);
        float t1 = (-b - sqrtDelta) * 0.5f / a;
        float t2 = (-b + sqrtDelta) * 0.5f / a;
        if (t1 > t2) std::swap(t1, t2);

        float t = FLT_MAX;
        if (t1 >= Epsilon) t = t1;
        if (t2 >= Epsilon && t2 < t) t = t2;

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
    Point getCentroid() const override {
        return Point(0.f);
    }
    AreaSample sampleArea(Sampler &rng) const override {
        NOT_IMPLEMENTED
    }
    std::string toString() const override {
        return "Sphere[]";
    }
};
}
REGISTER_SHAPE(Sphere, "sphere")