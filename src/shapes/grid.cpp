#include <lightwave.hpp>
#include <fstream>
#include <cstdint>

namespace lightwave {

class GridVolume : public Shape {
    float m_multiplier;     
    float m_sigma_t;
    Vector3i m_resolution;
    std::vector<float> m_density;
    float m_maxDensity;
    float invMaxDensity;
    Bounds m_bounds;
    Vector m_invExtent;

public:
    // GridVolume(const Properties &properties) {
    //     m_multiplier = properties.get<float>("multiplier", 1.0f);
    //     m_sigma_t = properties.get<float>("sigma_t", 1.f);

    //     auto path = properties.get<std::filesystem::path>("filename");
    //     std::ifstream input(path, std::ios::binary);
    //     if (!input.is_open()) {
    //         lightwave_throw("Error opening volume file: %s", path.string());
    //     }

    //     float fx, fy, fz;
    //     input.read(reinterpret_cast<char*>(&fx), sizeof(float));
    //     input.read(reinterpret_cast<char*>(&fy), sizeof(float));
    //     input.read(reinterpret_cast<char*>(&fz), sizeof(float));
    //     m_resolution = Vector3i((int)fx, (int)fy, (int)fz);

    //     int voxelCount = m_resolution.product();
    //     m_density.resize(voxelCount);
    //     m_maxDensity = 0;
    //     for (int i = 0; i < voxelCount; i++) {
    //         float v;
    //         input.read(reinterpret_cast<char*>(&v), sizeof(float));
    //         m_density[i] = v * m_multiplier;
    //         m_maxDensity = std::max(m_maxDensity, m_density[i]);
    //     }
    //     invMaxDensity = 1.f / m_maxDensity;
    // }

    GridVolume(const Properties &properties) {
        m_multiplier = properties.get<float>("multiplier", 1.0f);
        m_sigma_t    = properties.get<float>("sigma_t", 1.f);

        auto path = properties.get<std::filesystem::path>("filename");
        std::ifstream input(path, std::ios::binary);
        if (!input.is_open()) {
            lightwave_throw("Error opening volume file: %s", path.string());
        }

        // ------------------------------------------------------------
        // 1. Read magic
        // ------------------------------------------------------------
        char magic[3];
        input.read(magic, 3);
        if (magic[0] != 'V' || magic[1] != 'O' || magic[2] != 'L') {
            lightwave_throw("Not a Mitsuba VOL file: %s", path.string());
        }

        // ------------------------------------------------------------
        // 2. Read header
        // ------------------------------------------------------------
        uint8_t version;
        input.read(reinterpret_cast<char*>(&version), sizeof(uint8_t));
        if (version != 3) {
            lightwave_throw("Unsupported VOL version %d", version);
        }

        int32_t type;
        int32_t dimX, dimY, dimZ;
        int32_t channels;

        input.read(reinterpret_cast<char*>(&type),     sizeof(int32_t));
        input.read(reinterpret_cast<char*>(&dimX),     sizeof(int32_t));
        input.read(reinterpret_cast<char*>(&dimY),     sizeof(int32_t));
        input.read(reinterpret_cast<char*>(&dimZ),     sizeof(int32_t));
        input.read(reinterpret_cast<char*>(&channels), sizeof(int32_t));

        if (type != 1) {
            lightwave_throw("VOL file is not float32 (type=%d)", type);
        }
        if (channels != 1) {
            lightwave_throw("Only single-channel volumes supported");
        }

        m_resolution = Vector3i(dimX, dimY, dimZ);

        // ------------------------------------------------------------
        // 3. Skip bounding box (6 floats)
        // ------------------------------------------------------------
        float bbox[6];
        input.read(reinterpret_cast<char*>(bbox), sizeof(float) * 6);
        
        Point pMin(bbox[0], bbox[1], bbox[2]);
        Point pMax(bbox[3], bbox[4], bbox[5]);
        m_bounds = Bounds(pMin, pMax);
        m_invExtent = Vector(
            1.f / (pMax.x() - pMin.x()),
            1.f / (pMax.y() - pMin.y()),
            1.f / (pMax.z() - pMin.z())
        );

        // ------------------------------------------------------------
        // 4. Read voxel data
        // ------------------------------------------------------------
        int voxelCount = dimX * dimY * dimZ;
        m_density.resize(voxelCount);

        m_maxDensity = 0.f;
        for (int i = 0; i < voxelCount; ++i) {
            float v;
            input.read(reinterpret_cast<char*>(&v), sizeof(float));
            v *= m_multiplier;
            m_density[i] = v;
            m_maxDensity = std::max(m_maxDensity, v);
        }

        if (m_maxDensity > 0.f) {
            invMaxDensity = 1.f / m_maxDensity;
        } else {
            invMaxDensity = 0.f;
        }
    }

    bool intersect(const Ray &ray, Intersection &its,
                   Sampler &rng) const override {
        // const Bounds bound{Point{0.f}, Point{1.f}};
        float t_min, t_max;
        // if (!bound.intersectP(ray, its.t, &t_min, &t_max))
        if (!m_bounds.intersectP(ray, its.t, &t_min, &t_max))
            return false;

        float t = t_min;
        while (true) {
            t -= std::log(1.f - rng.next()) * invMaxDensity / m_sigma_t;
            if (t >= t_max) break;
            if (getDensity(ray(t)) * invMaxDensity > rng.next()) {
                its.t = t;
                its.position = ray(t);
                its.shadingNormal = -ray.direction; 
                its.geometryNormal = -ray.direction;
                its.tangent = Frame(its.shadingNormal).tangent;
                return true;
            }
        }
        return false;
    }

    float transmittance(const Ray &ray, float tMax,
                        Sampler &rng) const override {
        // const Bounds bound{Point{0.f}, Point{1.f}};
        float t_min, t_max;
        // if (!bound.intersectP(ray, tMax, &t_min, &t_max))
        if (!m_bounds.intersectP(ray, tMax, &t_min, &t_max))
            return 1.f;

        float Tr = 1.f, t = t_min;
        while (true) {
            t -= std::log(1.f - rng.next()) * invMaxDensity / m_sigma_t;
            if (t >= t_max) break;
            float density = getDensity(ray(t));
            Tr *= 1.f - std::max(0.f, density * invMaxDensity);

            if (Tr < 0.1f) {
                float q = std::max(0.05f, 1.f - Tr);
                if (rng.next() < q) return 0;
                Tr /= 1 - q;
            }
        }
        return std::clamp(Tr, 0.f, 1.f);
    }

    Bounds getBoundingBox() const override {
        // return Bounds::full();
        return m_bounds;
    }

    Point getCentroid() const override {
        return Point{ 0.f };
    }

    std::string toString() const override { return "GridVolume[]"; }

private:
    float lerp(float t, float v1, float v2) const {
        return (1 - t) * v1 + t * v2;
    }

    bool insideExclusive(const Pointi &p, const Boundsi &b) const {
        return (p.x() >= b.min().x() && p.x() < b.max().x() &&
                p.y() >= b.min().y() && p.y() < b.max().y() &&
                p.z() >= b.min().z() && p.z() < b.max().z());
    }

    float D(const Pointi &p) const {
        Boundsi sampleBounds(Pointi{0}, Pointi(m_resolution));
        if (!insideExclusive(p, sampleBounds)) return 0;
        return m_density[(p.z() * m_resolution.y() + p.y()) * m_resolution.x() + p.x()];
    }

    Point Normalize (const Point &p) const {
        Point pOffset(p.x() - m_bounds.min().x(), p.y() - m_bounds.min().y(), p.z() - m_bounds.min().z());
        return Point(pOffset.x() * m_invExtent.x(), pOffset.y() * m_invExtent.y(), pOffset.z() * m_invExtent.z());
    }
    float getDensity(const Point &p) const {
        // Point pSamples(p.x() * m_resolution.x() - 0.5f, p.y() * m_resolution.y() - 0.5f, p.z() * m_resolution.z() - 0.5f);
        Point pNormalized(Normalize(p));
        Point pSamples(pNormalized.x()  * m_resolution.x() - 0.5f, pNormalized.y()  * m_resolution.y() - 0.5f, pNormalized.z()  * m_resolution.z() - 0.5f);
        Pointi pi((int)floor(pSamples.x()), (int)floor(pSamples.y()), (int)floor(pSamples.z()));
        Vector d = pSamples - Point((float)pi.x(), (float)pi.y(), (float)pi.z());

        float d00 = lerp(d.x(), D(pi), D(pi + Vector3i(1, 0, 0)));
        float d10 = lerp(d.x(), D(pi + Vector3i(0, 1, 0)), D(pi + Vector3i(1, 1, 0)));
        float d01 = lerp(d.x(), D(pi + Vector3i(0, 0, 1)), D(pi + Vector3i(1, 0, 1)));
        float d11 = lerp(d.x(), D(pi + Vector3i(0, 1, 1)), D(pi + Vector3i(1, 1, 1)));
        float d0 = lerp(d.y(), d00, d10);
        float d1 = lerp(d.y(), d01, d11);

        return lerp(d.z(), d0, d1);
    }
};

} // namespace lightwave

REGISTER_SHAPE(GridVolume, "grid")