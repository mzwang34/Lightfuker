#include <lightwave.hpp>

namespace lightwave {

class Volume : public Shape {
    float m_density;
    ref<Shape> m_boundary;

public:
    Volume(const Properties &properties) {
        m_density  = properties.get<float>("density");
        m_boundary = properties.getOptionalChild<Shape>();
    }

    bool intersect(const Ray &ray, Intersection &its,
                   Sampler &rng) const override {
        float t = Epsilon - log(1.f - rng.next()) / m_density;
        if (t < its.t) {
            its.t             = t;
            its.position      = ray(t);
            its.shadingNormal = -ray.direction;
            return true;
        }
          
        return false;
    }

    float transmittance(const Ray &ray, float tMax,
                        Sampler &rng) const override {
        Intersection its;
        if (intersect(ray, its, rng) && its.t < tMax)
            return 0.f;
        return exp(-m_density * tMax);
    }

    Bounds getBoundingBox() const override {
        return Bounds::full();
    }

    Point getCentroid() const override {
        return Point{ 0.f };
    }

    std::string toString() const override { return "Volume[]"; }
};

} // namespace lightwave

REGISTER_SHAPE(Volume, "volume")
