#include <lightwave.hpp>

namespace lightwave {

class BlackbodyTexture : public Texture {
    float m_temperature;
    Color m_tint;

public:
    BlackbodyTexture(const Properties &properties) {
        m_temperature = properties.get<float>("temperature", 6500.f);
        m_tint = properties.get<Color>("tint");
    }

    Color evaluate(const Point2 &uv) const override {
        return Color::fromTemperature(m_temperature) * m_tint;
    }

    std::string toString() const override {
        return tfm::format(
            "BlackbodyTexture[\n"
            "  temperature = %s\n"
            "  tint        = %s\n"
            "]",
            indent(m_temperature),
            indent(m_tint));
    }
};

} // namespace lightwave

REGISTER_TEXTURE(BlackbodyTexture, "blackbody")