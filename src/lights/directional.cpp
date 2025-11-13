#include <lightwave.hpp>

namespace lightwave {

class DirectionalLight final : public Light {
    Color m_intensity;
    Vector m_dir;

public:
    DirectionalLight(const Properties &properties) : Light(properties) {
        m_intensity = properties.get<Color>("intensity", Color::black());
        m_dir       = properties.get<Vector>("direction", Vector(0.f));
    }

    DirectLightSample sampleDirect(const Point &origin,
                                   Sampler &rng) const override {
        // NOT_IMPLEMENTED
        DirectLightSample sample = DirectLightSample();
        sample.wi                = (m_dir).normalized();
        sample.distance          = Infinity;
        sample.weight            = m_intensity;
        return sample;
    }

    bool canBeIntersected() const override { return false; }

    std::string toString() const override {
        return tfm::format(
            "DirectionalLight[\n"
            "]");
    }
};

} // namespace lightwave

REGISTER_LIGHT(DirectionalLight, "directional")
