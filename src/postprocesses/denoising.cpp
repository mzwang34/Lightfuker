#include <lightwave.hpp>
#include <OpenImageDenoise/oidn.hpp>

namespace lightwave {

class Denoising : public Postprocess {
    ref<Image> m_normal;
    ref<Image> m_albedo;

public:
    Denoising(const Properties &properties) : Postprocess(properties) {
        m_normal = properties.get<Image>("normal");
        m_albedo = properties.get<Image>("albedo");
    }

    void execute() override {
        Point2i res = m_input->resolution();
        m_output->initialize(res);
        int width = res.x();
        int height = res.y();

        oidn::DeviceRef device = oidn::newDevice(oidn::DeviceType::CPU); 
        device.commit();

        oidn::FilterRef filter = device.newFilter("RT");
        filter.setImage("color",  m_input->data(),  oidn::Format::Float3, width, height); 
        filter.setImage("albedo", m_albedo->data(), oidn::Format::Float3, width, height); 
        filter.setImage("normal", m_normal->data(), oidn::Format::Float3, width, height); 
        filter.setImage("output", m_output->data(),  oidn::Format::Float3, width, height); 
        filter.set("hdr", true); 
        filter.commit();
        filter.execute();
        const char* errorMessage;
        if (device.getError(errorMessage) != oidn::Error::None)
            std::cout << "Error: " << errorMessage << std::endl;

        Streaming stream{ *m_output };
        stream.update();
        m_output->save();
    }

    std::string toString() const override {
        return tfm::format(
            "Denoising[\n"
            "  input = %s,\n"
            "  output = %s,\n"
            "]",
            indent(m_input),
            indent(m_output));
    }
};

} // namespace lightwave

REGISTER_POSTPROCESS(Denoising, "denoising");
