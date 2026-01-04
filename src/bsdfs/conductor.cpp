#include <lightwave.hpp>

namespace lightwave {

class Conductor : public Bsdf {
    ref<Texture> m_reflectance;

public:
    Conductor(const Properties &properties) {
        m_reflectance = properties.get<Texture>("reflectance");
    }

    BsdfEval evaluate(const Point2 &uv, const Vector &wo,
                      const Vector &wi) const override {
        // the probability of a light sample picking exactly the direction `wi'
        // that results from reflecting `wo' is zero, hence we can just ignore
        // that case and always return black
        return BsdfEval::invalid();
    }

    BsdfSample sample(const Point2 &uv, const Vector &wo,
                      Sampler &rng) const override {
        // NOT_IMPLEMENTED
        Vector wi    = Vector(-wo.x(), -wo.y(), wo.z());
        Color weight = m_reflectance.get()->evaluate(uv);
        return BsdfSample{ wi, weight, 1.f };
    }

    std::string toString() const override {
        return tfm::format(
            "Conductor[\n"
            "  reflectance = %s\n"
            "]",
            indent(m_reflectance));
    }

    Color getAlbedo(const Intersection &its) const override {
        return m_reflectance->evaluate(its.uv);
    }
};

} // namespace lightwave

REGISTER_BSDF(Conductor, "conductor")
