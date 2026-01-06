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
        float pdf = microfacet::pdfGGXVNDF(alpha, wm, wo) / (4 * max(abs(wo.dot(wm)), Epsilon));
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
        float pdf = microfacet::pdfGGXVNDF(alpha, wm, wo) / (4 * max(abs(wo.dot(wm)), Epsilon));
        return { wi, color * microfacet::smithG1(alpha, wm, wi), pdf };
        // hints:
        // * copy your roughconductor bsdf sample here
        // * you do not need to query textures
        //   * the reflectance is given by `color'
        //   * the variable `alpha' is already provided for you
    }
};

class Principled : public Bsdf {
    ref<Texture> m_baseColor;
    ref<Texture> m_roughness;
    ref<Texture> m_metallic;
    ref<Texture> m_specular;

    struct Combination {
        float diffuseSelectionProb;
        DiffuseLobe diffuse;
        MetallicLobe metallic;
    };

    Combination combine(const Point2 &uv, const Vector &wo) const {
        const auto baseColor = m_baseColor->evaluate(uv);
        const auto alpha     = max(float(1e-3), sqr(m_roughness->scalar(uv)));
        const auto specular  = m_specular->scalar(uv);
        const auto metallic  = m_metallic->scalar(uv);
        const auto F =
            specular * schlick((1 - metallic) * 0.08f, Frame::cosTheta(wo));

        const DiffuseLobe diffuseLobe = {
            .color = (1 - F) * (1 - metallic) * baseColor,
        };
        const MetallicLobe metallicLobe = {
            .alpha = alpha,
            .color = F * Color(1) + (1 - F) * metallic * baseColor,
        };

        const auto diffuseAlbedo = diffuseLobe.color.mean();
        const auto totalAlbedo =
            diffuseLobe.color.mean() + metallicLobe.color.mean();
        return {
            .diffuseSelectionProb =
                totalAlbedo > 0 ? diffuseAlbedo / totalAlbedo : 1.0f,
            .diffuse  = diffuseLobe,
            .metallic = metallicLobe,
        };
    }

public:
    Principled(const Properties &properties) {
        m_baseColor = properties.get<Texture>("baseColor");
        m_roughness = properties.get<Texture>("roughness");
        m_metallic  = properties.get<Texture>("metallic");
        m_specular  = properties.get<Texture>("specular");
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {
        PROFILE("Principled")

        const auto combination = combine(uv, wo);
        // NOT_IMPLEMENTED
        return { combination.diffuse.evaluate(wo, wi).value +
                 combination.metallic.evaluate(wo, wi).value,
                 combination.diffuse.evaluate(wo, wi).pdf * combination.diffuseSelectionProb +
                 combination.metallic.evaluate(wo, wi).pdf * (1 - combination.diffuseSelectionProb) };
        // hint: evaluate `combination.diffuse` and `combination.metallic` and
        // combine their results
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        PROFILE("Principled")

        const auto combination = combine(uv, wo);
        // NOT_IMPLEMENTED
        if (rng.next() < combination.diffuseSelectionProb) {
            BsdfSample s = combination.diffuse.sample(wo, rng);
            s.weight /= combination.diffuseSelectionProb;
            return { s.wi, s.weight, combination.diffuseSelectionProb * s.pdf };
        } else {
            BsdfSample s = combination.metallic.sample(wo, rng);
            s.weight /= (1.f - combination.diffuseSelectionProb);
            return { s.wi, s.weight, (1.f - combination.diffuseSelectionProb) * s.pdf };
        }

        // hint: sample either `combination.diffuse` (probability
        // `combination.diffuseSelectionProb`) or `combination.metallic`
    }

    Color getAlbedo(const Intersection &its) const override {
        return m_baseColor->evaluate(its.uv);
    }

    std::string toString() const override {
        return tfm::format(
            "Principled[\n"
            "  baseColor = %s,\n"
            "  roughness = %s,\n"
            "  metallic  = %s,\n"
            "  specular  = %s,\n"
            "]",
            indent(m_baseColor),
            indent(m_roughness),
            indent(m_metallic),
            indent(m_specular));
    }
};

} // namespace lightwave

REGISTER_BSDF(Principled, "principled")
