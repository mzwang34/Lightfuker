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
        NOT_IMPLEMENTED
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        NOT_IMPLEMENTED
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
