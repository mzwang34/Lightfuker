#include <lightwave.hpp>

namespace lightwave {

class Bloom : public Postprocess {
    float m_threshold;
    int m_radius;
    float m_sigma;
    float m_intensity;

public:
    Bloom(const Properties &properties) : Postprocess(properties) {
        m_threshold = properties.get<float>("threshold", 1.f);
        m_radius = properties.get<int>("radius", 5);
        m_sigma = properties.get<float>("sigma", 3.f);
        m_intensity = properties.get<float>("intensity", 1.f);
    }

    std::vector<float> createGaussianKernel(int radius, float sigma) {
        std::vector<float> kernel(2 * radius + 1);
        float sum = 0.f;
        for (int i = -radius; i <= radius; i++) {
            float v = std::exp(-(i * i) / (2.f * sigma * sigma));
            kernel[i + radius] = v;
            sum += v;
        }
        for (auto& k : kernel) k /= sum;
        return kernel;
    }

    void execute() override {
        m_output->initialize(m_input->resolution());

        Point2i res = m_input->resolution();
        Image bright(res), blurx(res), blury(res);
        for (int x = 0; x < res.x(); x++) {
            for (int y = 0; y < res.y(); y++) {
                Point2i p{ x, y };
                const Color &c = (*m_input)(p);
                if (c.luminance() > m_threshold)
                    bright(p) = c;
            }
        }

        std::vector<float> kernel = createGaussianKernel(m_radius, m_sigma);

        // horizontal gaussian blur
        for (int x = 0; x < res.x(); x++) {
            for (int y = 0; y < res.y(); y++) {
                Color sum(0.f);
                for (int k = -m_radius; k <= m_radius; k++) {
                    int nx = std::clamp(x + k, 0, res.x() - 1);
                    sum += kernel[k + m_radius] * bright({nx, y});
                }
                blurx({x, y}) = sum;
            }
        }

        // vertical gaussian blur
        for (int x = 0; x < res.x(); x++) {
            for (int y = 0; y < res.y(); y++) {
                Color sum(0.f);
                for (int k = -m_radius; k <= m_radius; k++) {
                    int ny = std::clamp(y + k, 0, res.y() - 1);
                    sum += kernel[k + m_radius] * blurx({x, ny});
                }
                blury({x, y}) = sum;
            }
        }

        for (int x = 0; x < res.x(); x++) {
            for (int y = 0; y < res.y(); y++) {
                Point2i p{ x, y };
                (*m_output)(p) += blury(p) * m_intensity;
            }
        }

        Streaming stream{ *m_output };
        stream.update();
        m_output->save();
    }

    std::string toString() const override {
        return tfm::format(
            "Bloom[\n"
            "  input = %s,\n"
            "  output = %s,\n"
            "]",
            indent(m_input),
            indent(m_output));
    }
};

} // namespace lightwave

REGISTER_POSTPROCESS(Bloom, "bloom");
