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
        float R = fresnelDielectric(cosTheta_o, ior);
        if (rng.next() < R) {
            return BsdfSample{ Vector(-wo.x(), -wo.y(), wo.z()),
                               m_reflectance.get()->evaluate(uv) };
        } else {
            Vector wi = refract(wo, Vector(0.f, 0.f, 1.f), ior);
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
