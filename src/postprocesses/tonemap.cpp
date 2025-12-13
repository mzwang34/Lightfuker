#include <lightwave.hpp>

namespace lightwave {

class Tonemap : public Postprocess {

public:
    Tonemap(const Properties &properties) : Postprocess(properties) {}

    void execute() override {
        m_output->initialize(m_input->resolution());

        // NOT_IMPLEMENTED
        Point2i res = m_input->resolution();
        for (int x = 0; x < res.x(); x++) {
            for (int y = 0; y < res.y(); y++) {
                Point2i p{ x, y };
                const Color &c = (*m_input)(p);

                Color out(c.r() / (1.0f + c.r()),
                          c.g() / (1.0f + c.g()),
                          c.b() / (1.0f + c.b()));

                (*m_output)(p) = out;
            }
        }

        Streaming stream{ *m_output };
        stream.update();
        m_output->save();
    }

    std::string toString() const override {
        return tfm::format(
            "Tonemap[\n"
            "  input = %s,\n"
            "  output = %s,\n"
            "]",
            indent(m_input),
            indent(m_output));
    }
};

} // namespace lightwave

REGISTER_POSTPROCESS(Tonemap, "tonemap");
