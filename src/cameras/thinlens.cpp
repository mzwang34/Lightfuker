#include <lightwave.hpp>

namespace lightwave {

class ThinLens : public Camera {
public:
    ThinLens(const Properties &properties) : Camera(properties) {
        const float fov           = properties.get<float>("fov");
        const std::string fovAxis = properties.get<std::string>("fovAxis");
        m_lensRadius              = properties.get<float>("lensRadius", 0.f);
        m_focalDistance           = properties.get<float>("focalDistance", 1.f);
        m_bokeh                   = properties.get<Texture>("bokeh", nullptr);

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
        Vector z   = Vector(0.0, 0.0, 1.0);
        Vector dir = (z + normalized.x() * s_x + normalized.y() * s_y).normalized();

        Point origin(0.f);
        Color throughput(1.0f);
        if (m_lensRadius > 0) {
            Point2 sLens;
            if (m_bokeh) {
                int cnt = 0;
                while (cnt++ < 64) {
                    Point2 s = rng.next2D(); 
                    sLens = Point2(2 * s.x() - 1, 1 - 2 * s.y()); 

                    Color c = m_bokeh->evaluate(s);
                    if (c.luminance() > rng.next()) {
                        throughput *= c;
                        break;
                    }
                }
            } else {
                sLens = squareToUniformDiskConcentric(rng.next2D());
            }
            Point pLens(m_lensRadius * sLens.x(), m_lensRadius * sLens.y(), 0.f);

            float ft = m_focalDistance / dir.z();
            Point pFocus = dir * ft;

            origin = pLens;
            dir = (pFocus - origin).normalized();
        }
        Ray r(origin, dir);
        r = m_transform->apply(r);
        return CameraSample{ r.normalized(), throughput };
    }

    std::string toString() const override {
        return tfm::format(
            "ThinLens[\n"
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
    float m_lensRadius, m_focalDistance;
    ref<Texture> m_bokeh;
};

} // namespace lightwave

REGISTER_CAMERA(ThinLens, "thinlens")
