#include <lightwave.hpp>

namespace lightwave {

class HenyeyGreenstein : public Bsdf {
    float m_g;
    Color m_albedo;

public:
    HenyeyGreenstein(const Properties &properties) {
        m_g      = properties.get<float>("g");
        m_albedo = properties.get<Color>("albedo");
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {
        float g2 = m_g * m_g;
        return BsdfEval{ m_albedo * Inv4Pi * (1.f - g2) / pow(1.f + g2 + 2.f * m_g * wi.dot(wo), 1.5f) };
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        float u = rng.next(), v = rng.next();
        float phi = 2.f * Pi * u;
        float cosTheta;
        if (abs(m_g) < 1e-3f) {
            cosTheta = 1.f - 2.f * v;
        } else {
            float t = (1.f - m_g * m_g) / (1.f + m_g - 2.f * m_g * v);
            cosTheta = -1.f / (2.f * m_g) * (1.f + m_g * m_g - t * t);
        }
        float sinTheta = sqrt(std::max(0.f, 1.f - cosTheta * cosTheta));
        Vector w{ sinTheta * cos(phi), sinTheta * sin(phi), cosTheta };

        return BsdfSample{ w, m_albedo };
    }

    std::string toString() const override {
        return tfm::format(
            "HenyeyGreenstein[\n"
            "  albedo = %s\n"
            "]",
            indent(m_albedo));
    }
};

} // namespace lightwave

REGISTER_BSDF(HenyeyGreenstein, "hg")
