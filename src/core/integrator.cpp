#include <lightwave/camera.hpp>
#include <lightwave/integrator.hpp>
#include <lightwave/parallel.hpp>

#include <algorithm>
#include <chrono>

#include <lightwave/iterators.hpp>
#include <lightwave/streaming.hpp>

namespace lightwave {

void SamplingIntegrator::execute() {
    if (!m_image) {
        lightwave_throw(
            "<integrator /> needs an <image /> child to render into!");
    }

    const Vector2i resolution = m_scene->camera()->resolution();
    m_image->initialize(resolution);

    Streaming stream{ *m_image };
    ProgressReporter progress{ resolution.product() *
                               long(m_sampler->samplesPerPixel()) };
    float norm = 0;

    const bool renderProgressively =
        resolution.product() * long(m_sampler->samplesPerPixel()) > 100000000l;

    for (auto spps :
         GeometricallyChunkedRange(m_sampler->samplesPerPixel(), 1024)) {
        if (!renderProgressively)
            spps = Range(0, m_sampler->samplesPerPixel());

        norm = 1.0f / float(*spps.end());

        for_each_parallel(
            BlockSpiral(resolution, Vector2i(64)), [&](auto block) {
                auto sampler = m_sampler->clone();
                for (auto pixel : block) {
                    Color sum;
                    for (auto sample : spps) {
                        sampler->seed(pixel, sample);
                        auto cameraSample =
                            m_scene->camera()->sample(pixel, *sampler);
                        sum += cameraSample.weight *
                               Li(cameraSample.ray, *sampler);
                    }
                    m_image->get(pixel) += sum;
                }

                progress += block.diagonal().product() *
                            long(*spps.end() - *spps.begin());
                stream.normalize(norm);
                stream.updateBlock(block);
            });

        logger(EInfo,
               "finished %d spp (%d this iteration) after %.2f seconds",
               *spps.end(),
               spps.count(),
               progress.getElapsedTime());

        m_image->save(norm);

        if (!renderProgressively)
            break;
    }

    // normalize the image such that the data inside the image is correct
    *m_image *= norm;

    progress.finish();
}

} // namespace lightwave
