#include <lightwave.hpp>

namespace lightwave {

/// @brief A rectangle in the xy-plane, spanning from [-1,-1,0] to [+1,+1,0].
class Rectangle : public Shape {
    /**
     * @brief Constructs a surface event for a given position, used by @ref
     * intersect to populate the @ref Intersection and by @ref sampleArea to
     * populate the @ref AreaSample .
     * @param surf The surface event to populate with texture coordinates,
     * shading frame and area pdf
     * @param position The hitpoint (i.e., point in [-1,-1,0] to [+1,+1,0]),
     * found via intersection or area sampling
     */
    inline void populate(SurfaceEvent &surf, const Point &position) const {
        surf.position = position;

        // map the position from [-1,-1,0]..[+1,+1,0] to [0,0]..[1,1] by
        // discarding the z component and rescaling
        surf.uv.x() = (position.x() + 1) / 2;
        surf.uv.y() = (position.y() + 1) / 2;

        // the tangent always points in positive x direction
        surf.tangent = Vector(1, 0, 0);
        // and accordingly, the normal always points in the positive z direction
        surf.shadingNormal  = Vector(0, 0, 1);
        surf.geometryNormal = Vector(0, 0, 1);

        // since we sample the area uniformly, the pdf is given by 1/surfaceArea
        surf.pdf = 1.0f / 4;
    }

public:
    Rectangle(const Properties &properties) {}

    bool intersect(const Ray &ray, Intersection &its,
                   Sampler &rng) const override {
        PROFILE("Rectangle")

        // if the ray travels in the xy-plane, we report no intersection
        // (we ignore the edge case - pun intended - that the ray might have
        // infinite intersections with the rectangle)
        if (ray.direction.z() == 0)
            return false;

        // ray.origin.z + t * ray.direction.z = 0
        // <=> t = -ray.origin.z / ray.direction.z
        const float t = -ray.origin.z() / ray.direction.z();

        // note that we never report an intersection closer than Epsilon (to
        // avoid self-intersections)! we also do not update the intersection if
        // a closer intersection already exists (i.e., its.t is lower than our
        // own t)
        if (t < Epsilon || t > its.t)
            return false;

        // compute the hitpoint
        const Point position = ray(t);
        // we have intersected an infinite plane at z=0; now dismiss anything
        // outside of the [-1,-1,0]..[+1,+1,0] domain.
        if (abs(position.x()) > 1 || abs(position.y()) > 1)
            return false;

        // we have determined there was an intersection! we are now free to
        // change the intersection object and return true.
        its.t = t;
        populate(its,
                 position); // compute the shading frame, texture coordinates
                            // and area pdf (same as sampleArea)
        return true;
    }

    Bounds getBoundingBox() const override {
        return Bounds(Point{ -1, -1, 0 }, Point{ +1, +1, 0 });
    }

    Point getCentroid() const override { return Point(0); }

    AreaSample sampleArea(Sampler &rng) const override {
        Point2 rnd = rng.next2D(); // sample a random point in [0,0]..[1,1]
        Point position{
            2 * rnd.x() - 1, 2 * rnd.y() - 1, 0
        }; // stretch the random point to [-1,-1]..[+1,+1] and set z=0

        AreaSample sample;
        populate(sample,
                 position); // compute the shading frame, texture coordinates
                            // and area pdf (same as intersection)
        return sample;
    }

    AreaSample sampleArea(const Point &origin, Sampler &rng) const override {
        // An Area-Preserving Parametrization for Spherical Rectangles EGSR 2013
        // Rectangle corners (counter-clockwise)
        Vector corners[4] = { Vector(-1.f, -1.f, 0.f),
                              Vector(1.f, -1.f, 0.f),
                              Vector(1.f, 1.f, 0.f),
                              Vector(-1.f, 1.f, 0.f) };

        // Directions to corners
        Vector v[4];
        for (int i = 0; i < 4; ++i)
            v[i] = (corners[i] - origin).normalized();

        // Total solid angle

        float omega = 0.0f;

        for (int i = 0; i < 4; ++i) {
            const Vector &a = v[i];
            const Vector &b = v[(i + 1) & 3];

            float cosTheta = a.dot(b);
            float sinTheta = a.cross(b).length();

            omega += std::atan2(sinTheta, cosTheta);
        }
        omega = 2.0f * omega;

        // Local frame (EGSR construction)
        Vector ex = (v[1] - v[0]).normalized();
        Vector ez = v[0].cross(v[1]).normalized();
        Vector ey = ez.cross(ex);

        auto frame = Frame(ex, ey, ez);

        // Spherical bounds
        float phiMin   = std::acos(v[0].z());
        float phiMax   = std::acos(v[2].z());
        float thetaMin = std::atan2(v[0].y(), v[0].x());
        float thetaMax = std::atan2(v[2].y(), v[2].x());

        // Uniform solid-angle sampling
        Point2 uv = rng.next2D();
        float phi = std::acos(std::cos(phiMin) +
                              uv.x() * (std::cos(phiMax) - std::cos(phiMin)));

        float theta = thetaMin + uv.y() * (thetaMax - thetaMin);

        Vector sample_dir(std::sin(phi) * std::cos(theta),
                          std::sin(phi) * std::sin(theta),
                          std::cos(phi));

        sample_dir = frame.toWorld(sample_dir);

        Intersection its;
        Ray ray(origin, sample_dir);
        // fix floating precision error at edge
        if (!intersect(ray, its, rng))
            return AreaSample::invalid();

        Point position = ray(its.t);

        AreaSample sample;
        populate(sample, position);

        sample.pdf = 1.0f / omega; // area pdf

        return sample;
    }

    std::string toString() const override { return "Rectangle[]"; }
};

} // namespace lightwave

// this informs lightwave to use our class Rectangle whenever a <shape
// type="rectangle" /> is found in a scene file
REGISTER_SHAPE(Rectangle, "rectangle")
