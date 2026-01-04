#include "fresnel.hpp"
#include "microfacet.hpp"
#include "iridescence.hpp"
#include <lightwave.hpp>

namespace lightwave {

class Iridescence : public Bsdf {
    float m_dinc;
    float m_eta2;
    float m_eta3;
    float m_kappa3;
    ref<Texture> m_roughness;

    Color evaluateIridescence(float cosTheta1) const {
        float eta2 = std::max(m_eta2, 1.000277f);
        float eta3 = std::max(m_eta3, 1.000277f);
        float kappa3 = std::max(m_kappa3, 1e-3f);

        float eta_2 = lerp(1.f, eta2, smoothstep(0.f, 0.03f, m_dinc));
        
        float cosTheta2 = sqrt(1.f - sqr(1.f / eta_2) * (1.f - sqr(cosTheta1)));

        // first interface
        Vector2 R12, phi12;
        fresnelDielectric(cosTheta1, 1.f, eta_2, R12, phi12);
        Vector2 R21 = R12;
        Vector2 T121 = Vector2(1.f) - R12;
        Vector2 phi21 = Vector2(Pi) - phi12;

        // second interface
        Vector2 R23, phi23;
        fresnelConductor(cosTheta2, eta_2, eta3, kappa3, R23, phi23);

        // phase shift
        float OPD = m_dinc * cosTheta2;
        Vector2 phi2 = phi21 + phi23;

        // compound terms
        Vector I(0.f);
        Vector2 R123 = R12 * R23;
        Vector2 r123(std::sqrt(R123.x()), std::sqrt(R123.y()));
        Vector2 Rs = sqr(T121) * R23 / (Vector2(1.f) - R123);

        // reflectance term for m=0
        Vector2 C0 = R12 + Rs;
        Vector S0 = evalSensitivity(0.f, 0.f);
        I += 0.5f * (C0.x() + C0.y()) * S0;

        // reflectance term for m>0
        Vector2 Cm = Rs - T121;
        for (int m = 1; m <= 3; ++m) {
            Cm *= r123;
            Vector SmS = 2.f * evalSensitivity(m * OPD, m * phi2.x());
            Vector SmP = 2.f * evalSensitivity(m * OPD, m * phi2.y());
            I += 0.5F * (Cm.x() * SmS + Cm.y() * SmP);
        }
        return saturate(xyzToRgb(I));
    }

public:
    Iridescence(const Properties &properties) {
        m_dinc = properties.get<float>("thickness", 0.57f);
        m_eta2 = properties.get<float>("ior_film", 1.8f);
        m_eta3 = properties.get<float>("ior_base", 1.08f);
        m_kappa3 = properties.get<float>("kappa3", 0.51f);
        m_roughness = properties.get<Texture>("roughness");
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {
        const auto alpha = max(float(1e-3), sqr(m_roughness->scalar(uv)));
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfEval::invalid();
        Vector wm = (wi + wo).normalized();             
        float D = microfacet::evaluateGGX(alpha, wm);
        float G = microfacet::smithG1(alpha, wm, wi) * microfacet::smithG1(alpha, wm, wo);
        Color I = evaluateIridescence(wm.dot(wi));
        Color value = D * G * I / (4.f * Frame::cosTheta(wo));
        float pdf = microfacet::pdfGGXVNDF(alpha, wm, wo) / (4.f * abs(wo.dot(wm)));

        return BsdfEval { value, pdf };
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        const auto alpha = max(float(1e-3), sqr(m_roughness->scalar(uv)));
        Vector wm = microfacet::sampleGGXVNDF(alpha, wo, rng.next2D());
        Vector wi = reflect(wo, wm);

        BsdfEval e = evaluate(uv, wo, wi);
        if (e.pdf < Epsilon) return BsdfSample::invalid();

        return BsdfSample { wi, e.value / e.pdf, e.pdf };
    }

    Color getAlbedo(const Intersection &its) const override {
        float cosTheta = Frame::cosTheta(its.wo);
        cosTheta = std::max(0.f, cosTheta);
        return evaluateIridescence(cosTheta);
    }

    std::string toString() const override {
        return tfm::format(
            "Iridescence[\n"
            "  thickness = %s,\n"
            "  ior_film = %s,\n"
            "  ior_base = %s,\n"
            "  kappa3 = %s,\n"
            "  roughness = %s\n"
            "]",
            m_dinc,
            m_eta2,
            m_eta3,
            m_kappa3,
            indent(m_roughness));
    }
};

} // namespace lightwave

REGISTER_BSDF(Iridescence, "iridescence")