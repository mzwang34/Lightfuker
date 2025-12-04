#include <lightwave.hpp>

namespace lightwave {

class Tonemap : public Postprocess {

public:
    Tonemap(const Properties &properties) : Postprocess(properties) {
    }

    void execute() override {
        m_output->initialize(m_input->resolution());

        NOT_IMPLEMENTED

        Streaming stream{*m_output};
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
