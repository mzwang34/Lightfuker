#include <lightwave.hpp>

#include <vector>

namespace lightwave {

class EnvironmentMap final : public BackgroundLight {
    /// @brief The texture to use as background
    ref<Texture> m_texture;
    /// @brief An optional transform from local-to-world space
    ref<Transform> m_transform;

public:
    EnvironmentMap(const Properties &properties) : BackgroundLight(properties) {
        m_texture   = properties.getChild<Texture>();
        m_transform = properties.getOptionalChild<Transform>();
    }

    EmissionEval evaluate(const Vector &direction) const override {
        // Point2 warped = Point2(0);
        //  hints:
        //  * if (m_transform) { transform direction vector from world to local
        //  coordinates }
        //  * find the corresponding pixel coordinate for the given local
        //  direction
        Vector local_direction =
            m_transform ? m_transform->inverse(direction) : direction;
        float phi   = atan2(-local_direction.z(), local_direction.x());
        float theta = acos(local_direction.y());
        float u     = phi * Inv2Pi + 0.5;
        float v     = theta * InvPi;
        return {
            .value = m_texture->evaluate(Point2(u, v)),
        };
    }

    DirectLightSample sampleDirect(const Point &origin,
                                   Sampler &rng) const override {
        Point2 warped    = rng.next2D();
        Vector direction = squareToUniformSphere(warped);
        auto E           = evaluate(direction);

        // implement better importance sampling here, if you ever need it
        // (useful for environment maps with bright tiny light sources, like the
        // sun for example)

        return {
            .wi       = direction,
            .weight   = E.value * FourPi,
            .distance = Infinity,
        };
    }

    std::string toString() const override {
        return tfm::format(
            "EnvironmentMap[\n"
            "  texture = %s,\n"
            "  transform = %s\n"
            "]",
            indent(m_texture),
            indent(m_transform));
    }
};

} // namespace lightwave

REGISTER_LIGHT(EnvironmentMap, "envmap")
