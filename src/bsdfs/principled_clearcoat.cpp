#include <lightwave.hpp>

#include "fresnel.hpp"
#include "microfacet.hpp"

namespace lightwave {

struct DiffuseLobe {
    Color color;

    BsdfEval evaluate(const Vector &wo, const Vector &wi) const {
        // NOT_IMPLEMENTED
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfEval::invalid();
        return BsdfEval{ color * InvPi * Frame::cosTheta(wi), abs(wi.z()) * InvPi };

        // hints:
        // * copy your diffuse bsdf evaluate here
        // * you do not need to query a texture, the albedo is given by `color`
    }

    BsdfSample sample(const Vector &wo, Sampler &rng) const {
        // NOT_IMPLEMENTED
        Vector wi = squareToCosineHemisphere(rng.next2D());
        if (Frame::cosTheta(wo) <= 0)
            wi = -wi;
        Color weight = color;
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfSample::invalid();

        return BsdfSample{ wi, weight, abs(wi.z()) * InvPi };
        // hints:
        // * copy your diffuse bsdf evaluate here
        // * you do not need to query a texture, the albedo is given by `color`
    }
};

struct MetallicLobe {
    float alpha;
    Color color;

    BsdfEval evaluate(const Vector &wo, const Vector &wi) const {
        // NOT_IMPLEMENTED
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfEval::invalid();
        Vector wm = (wi + wo).normalized();
        float pdf = microfacet::pdfGGXVNDF(alpha, wm, wo) / (4 * abs(wo.z()));
        return { color * microfacet::evaluateGGX(alpha, wm) *
                 microfacet::smithG1(alpha, wm, wi) *
                 microfacet::smithG1(alpha, wm, wo) /
                 (4 * abs(Frame::cosTheta(wo))), pdf };
        // hints:
        // * copy your roughconductor bsdf evaluate here
        // * you do not need to query textures
        //   * the reflectance is given by `color'
        //   * the variable `alpha' is already provided for you
    }

    BsdfSample sample(const Vector &wo, Sampler &rng) const {
        // NOT_IMPLEMENTED
        Vector wm = microfacet::sampleGGXVNDF(alpha, wo, rng.next2D());
        Vector wi = reflect(wo, wm);
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfSample::invalid();
        float pdf = microfacet::pdfGGXVNDF(alpha, wm, wo) / (4 * abs(wo.z()));
        return { wi, color * microfacet::smithG1(alpha, wm, wi), pdf };
        // hints:
        // * copy your roughconductor bsdf sample here
        // * you do not need to query textures
        //   * the reflectance is given by `color'
        //   * the variable `alpha' is already provided for you
    }
};

struct ClearcoatLobe {
    float alpha;
    Color color;

    BsdfEval evaluate(const Vector &wo, const Vector &wi) const {
        Vector wm = (wi + wo).normalized();
        float F = schlick(0.04f, wm.dot(wo));
        float D = microfacet::evaluateGTR1((1.f - alpha) * 0.1f + alpha * 0.001f, wm);
        float G1_i = microfacet::smithG1(0.25f, wm, wi);
        float G1_o = microfacet::smithG1(0.25f, wm, wo);
        Color value = color * F * D * G1_i * G1_o / (4.f * Frame::absCosTheta(wo));

        float pdf = D * Frame::absCosTheta(wm) / (4.f * wo.dot(wm));
        return BsdfEval { value, pdf };
    }

    BsdfSample sample(const Vector &wo, Sampler &rng) const {
        Vector wm = microfacet::sampleGTR1(0.25f, rng.next2D());
        if (wm.dot(wo) < 0.f) wm = -wm;

        Vector wi = reflect(wo, wm);
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfSample::invalid();

        float F = schlick(0.04f, wm.dot(wo));
        float D = microfacet::evaluateGTR1((1.f - alpha) * 0.1f + alpha * 0.001f, wm);
        float G1_i = microfacet::smithG1(0.25f, wm, wi);
        float G1_o = microfacet::smithG1(0.25f, wm, wo);

        float pdf = D * Frame::absCosTheta(wm) / (4.f * wo.dot(wm));
        if (pdf < Epsilon) return BsdfSample::invalid();

        Color weight = color * F * D * G1_i * G1_o / (4.f * Frame::absCosTheta(wo) * pdf);
        return BsdfSample { wi, weight, pdf };
    }
};

class PrincipledClearcoat : public Bsdf {
    ref<Texture> m_baseColor;
    ref<Texture> m_roughness;
    ref<Texture> m_metallic;
    ref<Texture> m_specular;
    ref<Texture> m_clearcoat;
    ref<Texture> m_clearcoatGloss;

    struct Combination {
        float diffuseSelectionProb;
        float metallicSelectionProb;
        float clearcoatSelectionProb;
        DiffuseLobe diffuse;
        MetallicLobe metallic;
        ClearcoatLobe clearcoat;
    };

    Combination combine(const Point2 &uv, const Vector &wo) const {
        const auto baseColor = m_baseColor->evaluate(uv);
        const auto alpha     = max(float(1e-3), sqr(m_roughness->scalar(uv)));
        const auto specular  = m_specular->scalar(uv);
        const auto metallic  = m_metallic->scalar(uv);
        const auto clearcoat = m_clearcoat->scalar(uv);
        const auto clearcoatGloss = m_clearcoatGloss->scalar(uv);
        const auto F =
            specular * schlick((1 - metallic) * 0.08f, Frame::cosTheta(wo));

        const DiffuseLobe diffuseLobe = {
            .color = (1 - F) * (1 - metallic) * baseColor,
        };
        const MetallicLobe metallicLobe = {
            .alpha = alpha,
            .color = F * Color(1) + (1 - F) * metallic * baseColor,
        };
        const ClearcoatLobe clearcoatLobe = {
            .alpha = clearcoatGloss,
            .color = Color(clearcoat),
        };

        float clearcoatWeight = saturate(clearcoat);
        float diffuseWeight = diffuseLobe.color.mean();
        float metallicWeight = metallicLobe.color.mean();
        float norm = 1.f / (clearcoatWeight + diffuseWeight + metallicWeight);

        return {
            .diffuseSelectionProb = diffuseWeight * norm,
            .metallicSelectionProb = metallicWeight * norm,
            .clearcoatSelectionProb = clearcoatWeight * norm,
            .diffuse  = diffuseLobe,
            .metallic = metallicLobe,
            .clearcoat = clearcoatLobe,
        };
    }

public:
    PrincipledClearcoat(const Properties &properties) {
        m_baseColor = properties.get<Texture>("baseColor");
        m_roughness = properties.get<Texture>("roughness");
        m_metallic  = properties.get<Texture>("metallic");
        m_specular  = properties.get<Texture>("specular");
        m_clearcoat = properties.get<Texture>("clearcoat");
        m_clearcoatGloss = properties.get<Texture>("clearcoatGloss");
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {
        PROFILE("Principled")

        const auto combination = combine(uv, wo);

        Vector wm = (wi + wo).normalized();
        float Fc = schlick(0.04f, wo.dot(wm)) * combination.clearcoat.color.mean();

        return { (combination.diffuse.evaluate(wo, wi).value +
                 combination.metallic.evaluate(wo, wi).value) * (1.f - Fc) +
                 combination.clearcoat.evaluate(wo, wi).value,
                 combination.diffuse.evaluate(wo, wi).pdf * combination.diffuseSelectionProb +
                 combination.metallic.evaluate(wo, wi).pdf * combination.metallicSelectionProb +
                 combination.clearcoat.evaluate(wo, wi).pdf * combination.clearcoatSelectionProb };
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        PROFILE("PrincipledClearCoat")

        const auto combination = combine(uv, wo);
        float p = rng.next();
        if (p <= combination.clearcoatSelectionProb) {
            BsdfSample s = combination.clearcoat.sample(wo, rng);
            s.weight /= combination.clearcoatSelectionProb;
            return { s.wi, s.weight, combination.clearcoatSelectionProb * s.pdf };
        } else if (p > combination.clearcoatSelectionProb && p <= (combination.clearcoatSelectionProb + combination.metallicSelectionProb)) {
            BsdfSample s = combination.metallic.sample(wo, rng);
            Vector wm = (wo + s.wi).normalized();
            float Fc = schlick(0.04f, wo.dot(wm)) * combination.clearcoat.color.mean();
            s.weight *= (1.f - Fc);
            s.weight /= combination.metallicSelectionProb;
            return { s.wi, s.weight, combination.metallicSelectionProb * s.pdf };
        } else {
            BsdfSample s = combination.diffuse.sample(wo, rng);
            Vector wm = (wo + s.wi).normalized();
            float Fc = schlick(0.04f, wo.dot(wm)) * combination.clearcoat.color.mean();
            s.weight *= (1.f - Fc);
            s.weight /= combination.diffuseSelectionProb;
            return { s.wi, s.weight, combination.diffuseSelectionProb * s.pdf };
        }
    }

    std::string toString() const override {
        return tfm::format(
            "PrincipledClearcoat[\n"
            "  baseColor = %s,\n"
            "  roughness = %s,\n"
            "  metallic  = %s,\n"
            "  specular  = %s,\n"
            "  clearcoat = %s,\n"
            "  clearcoatGloss = %s,\n"
            "]",
            indent(m_baseColor),
            indent(m_roughness),
            indent(m_metallic),
            indent(m_specular),
            indent(m_clearcoat),
            indent(m_clearcoatGloss));
    }
};

} // namespace lightwave

REGISTER_BSDF(PrincipledClearcoat, "principled_clearcoat")
