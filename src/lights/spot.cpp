#include <lightwave.hpp>

namespace lightwave {

class SpotLight final : public Light {
    Point m_position;
    Color m_power;
    Vector m_direction;
    float m_angle;
    float m_falloffStart;

public:
    SpotLight(const Properties &properties) : Light(properties) {
        m_position = properties.get<Point>("position");
        m_power = properties.get<Color>("power");
        m_direction = properties.get<Vector>("direction").normalized();
        m_angle = properties.get<float>("angle", 30.0f);
        m_falloffStart = properties.get<float>("falloffStart", 30.f);
    }

    DirectLightSample sampleDirect(const Point &origin,
                                   Sampler &rng) const override {
        Vector w = m_position - origin;
        float dist2 = w.lengthSquared();
        float dist = sqrt(dist2);
        w /= dist;

        float cosLight = m_direction.dot(-w);
        float cosTotal = cos(m_angle * Pi / 180.f);
        float cosFalloffStart = cos(m_falloffStart * Pi / 180.f);
        if (cosLight < 0 || cosLight < cosTotal)
            return DirectLightSample::invalid();
        
        float falloff = 1.f;
        if (cosFalloffStart > cosLight)
            falloff = (cosLight - cosTotal) / (cosFalloffStart - cosTotal);

        Color weight = m_power * Inv4Pi * pow(falloff, 4) / dist2;
        return DirectLightSample{ w, weight, dist, 1.f };
    }

    bool canBeIntersected() const override { return false; }

    std::string toString() const override {
        return tfm::format(
            "SpotLight[\n"
            "]");
    }
};

} // namespace lightwave

REGISTER_LIGHT(SpotLight, "spot")
