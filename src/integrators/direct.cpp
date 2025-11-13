#include <lightwave.hpp>

namespace lightwave {

class DirectIntegrator : public SamplingIntegrator {

public:
    DirectIntegrator(const Properties &properties)
        : SamplingIntegrator(properties) {
        // to parse properties from the scene description, use
        // properties.get(name, default_value) you can also omit the default
        // value if you want to require the user to specify a value
    }

    /**
     * @brief The job of an integrator is to return a color for a ray produced
     * by the camera model. This will be run for each pixel of the image,
     * potentially with multiple samples for each pixel.
     */
    Color Li(const Ray &ray, Sampler &rng) override {
        Intersection its = m_scene->intersect(ray, rng);
        if (its) {
            auto light_sample = m_scene->sampleLight(rng);
            auto sample = light_sample.light->sampleDirect(its.position, rng);
            auto direct_ray = Ray(its.position, sample.wi);
            auto its2       = m_scene->intersect(direct_ray, rng);
            if (its2.t < sample.distance) {
                return Color(0.f);
            } else {
                return sample.weight * its.evaluateBsdf(sample.wi).value;
            }
        } else {
            return its.evaluateEmission().value;
        }
    }

    /// @brief An optional textual representation of this class, which can be
    /// useful for debugging.
    std::string toString() const override {
        return tfm::format("DirectIntegrator");
    }
};

} // namespace lightwave

REGISTER_INTEGRATOR(DirectIntegrator, "direct")
