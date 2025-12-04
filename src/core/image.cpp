#include <lightwave/core.hpp>
#include <lightwave/image.hpp>
#include <lightwave/registry.hpp>

#include <stb_image.h>
#include <tinyexr.h>

namespace lightwave {

void Image::loadImage(const std::filesystem::path &path, bool isLinearSpace) {
    const auto extension = path.extension();
    logger(EInfo, "loading image %s", path);
    if (extension == ".exr") {
        // loading of EXR files is handled by TinyEXR
        float *data;
        const char *err;
        if (LoadEXR(&data,
                    &m_resolution.x(),
                    &m_resolution.y(),
                    path.generic_string().c_str(),
                    &err)) {
            lightwave_throw("could not load image %s: %s", path, err);
        }

        m_data.resize(m_resolution.x() * m_resolution.y());
        auto it = data;
        for (auto &pixel : m_data) {
            for (int i = 0; i < pixel.NumComponents; i++)
                pixel[i] = *it++;
            it++; // skip alpha channel
        }
        free(data);
    } else {
        // anything that is not an EXR file is handled by stb
        stbi_ldr_to_hdr_gamma(isLinearSpace ? 1.0f : 2.2f);

        int numChannels;
        float *data = stbi_loadf(path.generic_string().c_str(),
                                 &m_resolution.x(),
                                 &m_resolution.y(),
                                 &numChannels,
                                 3);
        if (data == nullptr) {
            lightwave_throw(
                "could not load image %s: %s", path, stbi_failure_reason());
        }

        m_data.resize(m_resolution.x() * m_resolution.y());
        auto it = data;
        for (auto &pixel : m_data) {
            for (int i = 0; i < pixel.NumComponents; i++)
                pixel[i] = *it++;
        }
        free(data);
    }
}

void Image::saveAt(const std::filesystem::path &path, float norm) const {
    if (resolution().isZero()) {
        logger(EWarn, "cannot save empty image %s!", path);
        return;
    }

    assert_condition(Color::NumComponents == 3, {
        logger(EError,
               "the number of components in Color has changed, you need to "
               "update Image::saveAt with new channel names.");
    });

    logger(EInfo, "saving image %s", path);

    // MARK: Create metadata

    std::string log = logger.history();
    std::vector<EXRAttribute> customAttributes;
    customAttributes.emplace_back(EXRAttribute{
        .name  = "log",
        .type  = "string",
        .value = reinterpret_cast<unsigned char *>(log.data()),
        .size  = int(log.size()),
    });

    // MARK: Create EXR header

    EXRHeader header;
    InitEXRHeader(&header);

    header.custom_attributes     = customAttributes.data();
    header.num_custom_attributes = int(customAttributes.size());

    header.compression_type =
        (m_resolution.x() < 16) && (m_resolution.y() < 16)
            ? TINYEXR_COMPRESSIONTYPE_NONE /* No compression for small image. */
            : TINYEXR_COMPRESSIONTYPE_ZIP;

    header.num_channels = Color::NumComponents;
    header.channels     = static_cast<EXRChannelInfo *>(malloc(
        sizeof(EXRChannelInfo) * static_cast<size_t>(header.num_channels)));
    header.pixel_types  = static_cast<int *>(
        malloc(sizeof(int) * static_cast<size_t>(header.num_channels)));
    header.requested_pixel_types = static_cast<int *>(
        malloc(sizeof(int) * static_cast<size_t>(header.num_channels)));

    // MARK: Create EXR image

    EXRImage image;
    InitEXRImage(&image);

    float *channelPtr[Color::NumComponents];
    image.width        = m_resolution.x();
    image.height       = m_resolution.y();
    image.num_channels = header.num_channels;
    image.images       = reinterpret_cast<unsigned char **>(channelPtr);

    // MARK: Copy normalized data

    std::vector<float> channels[Color::NumComponents];
    const size_t pixelCount = static_cast<size_t>(m_resolution.x()) *
                              static_cast<size_t>(m_resolution.y());

    for (int chan = 0; chan < Color::NumComponents; chan++) {
        channels[chan].resize(pixelCount);
        for (size_t px = 0; px < pixelCount; px++) {
            channels[chan][px] = m_data[px][chan] * norm;
        }

        header.pixel_types[chan] =
            TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
        header.requested_pixel_types[chan] =
            TINYEXR_PIXELTYPE_FLOAT; // save with float(fp32) pixel format
                                     // (i.e., no precision reduction)

        // Must be BGR order, since most EXR viewers expect this channel order.
        header.channels[chan].name[0]                 = "BGR"[chan];
        header.channels[chan].name[1]                 = 0;
        channelPtr[Color::NumComponents - (chan + 1)] = channels[chan].data();
    }

    // MARK: Save EXR

    const char *error;
    int ret = SaveEXRImageToFile(&image, &header, path.generic_string().c_str(), &error);

    header.num_custom_attributes = 0;
    header.custom_attributes     = nullptr; // memory freed by std::vector
    FreeEXRHeader(&header);

    if (ret != TINYEXR_SUCCESS) {
        logger(EError, "  error saving image %s: %s", path, error);
    }
}
} // namespace lightwave

REGISTER_CLASS(Image, "image", "default")
