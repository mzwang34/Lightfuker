#pragma once

#include <lightwave/color.hpp>
#include <lightwave/math.hpp>

namespace lightwave {

Vector2 sqr(Vector2 x) { return x * x; }

void fresnelDielectric(float ct1, float n1, float n2, Vector2 &R, Vector2 &phi) {
    float st1 = (1.f - ct1 * ct1);
    float nr = n1 / n2;

    if (sqr(nr) * st1 > 1) { // total reflection
        Vector2 R(1.f);
        Vector2 var = Vector2(-sqr(nr) * sqrt(st1 - 1.f / sqr(nr)) / ct1, -sqrt(st1 - 1.f / sqr(nr)) / ct1);
        phi = Vector2(2.f * std::atan(var.x()), 2.f * std::atan(var.y()));
    } else {
        float ct2 = sqrt(1.f - sqr(nr) * st1);
        Vector2 r(
            (n2 * ct1 - n1 * ct2) / (n2 * ct1 + n1 * ct2),
            (n1 * ct1 - n2 * ct2) / (n1 * ct1 + n2 * ct2) 
        );
        phi.x() = (r.x() < 0.f) ? Pi : 0.f;
        phi.y() = (r.y() < 0.f) ? Pi : 0.f;
        R = sqr(r);
    }
}

void fresnelConductor(float ct1, float n1, float n2, float k, Vector2 &R, Vector2 &phi) {
    if (k == 0.0f) {
        fresnelDielectric(ct1, n1, n2, R, phi);
        return;
    }

    float A = sqr(n2) * (1.0f - sqr(k)) - sqr(n1) * (1.0f - sqr(ct1));
    float B = std::sqrt(sqr(A) + sqr(2.0f * sqr(n2) * k));
    float U = std::sqrt((A + B) / 2.0f);
    float V = std::sqrt((B - A) / 2.0f);

    R.y() = (sqr(n1 * ct1 - U) + sqr(V)) / (sqr(n1 * ct1 + U) + sqr(V));
    
    Vector2 var1(2.0f * n1 * V * ct1, sqr(U) + sqr(V) - sqr(n1 * ct1));
    phi.y() = std::atan2(var1.x(), var1.y()) + Pi;
    
    R.x() = (sqr(sqr(n2) * (1.0f - sqr(k)) * ct1 - n1 * U) + sqr(2.0f * sqr(n2) * k * ct1 - n1 * V)) /
            (sqr(sqr(n2) * (1.0f - sqr(k)) * ct1 + n1 * U) + sqr(2.0f * sqr(n2) * k * ct1 + n1 * V));
    
    Vector2 var2(
        2.0f * n1 * sqr(n2) * ct1 * (2.0f * k * U - (1.0f - sqr(k)) * V),
        sqr(sqr(n2) * (1.0f + sqr(k)) * ct1) - sqr(n1) * (sqr(U) + sqr(V))
    );
    phi.x() = std::atan2(var2.x(), var2.y());
}

Vector sqrtVec(const Vector& v) {
    return Vector(std::sqrt(v.x()), std::sqrt(v.y()), std::sqrt(v.z()));
}

Vector cosVec(const Vector& v) {
    return Vector(std::cos(v.x()), std::cos(v.y()), std::cos(v.z()));
}

Vector expVec(const Vector& v) {
    return Vector(std::exp(v.x()), std::exp(v.y()), std::exp(v.z()));
}

Vector multVec(const Vector& a, const Vector& b) {
    return Vector(a.x() * b.x(), a.y() * b.y(), a.z() * b.z());
}

Vector evalSensitivity(float opd, float shift) {
    float phase = 2.f * Pi * opd * 1e-6;
    Vector val = Vector(5.4856e-13, 4.4201e-13, 5.2481e-13);
	Vector pos = Vector(1.6810e+06, 1.7953e+06, 2.2084e+06);
	Vector var = Vector(4.3278e+09, 9.3046e+09, 6.6121e+09);
    Vector xyz = multVec(val, multVec(sqrtVec(2.f * Pi * var), multVec(cosVec(pos * phase + Vector(shift)), expVec(-var * phase * phase))));
    xyz.x() += 9.7470e-14 * sqrt(2.0 * Pi * 4.5282e+09) * cos(2.2399e+06 * phase + shift) * exp(-4.5282e+09 * phase * phase);
    return xyz / 1.0685e-7;
}

Color xyzToRgb(const Vector &xyz) {
    float r =  2.3706743f * xyz.x() - 0.9000405f * xyz.y() - 0.4706338f * xyz.z();
    float g = -0.5138850f * xyz.x() + 1.4253036f * xyz.y() + 0.0885814f * xyz.z();
    float b =  0.0052982f * xyz.x() - 0.0146949f * xyz.y() + 1.0093968f * xyz.z();
    return Color(std::max(0.f, r), std::max(0.f, g), std::max(0.f, b));
}

float smoothstep(float edge0, float edge1, float x) {
    float t = (x - edge0) / (edge1 - edge0);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    
    return t * t * (3.0f - 2.0f * t);
}

template <typename T>
T lerp(T a, T b, float t) {
    return a + (b - a) * t;
}

} // namespace lightwave