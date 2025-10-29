#include <lightwave.hpp>

#include "../core/plyparser.hpp"
#include "accel.hpp"

namespace lightwave {

/**
 * @brief A shape consisting of many (potentially millions) of triangles, which
 * share an index and vertex buffer. Since individual triangles are rarely
 * needed (and would pose an excessive amount of overhead), collections of
 * triangles are combined in a single shape.
 */
class TriangleMesh : public AccelerationStructure {
    /**
     * @brief The index buffer of the triangles.
     * The n-th element corresponds to the n-th triangle, and each component of
     * the element corresponds to one vertex index (into @c m_vertices ) of the
     * triangle. This list will always contain as many elements as there are
     * triangles.
     */
    std::vector<Vector3i> m_triangles;
    /**
     * @brief The vertex buffer of the triangles, indexed by m_triangles.
     * Note that multiple triangles can share vertices, hence there can also be
     * fewer than @code 3 * numTriangles @endcode vertices.
     */
    std::vector<Vertex> m_vertices;
    /// @brief The file this mesh was loaded from, for logging and debugging
    /// purposes.
    std::filesystem::path m_originalPath;
    /// @brief Whether to interpolate the normals from m_vertices, or report the
    /// geometric normal instead.
    bool m_smoothNormals;

protected:
    int numberOfPrimitives() const override { return int(m_triangles.size()); }

    bool intersect(int primitiveIndex, const Ray &ray, Intersection &its,
                   Sampler &rng) const override {
        // hints:
        // * use m_triangles[primitiveIndex] to get the vertex indices of the
        // triangle that should be intersected
        // * if m_smoothNormals is true, interpolate the vertex normals from
        // m_vertices
        //   * make sure that your shading frame stays orthonormal!
        // * if m_smoothNormals is false, use the geometrical normal (can be
        // computed from the vertex positions)
        Vector d = ray.direction;
        Point o = ray.origin;
        Vector3i vert_indices = m_triangles[primitiveIndex];

        Vertex v0 = m_vertices[vert_indices[0]];
        Vertex v1 = m_vertices[vert_indices[1]];
        Vertex v2 = m_vertices[vert_indices[2]];

        // (1 - u - v)* v0 + u * v1+ v * v2 = o + td
        Vector e1 = v1.position - v0.position;
        Vector e2 = v2.position - v0.position;

        float detM = e1.dot(d.cross(e2));

        if (detM == 0)
            return false;
        float invDetM = 1.f / detM;

        Vector e_ori = o - v0.position;
        float detMu  = e_ori.dot(d.cross(e2));
        float u = detMu * invDetM;
        if (u < 0 || u > 1)
            return false;

        float detMv = d.dot(e_ori.cross(e1));
        float v = detMv * invDetM;
        if (v < 0 || u + v > 1)
            return false;

        float t = e2.dot(e_ori.cross(e1)) * invDetM;

        if (t < Epsilon || t > its.t)
            return false;

        its.t = t;
        its.position = ray(t);

        if (m_smoothNormals) {
            Vertex v_interp = Vertex::interpolate(Vector2(u, v), v0, v1, v2);
            its.shadingNormal = v_interp.normal.normalized();
        } else {
            its.shadingNormal = e1.cross(e2).normalized();
        }
        its.geometryNormal = e1.cross(e2).normalized();
        its.tangent = Frame(its.shadingNormal).tangent;
        its.pdf = 0.f; // TODO

        return true;
    }

    float transmittance(int primitiveIndex, const Ray &ray, float tMax,
                        Sampler &rng) const override {
        Intersection its(-ray.direction, tMax);
        return intersect(ray, its, rng) ? 0.f : 1.f;
    }

    Bounds getBoundingBox(int primitiveIndex) const override {
        Bounds bounds;
        bounds.empty();
        Vector3i vert_indices = m_triangles[primitiveIndex];
        bounds.extend(m_vertices[vert_indices[0]].position);
        bounds.extend(m_vertices[vert_indices[1]].position);
        bounds.extend(m_vertices[vert_indices[2]].position);
        return bounds;
    }

    Point getCentroid(int primitiveIndex) const override {
        Vector psum(0.f);
        for (int i = 0; i < 3; i++) psum = psum + (Vector)m_vertices[m_triangles[primitiveIndex][i]].position;
        return (1.f / 3.f) * psum;
    }

public:
    TriangleMesh(const Properties &properties) {
        m_originalPath  = properties.get<std::filesystem::path>("filename");
        m_smoothNormals = properties.get<bool>("smooth", true);
        readPLY(m_originalPath, m_triangles, m_vertices);
        logger(EInfo,
               "loaded ply with %d triangles, %d vertices",
               m_triangles.size(),
               m_vertices.size());
        buildAccelerationStructure();
    }

    bool intersect(const Ray &ray, Intersection &its,
                   Sampler &rng) const override {
        PROFILE("Triangle mesh")
        return AccelerationStructure::intersect(ray, its, rng);
    }

    AreaSample sampleArea(Sampler &rng) const override{
        // only implement this if you need triangle mesh area light sampling for
        // your rendering competition
        NOT_IMPLEMENTED
    }

    std::string toString() const override {
        return tfm::format(
            "Mesh[\n"
            "  vertices = %d,\n"
            "  triangles = %d,\n"
            "  filename = \"%s\"\n"
            "]",
            m_vertices.size(),
            m_triangles.size(),
            m_originalPath.generic_string());
    }
};

} // namespace lightwave

REGISTER_SHAPE(TriangleMesh, "mesh")
