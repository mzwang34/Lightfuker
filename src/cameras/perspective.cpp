#include <lightwave.hpp>

namespace lightwave {

/**
 * @brief A perspective camera with a given field of view angle and transform.
 *
 * In local coordinates (before applying m_transform), the camera looks in
 * positive z direction [0,0,1]. Pixels on the left side of the image ( @code
 * normalized.x < 0 @endcode ) are directed in negative x direction ( @code
 * ray.direction.x < 0 ), and pixels at the bottom of the image ( @code
 * normalized.y < 0 @endcode ) are directed in negative y direction ( @code
 * ray.direction.y < 0 ).
 */
class Perspective : public Camera {
public:
    Perspective(const Properties &properties) : Camera(properties) {
        const float fov           = properties.get<float>("fov");
        const std::string fovAxis = properties.get<std::string>("fovAxis");

        // NOT_IMPLEMENTED

        // hints:
        // * precompute any expensive operations here (most importantly
        // trigonometric functions)
        // * use m_resolution to find the aspect ratio of the image

        Vector z = Vector(0.0, 0.0, 1.0);
        float s_x_norm, s_y_norm;
        float ratio = (float) m_resolution.x() / m_resolution.y();
        if (fovAxis == "x") {
            s_x_norm = tan(fov * Pi / 360.0);
            s_y_norm = s_x_norm / ratio;
        } else {
            s_y_norm = tan(fov * Pi / 360.0);
            s_x_norm = ratio * s_y_norm;
        }
        Vector up = Vector(0.0, 1.0, 0.0);
        s_x       = up.cross(z);
        s_y       = z.cross(s_x);
        s_x       = s_x_norm * s_x.normalized();
        s_y       = s_y_norm * s_y.normalized();
    }

    CameraSample sample(const Point2 &normalized, Sampler &rng) const override {
        // NOT_IMPLEMENTED

        // hints:
        // * use m_transform to transform the local camera coordinate system
        // into the world coordinate system
        Vector z   = Vector(0.0, 0.0, 1.0);
        Vector dir = z + normalized.x() * s_x + normalized.y() * s_y;
        Ray r      = Ray(Point(0.f), dir.normalized());
        r          = m_transform->apply(r);
        return CameraSample{ r.normalized(), Color(1.0) };
    }

    std::string toString() const override {
        return tfm::format(
            "Perspective[\n"
            "  width = %d,\n"
            "  height = %d,\n"
            "  transform = %s,\n"
            "]",
            m_resolution.x(),
            m_resolution.y(),
            indent(m_transform));
    }

protected:
    Vector s_x, s_y;
};

} // namespace lightwave

REGISTER_CAMERA(Perspective, "perspective")
