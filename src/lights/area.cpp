#include <lightwave.hpp>

namespace lightwave {

class AreaLight final : public Light {
    ref<Instance> m_instance;
    bool m_improvedSampling;

public:
    AreaLight(const Properties &properties) : Light(properties) {
        m_instance = properties.getChild<Instance>();
        m_instance->setLight(this);
        m_improvedSampling = properties.get<bool>("improved", true);
    }

    DirectLightSample sampleDirect(const Point &origin,
                                   Sampler &rng) const override {
        AreaSample sample = m_improvedSampling ? m_instance->sampleArea(origin, rng) : m_instance->sampleArea(rng);                  

        Vector w = sample.position - origin;
        float dist2 = w.lengthSquared();
        float dist = sqrt(dist2);
        w /= dist;

        float cosTheta = (-w).dot(sample.geometryNormal);
        if (cosTheta <= Epsilon) 
            return DirectLightSample::invalid();
        
        // p(w) = p(y) * dist2 / cosTheta
        float pdf = std::max(sample.pdf * dist2 / cosTheta, Epsilon);

        Frame frame(sample.shadingNormal);
        EmissionEval emission = m_instance->emission()->evaluate(sample.uv, frame.toLocal(-w));

        return DirectLightSample{
            .wi = w,
            .weight = emission.value / pdf, 
            .distance = dist,
            .pdf = pdf,
        };
    }

    bool canBeIntersected() const override { return m_instance->isVisible(); }

    std::string toString() const override {
        return tfm::format(
            "AreaLight[\n"
            "]");
    }
};

} // namespace lightwave

REGISTER_LIGHT(AreaLight, "area")
