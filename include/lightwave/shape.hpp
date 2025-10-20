/**
 * @file shape.hpp
 * @brief Contains the Shape interface used to represent geometry, as well as
 * related structures.
 */

#pragma once

#include <lightwave/bsdf.hpp>
#include <lightwave/color.hpp>
#include <lightwave/core.hpp>
#include <lightwave/emission.hpp>
#include <lightwave/math.hpp>
#include <lightwave/properties.hpp>
#include <lightwave/shape.hpp>
#include <lightwave/texture.hpp>
#include <lightwave/transform.hpp>

namespace lightwave {

/// @brief The result of sampling a random point on a shape's surface via @ref
/// Shape::sampleArea .
struct AreaSample : public SurfaceEvent {
    /// @brief Creates an area sample with zero pdf to report that sampling has
    /// failed.
    static AreaSample invalid() {
        AreaSample result;
        result.pdf = 0;
        return result;
    }
};

/// @brief A shape represents a geometrical object that can be intersected by
/// rays.
class Shape : public Object {
public:
    /**
     * @brief Tests the shape for intersection with a ray, and on success
     * updates the provided Intersection object.
     * @note Intersections farther away than the previous value of @c its.t will
     * be dismissed.
     * @param rng Some shapes can randomly decide whether they are intersected,
     * for example to support transparency (alpha masking), or to implement
     * volumes using shapes.
     */
    virtual bool intersect(const Ray &ray, Intersection &its,
                           Sampler &rng) const = 0;

    /**
     * @brief Computes what fraction of light makes it through along the ray
     * until distance tMax.
     * @param rng More complex shapes may require random sampling to determine
     * their transmittance (e.g., heterogeneous volumes with ratio tracking).
     */
    virtual float transmittance(const Ray &ray, float tMax,
                                Sampler &rng) const {
        // Test the shape for intersection. If it is hit, light is assumed to be
        // blocked fully (i.e., the transmittance is 0). Otherwise, all light
        // passes through (transmittance is 1). If you want transmittance to be
        // material dependent, overload this method in the Instance class.
        Intersection its(-ray.direction, tMax);
        return intersect(ray, its, rng) ? 0.f : 1.f;
    }

    /// @brief Returns a bounding box that tightly encapsulates the shape.
    virtual Bounds getBoundingBox() const = 0;
    /**
     * @brief Returns the center of the shape, which must lie somewhere within
     * the bounding box of this shape.
     * @note Different shapes may have different definitions of "center" (some
     * might report center of mass, some might report an average of surface
     * points). Which definition is used is not strictly important, as long as
     * the centroid lies within the bounding box and can be used for
     * partitioning objects (e.g., when building a BVH structure).
     */
    virtual Point getCentroid() const = 0;
    /// @brief Samples a random point on the surface of this shape.
    virtual AreaSample sampleArea(Sampler &rng) const { NOT_IMPLEMENTED }

    /**
     * @brief Marks that the shape is part of the scene geometry, i.e., can be
     * hit through @ref Scene::intersect .
     * @example A shape that is added to an area light could be invisible to ray
     * tracing, if it is not also added to the scene using a reference.
     */
    virtual void markAsVisible() {}
};

} // namespace lightwave
