#include <lightwave.hpp>

namespace lightwave {

class pathTracerMISIntegrator : public SamplingIntegrator {
    int m_depth;
    std::string m_strategy;

public:
    pathTracerMISIntegrator(const Properties &properties)
        : SamplingIntegrator(properties) {
        m_depth = properties.get<int>("depth", 2);
        m_strategy = properties.get<std::string>("strategy", "mis");
    }

    Color Li(const Ray &ray, Sampler &rng) override {
        if (m_strategy == "mis") {
            Ray _ray = ray;
            Color throughput(1.f);
            Color c(0.f);
            float pre_bsdf = 0.f;
            for (int path_len = 0; path_len < m_depth; path_len++) {
                Intersection its = m_scene->intersect(_ray, rng);

                // hit background
                if (!its) {
                    if (its.background) {
                        if (path_len == 0) {
                            c += throughput * its.evaluateEmission().value;
                        } else {
                            float pdf_light = its.evaluateEmission().pdf * its.lightProbability;
                            float mis_bsdf = pre_bsdf / (pre_bsdf + pdf_light);
                            c += throughput * its.evaluateEmission().value * mis_bsdf;
                        }
                    }
                    break;
                }
            
                // hit light source
                if (its.evaluateEmission()) {
                    if (path_len == 0) {
                        c += its.evaluateEmission().value * throughput;
                    } else {
                        float cosTheta = std::abs(its.shadingNormal.dot(-_ray.direction));
                        float pdf_light = its.pdf * its.lightProbability * its.t * its.t / std::max(cosTheta, Epsilon);
                        float mis_bsdf = pre_bsdf / (pre_bsdf + pdf_light);
                        c += mis_bsdf * throughput * its.evaluateEmission().value;
                    }
                }

                if (path_len == m_depth - 1)
                    break;

                if (m_scene->hasLights()) {
                    LightSample lightSample = m_scene->sampleLight(rng);             
                    if (lightSample) {
                        const Light *light = lightSample.light;
                        DirectLightSample dSample = light->sampleDirect(its.position, rng);
                        if (!dSample.isInvalid()) {
                            Ray shadowRay{ its.position, dSample.wi };

                            float pdf_light = dSample.pdf * lightSample.probability;
                            float pdf_bsdf = its.evaluateBsdf(dSample.wi).pdf;
                            float light_mis = light->canBeIntersected() ? pdf_light / (pdf_light + pdf_bsdf) : 1.f;

                            float trans = m_scene->transmittance(
                                shadowRay, dSample.distance, rng);
                            if (trans > 0.f)
                                c += light_mis * trans * throughput * its.evaluateBsdf(dSample.wi).value * dSample.weight / lightSample.probability;
                        }
                    }
                }

                BsdfSample bsdfSample = its.sampleBsdf(rng);
                if (bsdfSample.isInvalid()) break;
                throughput *= bsdfSample.weight;
                _ray = Ray(its.position, bsdfSample.wi);
                pre_bsdf = bsdfSample.pdf;
            }
            return c;
        } else if (m_strategy == "nee") {
            Ray _ray = ray;
            Color throughput(1.f);
            Color c(0.f);
            for (int path_len = 0; path_len < m_depth; path_len++) {
                Intersection its = m_scene->intersect(_ray, rng);
                if (!its) {
                    if (its.background) {
                        if (path_len == 0) {
                            c += throughput * its.evaluateEmission().value;
                        }
                    }
                    break;
                }

                if (its.evaluateEmission()) {
                    if (path_len == 0) {
                        c += its.evaluateEmission().value * throughput;
                    }
                }

                if (path_len == m_depth - 1)
                    break;

                if (m_scene->hasLights()) {
                    LightSample lightSample = m_scene->sampleLight(rng);             
                    if (lightSample) {
                        const Light *light = lightSample.light;
                        DirectLightSample dSample = light->sampleDirect(its.position, rng);
                        if (!dSample.isInvalid()) {
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
        } else if (m_strategy == "bsdf") {
            Ray _ray = ray;
            Color throughput(1.f);
            Color c(0.f);
            for (int path_len = 0; path_len < m_depth; path_len++) {
                Intersection its = m_scene->intersect(_ray, rng);

                if (!its) {
                    if (its.background) {
                        c += throughput * its.evaluateEmission().value;
                    }
                    break;
                }

                c += throughput * its.evaluateEmission().value;

                if (path_len == m_depth - 1)
                    break;

                BsdfSample bsdfSample = its.sampleBsdf(rng);
                if (bsdfSample.isInvalid()) break;
                throughput *= bsdfSample.weight;
                _ray = Ray(its.position, bsdfSample.wi);
            }
            return c;
        }
    }

    /// @brief An optional textual representation of this class, which can be
    /// useful for debugging.
    std::string toString() const override {
        return tfm::format("pathTracerMISIntegrator[\n"
                           "  sampler = %s,\n"
                           "  image = %s,\n"
                           "]",
                           indent(m_sampler), indent(m_image));
    }
};

} // namespace lightwave

REGISTER_INTEGRATOR(pathTracerMISIntegrator, "pathtracer_mis")