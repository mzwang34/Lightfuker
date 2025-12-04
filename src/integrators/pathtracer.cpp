#include <lightwave.hpp>

namespace lightwave {

class pathTracerIntegrator : public SamplingIntegrator {
    int m_depth;
    bool m_nee;

public:
    pathTracerIntegrator(const Properties &properties)
        : SamplingIntegrator(properties) {
        m_depth = properties.get<int>("depth", 2);
        m_nee   = properties.get<bool>("nee", true);
    }

    Color Li(const Ray &ray, Sampler &rng) override {
        Ray _ray = ray;
        Color throughput(1.f);
        Color c(0.f);
        for (int path_len = 0; path_len < m_depth; path_len++) {
            Intersection its = m_scene->intersect(_ray, rng);
            if (!its) {
                c += throughput * its.evaluateEmission().value;
                break;
            }

            c += its.evaluateEmission().value * throughput;

            if (path_len == m_depth - 1)
                break;

            if (m_nee && m_scene-> hasLights()) {
                LightSample lightSample = m_scene->sampleLight(rng);             
                if (lightSample) {
                    const Light *light = lightSample.light;
                    if (!light->canBeIntersected()) {
                        DirectLightSample dSample = light->sampleDirect(its.position, rng);

                        Ray shadowRay{ its.position, dSample.wi };
                        float trans = m_scene->transmittance(
                            shadowRay, dSample.distance, rng);
                        if (trans > 0.f)
                            c += trans * throughput * its.evaluateBsdf(dSample.wi).value * dSample.weight / lightSample.probability;
                    }
                }
            }

            BsdfSample bsdfSample = its.sampleBsdf(rng);
            if (bsdfSample.isInvalid()) break;
            throughput *= bsdfSample.weight;
            _ray = Ray(its.position, bsdfSample.wi);
        }
        return c;
    }

    /// @brief An optional textual representation of this class, which can be
    /// useful for debugging.
    std::string toString() const override {
        return tfm::format("pathTracerIntegrator[\n"
                           "  sampler = %s,\n"
                           "  image = %s,\n"
                           "]",
                           indent(m_sampler), indent(m_image));
    }
};

} // namespace lightwave

REGISTER_INTEGRATOR(pathTracerIntegrator, "pathtracer")
