#include <lightwave.hpp>

namespace lightwave {

ImageTexture::ImageTexture(const Properties &properties) {
    if (properties.has("filename")) {
        m_image = std::make_shared<Image>(properties);
    } else {
        m_image = properties.getChild<Image>();
    }
    m_exposure = properties.get<float>("exposure", 1);

    // clang-format off
    m_border = properties.getEnum<BorderMode>("border", BorderMode::Repeat, {
        { "clamp", BorderMode::Clamp },
        { "repeat", BorderMode::Repeat },
    });

    m_filter = properties.getEnum<FilterMode>("filter", FilterMode::Bilinear, {
        { "nearest", FilterMode::Nearest },
        { "bilinear", FilterMode::Bilinear },
    });
    // clang-format on
}

Color ImageTexture::evaluate(const Point2 &uv) const {
    int w = m_image->resolution().x();
    int h = m_image->resolution().y();
    float x = uv.x() * w - 0.5f;
    float y = (1.f - uv.y()) * h - 0.5f;
    Color c(0.f);

    // border handling
    auto getPixelWithBorder = [&](int x, int y) {
        if (m_border == BorderMode::Clamp) {
            x = min(max(x, 0), w - 1);
            y = min(max(y, 0), h - 1);
        } else if (m_border == BorderMode::Repeat) {
            x = (x % w + w) % w; // avoid negative
            y = (y % h + h) % h;
        }
        return Point2i(x, y);
    };

    // filtering
    if (m_filter == FilterMode::Nearest) {
        int x0 = floor(x + 0.5f);
        int y0 = floor(y + 0.5f);
        c = m_image->get(getPixelWithBorder(x0, y0));
    } else if (m_filter == FilterMode::Bilinear) {
        Color t00 = m_image->get(getPixelWithBorder(floor(x), floor(y)));
        Color t10 = m_image->get(getPixelWithBorder(ceil(x), floor(y)));
        Color t01 = m_image->get(getPixelWithBorder(floor(x), ceil(y)));
        Color t11 = m_image->get(getPixelWithBorder(ceil(x), ceil(y)));
        float tx = x - floor(x);
        float ty = y - floor(y);
        Color t0 = tx * t10 + (1.f - tx) * t00;
        Color t1 = tx * t11 + (1.f - tx) * t01;
        c = ty * t1 + (1.f - ty) * t0;
    }

    return c * m_exposure;
}

std::string ImageTexture::toString() const {
    return tfm::format(
        "ImageTexture[\n"
        "  image = %s,\n"
        "  exposure = %f,\n"
        "]",
        indent(m_image),
        m_exposure);
}


} // namespace lightwave

REGISTER_TEXTURE(ImageTexture, "image")
