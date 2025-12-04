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
        NOT_IMPLEMENTED
    }

    float transmittance(const Ray &ray, float tMax,
                        Sampler &rng) const override {
        NOT_IMPLEMENTED
    }

    Bounds getBoundingBox() const override {
        NOT_IMPLEMENTED
    }

    Point getCentroid() const override {
        NOT_IMPLEMENTED
    }

    std::string toString() const override { return "Volume[]"; }
};

} // namespace lightwave

REGISTER_SHAPE(Volume, "volume")
