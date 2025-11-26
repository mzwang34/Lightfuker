#include "fresnel.hpp"
#include <lightwave.hpp>

namespace lightwave {

class Dielectric : public Bsdf {
    ref<Texture> m_ior;
    ref<Texture> m_reflectance;
    ref<Texture> m_transmittance;

public:
    Dielectric(const Properties &properties) {
        m_ior           = properties.get<Texture>("ior");
        m_reflectance   = properties.get<Texture>("reflectance");
        m_transmittance = properties.get<Texture>("transmittance");
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {
        // the probability of a light sample picking exactly the direction `wi'
        // that results from reflecting or refracting `wo' is zero, hence we can
        // just ignore that case and always return black
        return BsdfEval::invalid();
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        // NOT_IMPLEMENTED
        float cosTheta_o = Frame::cosTheta(wo);
        float ior        = m_ior->scalar(uv);
        if (cosTheta_o < 0) {
            ior        = 1.f / ior;
            cosTheta_o = -cosTheta_o;
        }
        float sin2Theta_o = 1.f - cosTheta_o * cosTheta_o;
        float sin2Theta_i = sin2Theta_o / (ior * ior);
        if (sin2Theta_i > 1.f)
            return BsdfSample{ Vector(-wo.x(), -wo.y(), wo.z()),
                               m_reflectance.get()->evaluate(uv) };
        float cosTheta_i = sqrt(1 - sin2Theta_i);
        float F_p =
            (ior * cosTheta_o - cosTheta_i) / (ior * cosTheta_o + cosTheta_i);
        float F_s =
            (cosTheta_o - ior * cosTheta_i) / (cosTheta_o + ior * cosTheta_i);
        float R = (F_p * F_p + F_s * F_s) / 2.f;
        if (rng.next() < R) {
            return BsdfSample{ Vector(-wo.x(), -wo.y(), wo.z()),
                               m_reflectance.get()->evaluate(uv) };
        } else {
            Vector n  = Frame::cosTheta(wo) > 0 ? Vector(0.f, 0.f, 1.f)
                                                : Vector(0.f, 0.f, -1.f);
            Vector wi = -wo / ior + (cosTheta_o / ior - cosTheta_i) * n;
            return BsdfSample{
                wi, m_transmittance.get()->evaluate(uv) / (ior * ior)
            };
        }
    }

    std::string toString() const override {
        return tfm::format(
            "Dielectric[\n"
            "  ior           = %s,\n"
            "  reflectance   = %s,\n"
            "  transmittance = %s\n"
            "]",
            indent(m_ior),
            indent(m_reflectance),
            indent(m_transmittance));
    }
};

} // namespace lightwave

REGISTER_BSDF(Dielectric, "dielectric")
