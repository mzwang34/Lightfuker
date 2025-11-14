#include <lightwave.hpp>

namespace lightwave {

class DirectIntegrator : public SamplingIntegrator {
public:
    DirectIntegrator(const Properties &properties)
        : SamplingIntegrator(properties) {}

    Color Li(const Ray &ray, Sampler &rng) override {
        Intersection its = m_scene->intersect(ray, rng);

        if (!its)
            return its.evaluateEmission().value;

        Color c(0.f);
        if (m_scene->hasLights()) {
            LightSample lightSample = m_scene->sampleLight(rng);
            if (lightSample) {
                const Light *light = lightSample.light;
                DirectLightSample dSample =
                    light->sampleDirect(its.position, rng);

                Ray shadowRay{ its.position, dSample.wi };
                Intersection shadowIts = m_scene->intersect(shadowRay, rng);
                if (shadowIts.t >= dSample.distance) {
                    c += dSample.weight * its.evaluateBsdf(dSample.wi).value /
                         lightSample.probability;
                }
            }
        }

        c += its.evaluateEmission().value;
        BsdfSample bsdfSample = its.sampleBsdf(rng);
        if (bsdfSample.isInvalid())
            return c;
        Ray newRay{ its.position, bsdfSample.wi };
        Intersection nextIts = m_scene->intersect(newRay, rng);
        c += nextIts.evaluateEmission().value * bsdfSample.weight;

        return c;
    }

    /// @brief An optional textual representation of this class, which can be
    /// useful for debugging.
    std::string toString() const override {
        return tfm::format("DirectIntegrator");
    }
};

} // namespace lightwave

REGISTER_INTEGRATOR(DirectIntegrator, "direct")
