#include <lightwave.hpp>

namespace lightwave {

class AovIntegrator : public SamplingIntegrator {
    std::string m_variable;
    float m_scale;

public:
    AovIntegrator(const Properties &properties)
        : SamplingIntegrator(properties) {
        m_variable = properties.get<std::string>("variable", "normals");
        m_scale = properties.get<float>("scale", 1.0f);
    }

    Color Li(const Ray &ray, Sampler &rng) override {
        if (m_variable == "normals") {
            Intersection its = m_scene->intersect(ray, rng);
            Vector n;
            if (its.background)
                n = Vector(0.f);
            else
                n = its.shadingNormal;
            n = (n + Vector(1.f)) * 0.5f;
            return Color(n);
        } /*else if (m_variable == "bvh") {
            Intersection its = m_scene->intersect(ray, rng);
            if (its.background) return Color(0.f);
            return Color(its.stats.bvhCounter, its.stats.primCounter, 0.f) / m_scale;
        }*/
        return Color(0.f);
    }

    /// @brief An optional textual representation of this class, which can be
    /// useful for debugging.
    std::string toString() const override {
        return tfm::format("AovIntegrator[\n"
                           "  sampler = %s,\n"
                           "  image = %s,\n"
                           "]",
                           indent(m_sampler), indent(m_image));
    }
};

} // namespace lightwave

REGISTER_INTEGRATOR(AovIntegrator, "aov")
