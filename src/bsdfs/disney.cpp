#include "disney.hpp"
#include "fresnel.hpp"
#include "microfacet.hpp"

#include <lightwave.hpp>

namespace lightwave {

struct DisneyDiffuse {
    Color baseColor;
    float roughness;
    float subsurface;

    BsdfEval evaluate(const Vector &wo, const Vector &wi) const {
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfEval::invalid();

        Vector wh = (wi + wo).normalized();
        float cosTheta_o = Frame::absCosTheta(wo);
        float cosTheta_i = Frame::absCosTheta(wi);

        float FD90 = F_D90(roughness, wh, wi);
        Color f_baseDiffuse = baseColor * InvPi * F(FD90, wi) * F(FD90, wo) * cosTheta_i;

        float FSS90 = F_SS90(roughness, wh, wi);
        Color f_subsurface = 1.25f * baseColor * InvPi * (F(FSS90, wi) * F(FSS90, wo) * (1.f / (cosTheta_i + cosTheta_o) - 0.5f) + 0.5f) * cosTheta_i;

        Color value = (1 - subsurface) * f_baseDiffuse + subsurface * f_subsurface;
        return BsdfEval { value, cosTheta_i * InvPi };
    }

    BsdfSample sample(const Vector &wo, Sampler &rng) const {
        Vector wi = squareToCosineHemisphere(rng.next2D());
        if (Frame::cosTheta(wo) <= 0)
            wi = -wi;
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfSample::invalid();
        BsdfEval e = evaluate(wo, wi);
        if (e.pdf < Epsilon)
            return BsdfSample::invalid();
        return BsdfSample { wi, e.value / e.pdf, e.pdf };
    }
};

struct DisneyMetal {
    Color baseColor;
    float anisotropic;
    float roughness;
    float specularTint;
    float specular;
    float metallic;
    float eta;

    BsdfEval evaluate(const Vector &wo, const Vector &wi) const {
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfEval::invalid();

        Vector wh = (wi + wo).normalized();
        Color c_tint = baseColor.luminance() > 0 ? baseColor / baseColor.luminance() : Color(1);
        Color ks = Color(1.f - specularTint) + specularTint * c_tint;
        Color C0 = specular * R0(eta) * (1.f - metallic) * ks + metallic * baseColor;
        Color Fm = schlick(C0, wh.dot(wi));
        float aspect = sqrt(1.f - 0.9f * anisotropic);
        float ax = max(0.0001f, sqr(roughness) / aspect);
        float ay = max(0.0001f, sqr(roughness) * aspect);
        float Dm = microfacet::evaluateAnisotropicGGX(ax, ay, wh);
        float Gm = microfacet::anisotropicSmithG1(ax, ay, wh, wi) * microfacet::anisotropicSmithG1(ax, ay, wh, wo);
        Color f_metal = Fm * Dm * Gm / (4.f * Frame::absCosTheta(wo));

        float pdf = microfacet::pdfAnisotropicGGXVNDF(ax, ay, wh, wo) / (4.f * abs(wh.dot(wo)));
        
        return BsdfEval { f_metal, pdf };
    }

    BsdfSample sample(const Vector &wo, Sampler &rng) const {
        float aspect = sqrt(1.f - 0.9f * anisotropic);
        float ax = max(0.0001f, sqr(roughness) / aspect);
        float ay = max(0.0001f, sqr(roughness) * aspect);
        Vector wh = microfacet::sampleAnisotropicGGXVNDF(ax, ay, wo, rng.next2D());
        Vector wi = reflect(wo, wh);
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfSample::invalid();
        BsdfEval e = evaluate(wo, wi);
        if (e.pdf < Epsilon)
            return BsdfSample::invalid();
        return BsdfSample { wi, e.value / e.pdf, e.pdf };
    }
};

struct DisneyClearcoat {
    Color baseColor;
    float clearcoatGloss;

    BsdfEval evaluate(const Vector &wo, const Vector &wi) const {
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfEval::invalid();

        Vector wh = (wi + wo).normalized();
        float cosTheta_o = Frame::absCosTheta(wo);

        float Fc = schlick(0.04f, abs(wh.dot(wi)));
        float alpha = (1.f - clearcoatGloss) * 0.1f + clearcoatGloss * 0.001f;
        float Dc = microfacet::evaluateGTR1(alpha, wh);
        float Gc = microfacet::anisotropicSmithG1(0.25f, 0.25f, wh, wi) * microfacet::anisotropicSmithG1(0.25f, 0.25f, wh, wo);
        float f_clearcoat = Fc * Dc * Gc / (4.f * cosTheta_o);

        float pdf = Dc * Frame::absCosTheta(wh) / (4.f * abs(wh.dot(wi)));

        return BsdfEval { Color(f_clearcoat), pdf };
    }

    BsdfSample sample(const Vector &wo, Sampler &rng) const {
        float alpha = (1.f - clearcoatGloss) * 0.1f + clearcoatGloss * 0.001f;
        Vector wh = microfacet::sampleGTR1(alpha, rng.next2D());
        if (wh.dot(wo) < 0.f) wh = -wh;
        Vector wi = reflect(wo, wh);
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfSample::invalid();
        BsdfEval e = evaluate(wo, wi);
        if (e.pdf < Epsilon)
            return BsdfSample::invalid();
        return BsdfSample { wi, e.value / e.pdf, e.pdf };
    }
};

struct DisneyGlass {
    Color baseColor;
    float eta;
    float anisotropic;
    float roughness;

    BsdfEval evaluate(const Vector &wo, const Vector &wi) const {
        bool reflect     = Frame::sameHemisphere(wi, wo);
        float cosTheta_o = Frame::cosTheta(wo);
        float cosTheta_i = Frame::cosTheta(wi);
        float etap = 1;
        if (!reflect) {
            etap = cosTheta_o > 0 ? eta : (1 / eta);
        }
        Vector wh = etap * wi + wo;
        if (cosTheta_i == 0 || cosTheta_o == 0 || wh.lengthSquared() == 0) 
            return BsdfEval::invalid();
            wh = wh.normalized();
        if (wh.z() < 0.f) wh = -wh;
        
        if (wh.dot(wi) * cosTheta_i < 0 || wh.dot(wo) * cosTheta_o < 0)
            return BsdfEval::invalid();
        
        float HdotI = wh.dot(wi);
        float HdotO = wh.dot(wo);

        float Fg = fresnelDielectric(HdotO, eta);
        float aspect = sqrt(1.f - 0.9f * anisotropic);
        float ax = max(0.0001f, sqr(roughness) / aspect);
        float ay = max(0.0001f, sqr(roughness) * aspect);
        float Dg = microfacet::evaluateAnisotropicGGX(ax, ay, wh);
        float Gg = microfacet::anisotropicSmithG1(ax, ay, wh, wi) * microfacet::anisotropicSmithG1(ax, ay, wh, wo);
        
        Color f_glass;
        float pdf;
        if (reflect) {
            f_glass = baseColor * Fg * Dg * Gg / (4.f * abs(cosTheta_o));
            pdf = Fg * microfacet::pdfAnisotropicGGXVNDF(ax, ay, wh, wo) * microfacet::detReflection(wh, wo);
        } else {
            f_glass = sqrt(baseColor) * (1.f - Fg) * Dg * Gg * abs(HdotI * HdotO) / (abs(cosTheta_o) * sqr(HdotO + etap * HdotI));
            pdf = (1.f - Fg) * microfacet::pdfAnisotropicGGXVNDF(ax, ay, wh, wo) * microfacet::detRefraction(wh, wi, wo, etap);
        }

        return BsdfEval { f_glass, pdf };                
    }

    BsdfSample sample(const Vector &wo, Sampler &rng) const {
        float aspect = sqrt(1.f - 0.9f * anisotropic);
        float ax = max(0.0001f, sqr(roughness) / aspect);
        float ay = max(0.0001f, sqr(roughness) * aspect);
        Vector wh = microfacet::sampleAnisotropicGGXVNDF(ax, ay, wo, rng.next2D());
        float etap = Frame::cosTheta(wo) > 0 ? eta : (1 / eta);
        
        float Fg = fresnelDielectric(wh.dot(wo), etap);
        Vector wi;
        Color weight;
        float pdf;
        if (rng.next() < Fg) {
            wi = reflect(wo, wh);
            if (!Frame::sameHemisphere(wi, wo)) 
                return BsdfSample::invalid();
            BsdfEval e = evaluate(wo, wi);
            if (e.pdf < Epsilon)
                return BsdfSample::invalid();
            weight = e.value / e.pdf;
            pdf = e.pdf;
        } else {
            wi = refract(wo, wh, etap);
            if (Frame::sameHemisphere(wi, wo) || wi.z() == 0 || wi.isZero())
                return BsdfSample::invalid();
            BsdfEval e = evaluate(wo, wi);
            if (e.pdf < Epsilon)
                return BsdfSample::invalid();
            weight = e.value / e.pdf;
            pdf = e.pdf;
        }
        return BsdfSample { wi, weight, pdf };
    }
};

struct DisneySheen {
    Color baseColor;
    float sheenTint;

    BsdfEval evaluate(const Vector &wo, const Vector &wi) const {
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfEval::invalid();

        Vector wh = (wi + wo).normalized();    
        Color c_tint = baseColor.luminance() > 0 ? baseColor / baseColor.luminance() : Color(1);
        Color c_sheen = (1.f - sheenTint) * Color(1) + sheenTint * c_tint;
        Color f_sheen = c_sheen * pow(1 - abs(wh.dot(wi)), 5) * Frame::absCosTheta(wi);

        return BsdfEval { f_sheen, Frame::cosTheta(wi) * InvPi };
    }

    BsdfSample sample(const Vector &wo, Sampler &rng) const {
        Vector wi = squareToCosineHemisphere(rng.next2D());
        if (Frame::cosTheta(wo) <= 0)
            wi = -wi;
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfSample::invalid();
        BsdfEval e = evaluate(wo, wi);
        if (e.pdf < Epsilon)
            return BsdfSample::invalid();
        return BsdfSample { wi, e.value / e.pdf, e.pdf };
    }
};

class Disney : public Bsdf {
    ref<Texture> m_subsurface;
    ref<Texture> m_metallic;
    ref<Texture> m_specular;
    ref<Texture> m_specularTint;
    ref<Texture> m_specularTrans;
    ref<Texture> m_roughness;
    ref<Texture> m_anisotropic;
    ref<Texture> m_sheen;
    ref<Texture> m_sheenTint;
    ref<Texture> m_clearcoat;
    ref<Texture> m_clearcoatGloss;
    ref<Texture> m_baseColor;
    float m_eta;

    struct Combination {
        DisneyDiffuse diffuse;
        DisneyMetal metal;
        DisneyClearcoat clearcoat;
        DisneyGlass glass;
        DisneySheen sheen;
    };

public:
    Disney(const Properties &properties) {
        m_subsurface = properties.get<Texture>("subsurface", nullptr);
        m_metallic = properties.get<Texture>("metallic", nullptr);
        m_specular = properties.get<Texture>("specular", nullptr);
        m_specularTint = properties.get<Texture>("specularTint", nullptr);
        m_specularTrans = properties.get<Texture>("specularTrans", nullptr);
        m_roughness = properties.get<Texture>("roughness", nullptr);
        m_anisotropic = properties.get<Texture>("anisotropic", nullptr);
        m_sheen = properties.get<Texture>("sheen", nullptr);
        m_sheenTint = properties.get<Texture>("sheenTint", nullptr);
        m_clearcoat = properties.get<Texture>("clearcoat", nullptr);
        m_clearcoatGloss = properties.get<Texture>("clearcoatGloss", nullptr);
        m_baseColor = properties.get<Texture>("baseColor", nullptr);
        m_eta = properties.get<float>("eta", 1.5f);
    }

    Combination combine(const Point2 &uv, const Vector &wo) const {
        const auto baseColor = m_baseColor? m_baseColor->evaluate(uv) : Color::white();
        const auto roughness = m_roughness? m_roughness->scalar(uv) : 0.f;
        const auto subsurface = m_subsurface? m_subsurface->scalar(uv) : 0.f;
        const auto anisotropic = m_anisotropic? m_anisotropic->scalar(uv) : 0.f;
        const auto specularTint = m_specularTint? m_specularTint->scalar(uv) : 0.f;
        const auto specular = m_specular? m_specular->scalar(uv) : 0.f;
        const auto metallic = m_metallic? m_metallic->scalar(uv) : 0.f;
        const auto clearcoatGloss = m_clearcoatGloss? m_clearcoatGloss->scalar(uv) : 0.f;
        const auto sheenTint = m_sheenTint? m_sheenTint->scalar(uv) : 0.f;

        const DisneyDiffuse disneyDiffuse = {
            .baseColor = baseColor,
            .roughness = roughness,
            .subsurface = subsurface,
        };
        const DisneyMetal disneyMetal = {
            .baseColor = baseColor,
            .anisotropic = anisotropic,
            .roughness = roughness,
            .specularTint = specularTint,
            .specular = specular,
            .metallic = metallic,
            .eta = m_eta,
        };
        const DisneyClearcoat disneyClearcoat = {
            .baseColor = baseColor,
            .clearcoatGloss = clearcoatGloss,
        };
        const DisneyGlass disneyGlass = {
            .baseColor = baseColor,
            .eta = m_eta,
            .anisotropic = anisotropic,
            .roughness = roughness,
        };
        const DisneySheen disneySheen = {
            .baseColor = baseColor,
            .sheenTint = sheenTint,
        };

        return {
            .diffuse  = disneyDiffuse,
            .metal = disneyMetal,
            .clearcoat = disneyClearcoat,
            .glass = disneyGlass,
            .sheen = disneySheen
        };
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {
        const auto specularTrans = m_specularTrans? m_specularTrans->scalar(uv) : 0.f;
        const auto metallic = m_metallic? m_metallic->scalar(uv) : 0.f;
        const auto clearcoat = m_clearcoat? m_clearcoat->scalar(uv) : 0.f;
        const auto sheen = m_sheen? m_sheen->scalar(uv) : 0.f;
        
        float diffuseWeight = (1.f - specularTrans) * (1.f - metallic);
        float metalWeight = (1.f - specularTrans * (1.f - metallic));
        float glassWeight = (1.f - metallic) * specularTrans;
        float clearcoatWeight = 0.25f * clearcoat;
        float sheenWeight = (1.f - metallic) * sheen;

        const auto comb = combine(uv, wo);

        if (Frame::cosTheta(wo) < 0) {
            return { glassWeight * comb.glass.evaluate(wo, wi).value, 
                     comb.glass.evaluate(wo, wi).pdf };
        }

        Color f_disney = diffuseWeight * comb.diffuse.evaluate(wo, wi).value + 
                         sheenWeight * comb.sheen.evaluate(wo, wi).value +
                         metalWeight * comb.metal.evaluate(wo, wi).value + 
                         clearcoatWeight * comb.clearcoat.evaluate(wo, wi).value +
                         glassWeight * comb.glass.evaluate(wo, wi).value;

        float totalWeight = diffuseWeight + metalWeight + glassWeight + clearcoatWeight;
        float invW = 1.f / totalWeight;
        
        float pDiffuse = diffuseWeight * invW;
        float pMetal = metalWeight * invW;
        float pGlass = glassWeight * invW;
        float pClearcoat = clearcoatWeight * invW;

        float pdf = pDiffuse   * comb.diffuse.evaluate(wo, wi).pdf + 
                    pMetal     * comb.metal.evaluate(wo, wi).pdf + 
                    pClearcoat * comb.clearcoat.evaluate(wo, wi).pdf +
                    pGlass     * comb.glass.evaluate(wo, wi).pdf;

        return BsdfEval { f_disney, pdf };
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        const auto specularTrans = m_specularTrans? m_specularTrans->scalar(uv) : 0.f;
        const auto metallic = m_metallic? m_metallic->scalar(uv) : 0.f;
        const auto clearcoat = m_clearcoat? m_clearcoat->scalar(uv) : 0.f;
        const auto sheen = m_sheen? m_sheen->scalar(uv) : 0.f;

        const auto comb = combine(uv, wo);
        float p = rng.next();
        float diffuseWeight = (1.f - specularTrans) * (1.f - metallic);
        float metalWeight = 1.f - specularTrans * (1.f - metallic);
        float glassWeight = (1.f - metallic) * specularTrans;
        float clearcoatWeight = 0.25f * clearcoat;
        float sheenWeight = (1.f - metallic) * sheen;

        float totalWeight = diffuseWeight + metalWeight + glassWeight + clearcoatWeight;
        float invW = 1.f / totalWeight;
        
        float pDiffuse = diffuseWeight * invW;
        float pMetal = metalWeight * invW;
        float pGlass = glassWeight * invW;
        float pClearcoat = clearcoatWeight * invW;

        BsdfSample s;
        if (Frame::cosTheta(wo) < 0) {
            s = comb.glass.sample(wo, rng);
            return { s.wi, s.weight, pGlass * s.pdf };
        }

        if (p < pDiffuse) {
            s = comb.diffuse.sample(wo, rng);
            s.weight /= pDiffuse;
            return { s.wi, s.weight, pDiffuse * s.pdf };
        } else if (p < (pDiffuse + pMetal)) {
            s = comb.metal.sample(wo, rng);
            s.weight /= pMetal;
            return { s.wi, s.weight, pMetal * s.pdf };
        } else if (p < (pDiffuse + pMetal + pGlass)) {
            s = comb.glass.sample(wo, rng);
            s.weight /= pGlass;
            return { s.wi, s.weight, pGlass * s.pdf };
        } else {
            s = comb.clearcoat.sample(wo, rng);
            s.weight /= pClearcoat;
            return { s.wi, s.weight, pClearcoat * s.pdf };
        }
        return s;
    }

    Color getAlbedo(const Intersection &its) const override {
        return m_baseColor->evaluate(its.uv);
    }

    std::string toString() const override {
        return tfm::format(
            "Disney[\n"
            "  subsurface     = %s\n"
            "  metallic       = %s,\n"
            "  specular       = %s,\n"
            "  specularTint   = %s,\n"
            "  specularTrans  = %s,\n"
            "  roughness      = %s,\n"
            "  anisotropic    = %s,\n"
            "  sheen          = %s,\n"
            "  sheenTint      = %s,\n"
            "  clearcoat      = %s,\n"
            "  clearcoatGloss = %s,\n"
            "  baseColor      = %s\n"
            "]",
            indent(m_subsurface),
            indent(m_metallic),
            indent(m_specular),
            indent(m_specularTint),
            indent(m_specularTrans),
            indent(m_roughness),
            indent(m_anisotropic),
            indent(m_sheen),
            indent(m_sheenTint),
            indent(m_clearcoat),
            indent(m_clearcoatGloss),
            indent(m_baseColor));
    }
};

} // namespace lightwave

REGISTER_BSDF(Disney, "disney")
