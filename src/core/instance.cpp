#include <lightwave/core.hpp>
#include <lightwave/instance.hpp>
#include <lightwave/registry.hpp>
#include <lightwave/sampler.hpp>

namespace lightwave {

void Instance::transformFrame(SurfaceEvent &surf, const Vector &wo) const {
    surf.position = m_transform->apply(surf.position);

    surf.geometryNormal =
        m_transform->applyNormal(surf.geometryNormal).normalized();
    surf.tangent = m_transform->apply(surf.tangent).normalized();
    if (m_normal) {
        Vector local_normal = (m_normal->evaluate(surf.uv)).data();
        Vector normal =
            surf.shadingFrame().toWorld(local_normal * 2.f - Vector(1.f));
        surf.shadingNormal = m_transform->applyNormal(normal).normalized();
        surf.tangent = surf.shadingFrame().tangent.normalized();
    } else {
        surf.shadingNormal =
            m_transform->applyNormal(surf.shadingNormal).normalized();
    }

    float scale = m_transform->apply(Vector(1, 0, 0)).length();
    surf.pdf /= (scale * scale); // uniform scale only
}

inline void validateIntersection(const Intersection &its) {
    // use the following macros to make debugginer easier:
    // * assert_condition(condition, { ... });
    // * assert_normalized(vector, { ... });
    // * assert_ortoghonal(vec1, vec2, { ... });
    // * assert_finite(value or vector or color, { ... });

    // each assert statement takes a block of code to execute when it fails
    // (useful for printing out variables to narrow done what failed)

    assert_finite(its.t, {
        logger(
            EError,
            "  your intersection produced a non-finite intersection distance");
        logger(EError, "  offending shape: %s", its.instance->shape());
    });
    assert_condition(its.t >= Epsilon, {
        logger(EError,
               "  your intersection is susceptible to self-intersections");
        logger(EError, "  offending shape: %s", its.instance->shape());
        logger(EError,
               "  returned t: %.3g (smaller than Epsilon = %.3g)",
               its.t,
               Epsilon);
    });
}

bool Instance::hasAlpha(Intersection &its, Sampler &rng) const {
    if (!m_alpha) return false;
    float alpha = m_alpha->scalar(its.uv);
    if (alpha < rng.next()) return true;
    return false;
}

bool Instance::intersect(const Ray &worldRay, Intersection &its,
                         Sampler &rng) const {
    if (!m_transform) {
        // fast path, if no transform is needed
        const Ray localRay        = worldRay;
        const bool wasIntersected = m_shape->intersect(localRay, its, rng);
        if (wasIntersected) {
            if (hasAlpha(its, rng)) return false;
            its.instance = this;
            validateIntersection(its);
        }
        return wasIntersected;
    }

    const float previousT = its.t;
    Ray localRay;

    // hints:
    // * transform the ray (do not forget to normalize!)
    // * how does its.t need to change?
    localRay = m_transform->inverse(worldRay);
    float len = localRay.direction.length();
    localRay  = localRay.normalized();
    its.t *= len;

    const bool wasIntersected = m_shape->intersect(localRay, its, rng);
    if (wasIntersected) {
        if (hasAlpha(its, rng)) {
            its.t = previousT;
            return false;
        }
        its.instance = this;
        validateIntersection(its);
        // hint: how does its.t need to change?

        its.t /= len;
        transformFrame(its, -localRay.direction);
    } else {
        its.t = previousT;
    }

    return wasIntersected;
}

float Instance::transmittance(const Ray &worldRay, float tMax,
                              Sampler &rng) const {
    if (!m_transform) {
        return m_shape->transmittance(worldRay, tMax, rng);
    }

    Ray localRay = m_transform->inverse(worldRay);

    const float dLength = localRay.direction.length();
    if (dLength == 0)
        return 0;
    localRay.direction /= dLength;
    tMax *= dLength;

    return m_shape->transmittance(localRay, tMax, rng);
}

Bounds Instance::getBoundingBox() const {
    if (!m_transform) {
        // fast path
        return m_shape->getBoundingBox();
    }

    const Bounds untransformedAABB = m_shape->getBoundingBox();
    if (untransformedAABB.isUnbounded()) {
        return Bounds::full();
    }

    Bounds result;
    for (int point = 0; point < 8; point++) {
        Point p = untransformedAABB.min();
        for (int dim = 0; dim < p.Dimension; dim++) {
            if ((point >> dim) & 1) {
                p[dim] = untransformedAABB.max()[dim];
            }
        }
        p = m_transform->apply(p);
        result.extend(p);
    }
    return result;
}

Point Instance::getCentroid() const {
    if (!m_transform) {
        // fast path
        return m_shape->getCentroid();
    }

    return m_transform->apply(m_shape->getCentroid());
}

AreaSample Instance::sampleArea(Sampler &rng) const {
    AreaSample sample = m_shape->sampleArea(rng);
    transformFrame(sample, Vector());
    return sample;
}

AreaSample Instance::sampleArea(const Point& origin, Sampler &rng) const {
    Point localOrigin = m_transform->inverse(origin);
    AreaSample sample = m_shape->sampleArea(localOrigin, rng);
    transformFrame(sample, Vector());
    return sample;
}

} // namespace lightwave

REGISTER_CLASS(Instance, "instance", "default")
