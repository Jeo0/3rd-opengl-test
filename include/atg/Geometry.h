// atg layer 1 -- geometry plumbing
// everything in here is cpu only and never touches opengl, so it runs the same
// inside the app, in a scratch test, or in a unit test.
// the renderer's Mesh/Model are treated as READ ONLY sources; we copy out of them.

#pragma once
#include "glm/glm.hpp"
#include <cstdint>
#include <vector>

class Model; // forward declare so this header doesn't drag GL in

namespace atg {

    // plain triangle soup in world space, welded. no normals/uvs/colors on purpose:
    // trajectory generation only cares about where the surface is.
    struct MeshData {
        std::vector<glm::vec3> positions;
        std::vector<uint32_t>  indices;     // 3 per triangle
    };

    // what every sampler produces. kept separate from MeshData since it has no faces.
    using PointSet = std::vector<glm::vec3>;

    // bakes each Mesh's c_modelPos into its vertices and merges all meshes into one
    MeshData ExtractGeometry(const Model& p_model);

    // gltf splits vertices on uv/normal seams (a cube comes in as 24 verts, not 8).
    // merges vertices closer than p_epsilon and drops triangles that collapse.
    void WeldVertices(MeshData& p_mesh, float p_epsilon = 1e-5f);

    // uniform scale + translate so the largest bounding box side == p_maxExtent and the
    // box is centered on p_center. units are whatever p_maxExtent is in (use metres).
    void NormalizeToWorkspace(MeshData& p_mesh, const glm::vec3& p_center, float p_maxExtent);

    float SurfaceArea(const MeshData& p_mesh);
    float TriangleArea(const MeshData& p_mesh, size_t p_triangle);

} // namespace atg

