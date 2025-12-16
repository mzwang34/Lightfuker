#include <lightwave.hpp>

namespace lightwave {

class AreaLight final : public Light {
    ref<Shape> m_shape;
    ref<Emission> m_emission;

public:
    AreaLight(const Properties &properties) : Light(properties) {
        m_shape = properties.getChild<Shape>();
        m_emission = properties.getChild<Emission>();
    }

    DirectLightSample sampleDirect(const Point &origin,
                                   Sampler &rng) const override {
        AreaSample sample = m_shape->sampleArea(rng);

        Vector w = sample.position - origin;
        float dist2 = w.lengthSquared();
        float dist = sqrt(dist2);
        w /= dist;

        float cosTheta = (-w).dot(sample.shadingNormal);
        if (cosTheta <= Epsilon) 
            return DirectLightSample::invalid();
        
        // p(w) = p(y) * dist2 / cosTheta
        float pdf = std::max(sample.pdf * dist2 / cosTheta, Epsilon);

        Frame frame(sample.shadingNormal);
        EmissionEval emission = m_emission->evaluate(sample.uv, frame.toLocal(-w));

        return DirectLightSample{
            .wi = w,
            .weight = emission.value / pdf, 
            .distance = dist,
        };
    }

    bool canBeIntersected() const override { return true; }

    std::string toString() const override {
        return tfm::format(
            "AreaLight[\n"
            "]");
    }
};

} // namespace lightwave

REGISTER_LIGHT(AreaLight, "area")
