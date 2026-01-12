#pragma once

#include <lightwave/color.hpp>
#include <lightwave/math.hpp>

namespace lightwave {

float F_D90(float roughness, Vector wh, Vector wo) {
    return 0.5f + 2.f * roughness * sqr(wh.dot(wo));
}

float F_SS90(float roughness, Vector wh, Vector wo) {
    return roughness * sqr(wh.dot(wo));
}

float F(float f, Vector w) {
    return 1.f + (f - 1.f) * pow(1.f - Frame::absCosTheta(w), 5);
}

Color sqrt(Color c) {
    return Color(sqrt(c.r()), sqrt(c.g()), sqrt(c.b()));
}

float R0(float eta) {
    return sqr(eta - 1.f) / sqr(eta + 1.f);
}

} // namespace lightwave
