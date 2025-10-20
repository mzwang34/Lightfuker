#include <lightwave/bsdf.hpp>
#include <lightwave/color.hpp>
#include <lightwave/emission.hpp>
#include <lightwave/instance.hpp>
#include <lightwave/light.hpp>
#include <lightwave/math.hpp>
#include <lightwave/profiler.hpp>
#include <lightwave/sampler.hpp>

namespace lightwave {

// based on the MESA implementation of the GLU library
std::optional<Matrix4x4> invert(const Matrix4x4 &m) {
    Matrix4x4 inv;
    // clang-format off
    inv(0, 0) = m(1, 1) * m(2, 2) * m(3, 3) - 
               m(1, 1) * m(2, 3) * m(3, 2) - 
               m(2, 1) * m(1, 2) * m(3, 3) + 
               m(2, 1) * m(1, 3) * m(3, 2) +
               m(3, 1) * m(1, 2) * m(2, 3) - 
               m(3, 1) * m(1, 3) * m(2, 2);

    inv(1, 0) = -m(1, 0) * m(2, 2) * m(3, 3) + 
                m(1, 0) * m(2, 3) * m(3, 2) + 
                m(2, 0) * m(1, 2) * m(3, 3) - 
                m(2, 0) * m(1, 3) * m(3, 2) - 
                m(3, 0) * m(1, 2) * m(2, 3) + 
                m(3, 0) * m(1, 3) * m(2, 2);

    inv(2, 0) = m(1, 0) * m(2, 1) * m(3, 3) - 
               m(1, 0) * m(2, 3) * m(3, 1) - 
               m(2, 0) * m(1, 1) * m(3, 3) + 
               m(2, 0) * m(1, 3) * m(3, 1) + 
               m(3, 0) * m(1, 1) * m(2, 3) - 
               m(3, 0) * m(1, 3) * m(2, 1);

    inv(3, 0) = -m(1, 0) * m(2, 1) * m(3, 2) + 
                 m(1, 0) * m(2, 2) * m(3, 1) +
                 m(2, 0) * m(1, 1) * m(3, 2) - 
                 m(2, 0) * m(1, 2) * m(3, 1) - 
                 m(3, 0) * m(1, 1) * m(2, 2) + 
                 m(3, 0) * m(1, 2) * m(2, 1);
    // clang-format on

    const float det = m(0, 0) * inv(0, 0) + m(0, 1) * inv(1, 0) +
                      m(0, 2) * inv(2, 0) + m(0, 3) * inv(3, 0);
    if (det == 0)
        return {};

    // clang-format off
    inv(0, 1) = -m(0, 1) * m(2, 2) * m(3, 3) + 
                m(0, 1) * m(2, 3) * m(3, 2) + 
                m(2, 1) * m(0, 2) * m(3, 3) - 
                m(2, 1) * m(0, 3) * m(3, 2) - 
                m(3, 1) * m(0, 2) * m(2, 3) + 
                m(3, 1) * m(0, 3) * m(2, 2);

    inv(1, 1) = m(0, 0) * m(2, 2) * m(3, 3) - 
               m(0, 0) * m(2, 3) * m(3, 2) - 
               m(2, 0) * m(0, 2) * m(3, 3) + 
               m(2, 0) * m(0, 3) * m(3, 2) + 
               m(3, 0) * m(0, 2) * m(2, 3) - 
               m(3, 0) * m(0, 3) * m(2, 2);

    inv(2, 1) = -m(0, 0) * m(2, 1) * m(3, 3) + 
                m(0, 0) * m(2, 3) * m(3, 1) + 
                m(2, 0) * m(0, 1) * m(3, 3) - 
                m(2, 0) * m(0, 3) * m(3, 1) - 
                m(3, 0) * m(0, 1) * m(2, 3) + 
                m(3, 0) * m(0, 3) * m(2, 1);

    inv(3, 1) = m(0, 0) * m(2, 1) * m(3, 2) - 
                m(0, 0) * m(2, 2) * m(3, 1) - 
                m(2, 0) * m(0, 1) * m(3, 2) + 
                m(2, 0) * m(0, 2) * m(3, 1) + 
                m(3, 0) * m(0, 1) * m(2, 2) - 
                m(3, 0) * m(0, 2) * m(2, 1);

    inv(0, 2) = m(0, 1) * m(1, 2) * m(3, 3) - 
               m(0, 1) * m(1, 3) * m(3, 2) - 
               m(1, 1) * m(0, 2) * m(3, 3) + 
               m(1, 1) * m(0, 3) * m(3, 2) + 
               m(3, 1) * m(0, 2) * m(1, 3) - 
               m(3, 1) * m(0, 3) * m(1, 2);

    inv(1, 2) = -m(0, 0) * m(1, 2) * m(3, 3) + 
                m(0, 0) * m(1, 3) * m(3, 2) + 
                m(1, 0) * m(0, 2) * m(3, 3) - 
                m(1, 0) * m(0, 3) * m(3, 2) - 
                m(3, 0) * m(0, 2) * m(1, 3) + 
                m(3, 0) * m(0, 3) * m(1, 2);

    inv(2, 2) = m(0, 0) * m(1, 1) * m(3, 3) - 
                m(0, 0) * m(1, 3) * m(3, 1) - 
                m(1, 0) * m(0, 1) * m(3, 3) + 
                m(1, 0) * m(0, 3) * m(3, 1) + 
                m(3, 0) * m(0, 1) * m(1, 3) - 
                m(3, 0) * m(0, 3) * m(1, 1);

    inv(3, 2) = -m(0, 0) * m(1, 1) * m(3, 2) + 
                 m(0, 0) * m(1, 2) * m(3, 1) + 
                 m(1, 0) * m(0, 1) * m(3, 2) - 
                 m(1, 0) * m(0, 2) * m(3, 1) - 
                 m(3, 0) * m(0, 1) * m(1, 2) + 
                 m(3, 0) * m(0, 2) * m(1, 1);

    inv(0, 3) = -m(0, 1) * m(1, 2) * m(2, 3) + 
                m(0, 1) * m(1, 3) * m(2, 2) + 
                m(1, 1) * m(0, 2) * m(2, 3) - 
                m(1, 1) * m(0, 3) * m(2, 2) - 
                m(2, 1) * m(0, 2) * m(1, 3) + 
                m(2, 1) * m(0, 3) * m(1, 2);

    inv(1, 3) = m(0, 0) * m(1, 2) * m(2, 3) - 
               m(0, 0) * m(1, 3) * m(2, 2) - 
               m(1, 0) * m(0, 2) * m(2, 3) + 
               m(1, 0) * m(0, 3) * m(2, 2) + 
               m(2, 0) * m(0, 2) * m(1, 3) - 
               m(2, 0) * m(0, 3) * m(1, 2);

    inv(2, 3) = -m(0, 0) * m(1, 1) * m(2, 3) + 
                 m(0, 0) * m(1, 3) * m(2, 1) + 
                 m(1, 0) * m(0, 1) * m(2, 3) - 
                 m(1, 0) * m(0, 3) * m(2, 1) - 
                 m(2, 0) * m(0, 1) * m(1, 3) + 
                 m(2, 0) * m(0, 3) * m(1, 1);

    inv(3, 3) = m(0, 0) * m(1, 1) * m(2, 2) - 
                m(0, 0) * m(1, 2) * m(2, 1) - 
                m(1, 0) * m(0, 1) * m(2, 2) + 
                m(1, 0) * m(0, 2) * m(2, 1) + 
                m(2, 0) * m(0, 1) * m(1, 2) - 
                m(2, 0) * m(0, 2) * m(1, 1);
    // clang-format on

    return inv * (1.f / det);
}

void buildOrthonormalBasis(const Vector &a, Vector &b, Vector &c) {
    if (abs(a.x()) > abs(a.y())) {
        auto invLen = 1 / sqrt(a.x() * a.x() + a.z() * a.z());
        c           = { a.z() * invLen, 0, -a.x() * invLen };
    } else {
        auto invLen = 1 / sqrt(a.y() * a.y() + a.z() * a.z());
        c           = { 0, a.z() * invLen, -a.y() * invLen };
    }
    b = c.cross(a);
}

EmissionEval Intersection::evaluateEmission() const {
    if (!instance) {
        if (background) {
            // nothing was hit, but a background light is available
            auto E = background->evaluate(-wo);
            return E;
        }

        // nothing was hit, no background light available
        return EmissionEval::invalid();
    }

    if (!instance->emission()) {
        // something was hit, but has no emission
        return EmissionEval::invalid();
    }

    // something was hit and has an emission
    auto E = instance->emission()->evaluate(uv, shadingFrame().toLocal(wo));
    return {
        .value = E.value,
    };
}

BsdfSample Intersection::sampleBsdf(Sampler &rng) const {
    PROFILE("Sample Bsdf")

    if (!instance || !instance->bsdf())
        return BsdfSample::invalid();
    assert_normalized(wo, {});
    auto bsdfSample =
        instance->bsdf()->sample(uv, shadingFrame().toLocal(wo), rng);
    if (bsdfSample.isInvalid())
        return bsdfSample;
    assert_normalized(bsdfSample.wi, {
        logger(EError, "offending BSDF: %s", instance->bsdf()->toString());
        logger(EError, "  input was: %s with length %f", wo, wo.length());
    });
    bsdfSample.wi = shadingFrame().toWorld(bsdfSample.wi);
    assert_normalized(bsdfSample.wi, {});
    return bsdfSample;
}

Light *Intersection::light() const {
    if (!instance)
        return background;
    return instance->light();
}

void Frame::validate() const {
    assert_normalized(normal, {});
    assert_normalized(tangent, {});
    assert_normalized(bitangent, {});
    assert_orthogonal(normal, tangent, {});
    assert_orthogonal(normal, bitangent, {});
    assert_orthogonal(tangent, bitangent, {});
}

Color Color::fromTemperature(float t) {
    // From Blender Cycles
    // (https://github.com/blender/cycles/blob/main/src/kernel/svm/math_util.h)
    // Calculate color in range 800..12000 using an approximation
    // a/x+bx+c for R and G and ((at + b)t + c)t + d) for B.
    // The result of this can be negative to support gamut wider than
    // than rec.709, just needs to be clamped.

    // clang-format off
    static const float table_r[][3] = {
        {1.61919106e+03f, -2.05010916e-03f, 5.02995757e+00f},
        {2.48845471e+03f, -1.11330907e-03f, 3.22621544e+00f},
        {3.34143193e+03f, -4.86551192e-04f, 1.76486769e+00f},
        {4.09461742e+03f, -1.27446582e-04f, 7.25731635e-01f},
        {4.67028036e+03f,  2.91258199e-05f, 1.26703442e-01f},
        {4.59509185e+03f,  2.87495649e-05f, 1.50345020e-01f},
        {3.78717450e+03f,  9.35907826e-06f, 3.99075871e-01f}
    };

    static const float table_g[][3] = {
        {-4.88999748e+02f,  6.04330754e-04f, -7.55807526e-02f},
        {-7.55994277e+02f,  3.16730098e-04f,  4.78306139e-01f},
        {-1.02363977e+03f,  1.20223470e-04f,  9.36662319e-01f},
        {-1.26571316e+03f,  4.87340896e-06f,  1.27054498e+00f},
        {-1.42529332e+03f, -4.01150431e-05f,  1.43972784e+00f},
        {-1.17554822e+03f, -2.16378048e-05f,  1.30408023e+00f},
        {-5.00799571e+02f, -4.59832026e-06f,  1.09098763e+00f}
    };

    static const float table_b[][4] = {
        { 5.96945309e-11f, -4.85742887e-08f, -9.70622247e-05f, -4.07936148e-03f},
        { 2.40430366e-11f,  5.55021075e-08f, -1.98503712e-04f,  2.89312858e-02f},
        {-1.40949732e-11f,  1.89878968e-07f, -3.56632824e-04f,  9.10767778e-02f},
        {-3.61460868e-11f,  2.84822009e-07f, -4.93211319e-04f,  1.56723440e-01f},
        {-1.97075738e-11f,  1.75359352e-07f, -2.50542825e-04f, -2.22783266e-02f},
        {-1.61997957e-13f, -1.64216008e-08f,  3.86216271e-04f, -7.38077418e-01f},
        { 6.72650283e-13f, -2.73078809e-08f,  4.24098264e-04f, -7.52335691e-01f}
    };
    // clang-format on

    if (t >= 12000.0f)
        return Color(
            0.8262954810464208f, 0.9945080501520986f, 1.566307710274283f);
    else if (t < 800.0f)
        return Color(
            5.413294490189271f, -0.20319390035873933f, -0.0822535242887164f);

    const int i = (t >= 6365.0f)   ? 6
                  : (t >= 3315.0f) ? 5
                  : (t >= 1902.0f) ? 4
                  : (t >= 1449.0f) ? 3
                  : (t >= 1167.0f) ? 2
                  : (t >= 965.0f)  ? 1
                                   : 0;

    const float *r = table_r[i];
    const float *g = table_g[i];
    const float *b = table_b[i];

    const float t_inv = 1.0f / t;
    return Color(r[0] * t_inv + r[1] * t + r[2],
                 g[0] * t_inv + g[1] * t + g[2],
                 ((b[0] * t + b[1]) * t + b[2]) * t + b[3]);
}

} // namespace lightwave
