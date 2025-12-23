#include "fresnel.hpp"
#include "microfacet.hpp"
#include <lightwave.hpp>

namespace lightwave {

class RoughDielectric : public Bsdf {
    ref<Texture> m_ior;
    ref<Texture> m_reflectance;
    ref<Texture> m_transmittance;
    ref<Texture> m_roughness;

public:
    RoughDielectric(const Properties &properties) {
        m_ior           = properties.get<Texture>("ior");
        m_reflectance   = properties.get<Texture>("reflectance");
        m_transmittance = properties.get<Texture>("transmittance");
        m_roughness     = properties.get<Texture>("roughness");
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {      
        const auto alpha = max(float(1e-3), sqr(m_roughness->scalar(uv)));
                        
        float cosTheta_o = Frame::cosTheta(wo);
        float cosTheta_i = Frame::cosTheta(wi);
        bool reflect = cosTheta_i * cosTheta_o > 0;
        float eta = m_ior->scalar(uv);
        float etap = 1;
        if (!reflect) {
            etap = cosTheta_o > 0? eta : (1 / eta);
        }
        Vector wm = etap * wi + wo;
        if (cosTheta_i == 0 || cosTheta_o == 0 || wm.lengthSquared() == 0) 
            return BsdfEval::invalid();
        wm = wm.normalized();
        if (wm.z() < 0.f) wm = -wm;
        
        if (wm.dot(wi) * cosTheta_i < 0 || wm.dot(wo) * cosTheta_o < 0)
            return BsdfEval::invalid();
        
        float pdf;
        Color value;
        float R = fresnelDielectric(wo.dot(wm), eta);
        auto c_reflect = R * m_reflectance->evaluate(uv);
        auto c_transmit = (1 - R) * m_transmittance->evaluate(uv);
        float D = microfacet::evaluateGGX(alpha, wm);
        float G1_i = microfacet::smithG1(alpha, wm, wi);
        float G1_o = microfacet::smithG1(alpha, wm, wo);
        
        if (reflect) {
            value = c_reflect * D * G1_i * G1_o / (4 * abs(cosTheta_o));
            pdf = R * microfacet::pdfGGXVNDF(alpha, wm, wo) * microfacet::detReflection(wm, wo);
        } else {
            float denom = sqr(wi.dot(wm) + wo.dot(wm) / etap) * cosTheta_o;
            value = c_transmit * D * G1_i * G1_o * std::abs(wi.dot(wm) * wo.dot(wm) / denom);
            pdf = (1 - R) * microfacet::pdfGGXVNDF(alpha, wm, wo) * abs(wi.dot(wm)) / microfacet::detRefraction(wm, wi, wo, etap);
        }
        return BsdfEval{ value, pdf };
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        const auto alpha = max(float(1e-3), sqr(m_roughness->scalar(uv)));
        float eta = m_ior->scalar(uv);
        float cosTheta_o = Frame::cosTheta(wo);
        float etap = cosTheta_o > 0 ? eta : (1 / eta);

        Vector wm = microfacet::sampleGGXVNDF(alpha, wo, rng.next2D());
        float R = fresnelDielectric(wo.dot(wm), etap);
        
        float pdf;
        Color weight;
        Vector wi;
        if (rng.next() < R) {
            wi = reflect(wo, wm);
            if (!Frame::sameHemisphere(wi, wo)) 
                return BsdfSample::invalid();
            weight = m_reflectance->evaluate(uv) * microfacet::smithG1(alpha, wm, wi);
            pdf = R * microfacet::pdfGGXVNDF(alpha, wm, wo) * microfacet::detReflection(wm, wo);
        } else {
            wi = refract(wo, wm, etap);
            if (Frame::sameHemisphere(wi, wo) || wi.z() == 0 || wi.isZero())
                return BsdfSample::invalid();
            float denom = sqr(wi.dot(wm) + wo.dot(wm) / etap) * cosTheta_o;
            weight = m_transmittance->evaluate(uv) * microfacet::smithG1(alpha, wm, wi);
            pdf = (1 - R) * microfacet::pdfGGXVNDF(alpha, wm, wo) * abs(wi.dot(wm)) / microfacet::detRefraction(wm, wi, wo, etap);
        }
        return BsdfSample { wi, weight, pdf };
    }

    std::string toString() const override {
        return tfm::format(
            "RoughDielectric[\n"
            "  ior           = %s,\n"
            "  reflectance   = %s,\n"
            "  transmittance = %s\n"
            "  roughness = %s\n"
            "]",
            indent(m_ior),
            indent(m_reflectance),
            indent(m_transmittance),
            indent(m_roughness));
    }
};

} // namespace lightwave

REGISTER_BSDF(RoughDielectric, "roughdielectric")
