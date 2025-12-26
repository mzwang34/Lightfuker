#include <lightwave.hpp>

namespace lightwave {

class Volume : public Shape {
    float m_density;
    ref<Shape> m_boundary;

public:
    Volume(const Properties &properties) {
        m_density  = properties.get<float>("density");
        m_boundary = properties.getOptionalChild<Shape>();
    }

    bool intersect(const Ray &ray, Intersection &its,
                   Sampler &rng) const override {
        if (m_boundary) {
            Intersection its_entry;
            if (!m_boundary->intersect(ray, its_entry, rng))
                return false;

            bool is_outside = ray.direction.dot(its_entry.shadingNormal) < 0.f;
            float t_entry, t_exit;

            if (is_outside) {
                t_entry = its_entry.t;

                Ray ray_inside(ray(t_entry), ray.direction);
                Intersection its_exit;
                if (m_boundary->intersect(ray_inside, its_exit, rng)) {
                    t_exit = t_entry + its_exit.t;
                } else {
                    return false;
                }
            } else {
                t_entry = 0;
                t_exit  = its_entry.t;
            }
            float t        = Epsilon - log(1.f - rng.next()) / m_density;
            float t_target = t_entry + t;
            if (t_target >= t_exit) {
                return false;
            }
            if (t_target < its.t) {
                its.t              = t_target;
                its.position       = ray(t_target);
                its.shadingNormal  = -ray.direction;
                its.geometryNormal = -ray.direction;
                its.tangent        = Frame(its.shadingNormal).tangent;
                return true;
            }

        } else {
            float t = Epsilon - log(1.f - rng.next()) / m_density;
            if (t < its.t) {
                its.t              = t;
                its.position       = ray(t);
                its.shadingNormal  = -ray.direction;
                its.geometryNormal = -ray.direction;
                its.tangent        = Frame(its.shadingNormal).tangent;
                return true;
            }
        }
        return false;
    }

    float transmittance(const Ray &ray, float tMax,
                        Sampler &rng) const override {
        if (m_boundary) {
            Intersection its_entry;
            if (!m_boundary->intersect(ray, its_entry, rng))
                return 1.f;

            bool is_outside = ray.direction.dot(its_entry.shadingNormal) < 0.f;
            float t_entry, t_exit;

            if (is_outside) {
                t_entry = its_entry.t;

                Ray ray_inside(ray(t_entry), ray.direction);
                Intersection its_exit;
                if (m_boundary->intersect(ray_inside, its_exit, rng)) {
                    t_exit = t_entry + its_exit.t;
                } else {
                    return 1.f;
                }
            } else {
                t_entry = 0.f;
                t_exit  = its_entry.t;
            }
            if (tMax <= t_entry)
                return 1.f;

            if (t_exit >= tMax)
                t_exit = tMax;

            return exp(-m_density * (t_exit - t_entry));

        } else {
            return exp(-m_density * tMax);
        }
    }

    Bounds getBoundingBox() const override {
        return m_boundary ? m_boundary->getBoundingBox() : Bounds::full();
    }

    Point getCentroid() const override {
        return m_boundary ? m_boundary->getCentroid() : Point{ 0.f };
    }

    std::string toString() const override { return "Volume[]"; }
};

} // namespace lightwave

REGISTER_SHAPE(Volume, "volume")
