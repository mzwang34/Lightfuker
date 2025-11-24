#include <lightwave.hpp>

namespace lightwave {

class CheckerboardTexture : public Texture {
    Vector2 m_scale;
    Color m_color0;
    Color m_color1;

public:
    CheckerboardTexture(const Properties &properties) {
        m_scale  = properties.get<Vector2>("scale", Vector2(1, 1));
        m_color0 = properties.get<Color>("color0", Color(0));
        m_color1 = properties.get<Color>("color1", Color(1));
    }

    Color evaluate(const Point2 &uv) const override { 
        int u = int(floor(uv.x() * m_scale.x()));
        int v = int(floor(uv.y() * m_scale.y()));
        if (u % 2 == v % 2)
            return m_color0;
        return m_color1;
    }

    std::string toString() const override {
        return tfm::format(
            "CheckerboardTexture[\n"
            "  scale = %s\n"
            "  color0 = %s\n"
            "  color1 = %s\n"
            "]",
            indent(m_scale),
            indent(m_color0),
            indent(m_color1));
    }
};

} // namespace lightwave

REGISTER_TEXTURE(CheckerboardTexture, "checkerboard")
