#include <lightwave.hpp>

#include "pcg32.h"
#include "primes.h"
#include <functional>

namespace lightwave {

class Halton : public Sampler {
    int m_dimension;
    int64_t m_haltonIndex;

    static constexpr int maxHaltonResolution = 128;
    static constexpr int BaseScales[2] = {128, 243}; 
    static constexpr int BaseExponents[2] = {7, 5};
    int multInverse[2];

public:
    Halton(const Properties &properties) : Sampler(properties) { }

    void seed(int sampleIndex) override { 
        m_haltonIndex = sampleIndex;
        m_dimension = 0;
    }

    void seed(const Point2i &pixel, int sampleIndex) override {
        m_haltonIndex = 0;
        int sampleStride = BaseScales[0] * BaseScales[1];
        if (sampleStride > 1) {
            Point2i pm(pixel.x() % maxHaltonResolution, pixel.y() % maxHaltonResolution);
            for (int i = 0; i < 2; i++) {
                uint64_t dimOffset = (i == 0) ? inverseRadicalInverse(pm[i], 2, BaseExponents[i])
                                              : inverseRadicalInverse(pm[i], 3, BaseExponents[i]);
                uint64_t multInv = multiplicativeInverse(sampleStride / BaseScales[i], BaseScales[i]);
                m_haltonIndex += dimOffset * (sampleStride / BaseScales[i]) * multInv;
            }
            m_haltonIndex %= sampleStride;
        }
        m_haltonIndex += sampleIndex * sampleStride;
        m_dimension = 0;
    }

    float next() override { 
        if (m_dimension == 0) {
            m_dimension++;
            return (float)radicalInverse(2, m_haltonIndex >> BaseExponents[0]);
        }
        if (m_dimension == 1) {
            m_dimension++;
            return (float)radicalInverse(3, m_haltonIndex / BaseScales[1]);
        }
        if (m_dimension >= PrimeTableSize)
            m_dimension = 2;
        return radicalInverse(Primes[m_dimension++], m_haltonIndex);
    }

    ref<Sampler> clone() const override {
        return std::make_shared<Halton>(*this);
    }

    std::string toString() const override {
        return tfm::format("Halton[\n"
                           "  count = %d\n"
                           "]",
                           m_samplesPerPixel);
    }

private:
    uint64_t multiplicativeInverse(int64_t a, int64_t n) {
        int64_t x, y;
        extendedGCD(a, n, &x, &y);
        return x % n;
    }

    void extendedGCD(uint64_t a, uint64_t b, int64_t *x, int64_t *y) {
        if (b == 0) {
            *x = 1;
            *y = 0;
            return;
        }
        int64_t d = a / b, xp, yp;
        extendedGCD(b, a % b, &xp, &yp);
        *x = yp;
        *y = xp - (d * yp);
    }

    uint64_t inverseRadicalInverse(uint64_t inverse, int base, int nDigits) {
        uint64_t index = 0;
        for (int i = 0; i < nDigits; i++) {
            uint64_t digit = inverse % base;
            inverse /= base;
            index = index * base + digit;
        }
        return index;
    }

    float radicalInverse(int baseIndex, uint64_t a) {
        int base = Primes[baseIndex];
        float invBase = 1.f / base;
        float invBaseM = 1.f;
        uint64_t reversedDigits = 0;
        while (a) {
            uint64_t next = a / base;
            uint64_t digit = a - next * base;
            reversedDigits = reversedDigits * base + digit;
            invBaseM *= invBase;
            a = next;
        }
        return std::min(reversedDigits * invBaseM, 1.f - Epsilon);
    }
};

} // namespace lightwave

REGISTER_SAMPLER(Halton, "halton")
