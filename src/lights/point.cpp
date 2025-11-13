#include <lightwave.hpp>

namespace lightwave {

class PointLight final : public Light {
    Color m_power;
    Point m_pos;

public:
    PointLight(const Properties &properties) : Light(properties) {
        m_power = properties.get<Color>("power", Color::black());
        m_pos   = properties.get<Point>("position", Point(0.f));
    }

    DirectLightSample sampleDirect(const Point &origin,
                                   Sampler &rng) const override {
        // NOT_IMPLEMENTED
        DirectLightSample sample = DirectLightSample();
        sample.wi                = (m_pos - origin).normalized();
        sample.distance          = (m_pos - origin).length();
        sample.weight = m_power * Inv4Pi / (m_pos - origin).lengthSquared();
        return sample;
    }

    bool canBeIntersected() const override { return false; }

    std::string toString() const override {
        return tfm::format(
            "PointLight[\n"
            "]");
    }
};

} // namespace lightwave

REGISTER_LIGHT(PointLight, "point")
