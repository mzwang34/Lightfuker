#include <lightwave.hpp>

namespace lightwave {

class Diffuse : public Bsdf {
    ref<Texture> m_albedo;

public:
    Diffuse(const Properties &properties) {
        m_albedo = properties.get<Texture>("albedo");
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfEval::invalid();
        return BsdfEval{ m_albedo.get()->evaluate(uv) * InvPi *
                         Frame::cosTheta(wi) };
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        // uniform
        // Vector wi = squareToUniformHemisphere(rng.next2D());
        // Color weight = Frame::cosTheta(wi) * m_albedo.get()->evaluate(uv) *
        // InvPi / uniformHemispherePdf();

        // cos
        Vector wi = squareToCosineHemisphere(rng.next2D());
        if (Frame::cosTheta(wo) <= 0)
            wi = -wi;
        Color weight = m_albedo.get()->evaluate(uv);
        if (!Frame::sameHemisphere(wi, wo))
            return BsdfSample::invalid();

        return BsdfSample{ wi, weight };
    }

    std::string toString() const override {
        return tfm::format(
            "Diffuse[\n"
            "  albedo = %s\n"
            "]",
            indent(m_albedo));
    }
};

} // namespace lightwave

REGISTER_BSDF(Diffuse, "diffuse")
