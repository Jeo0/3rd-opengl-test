// atg layer 1 -- geometry plumbing
// everything in here is cpu only and never touches opengl, so it runs the same
// inside the app, in a scratch test, or in a unit test.
// the renderer's Mesh/Model are treated as READ ONLY sources; we copy out of them.

#pragma once
#include "glm/glm.hpp"
#include <iomanip>
#include <cstdint>
#include <vector>

class Model; // forward declare so this header doesn't drag GL in

// namespace atg {
//
//     // plain triangle soup in world space, welded. no normals/uvs/colors on purpose:
//     // trajectory generation only cares about where the surface is.
//     struct MeshData {
//         std::vector<glm::vec3> positions;
//         std::vector<uint32_t>  indices;     // 3 per triangle
//     };
//
//     // what every sampler produces. kept separate from MeshData since it has no faces.
//     using PointSet = std::vector<glm::vec3>;
//
//     // bakes each Mesh's c_modelPos into its vertices and merges all meshes into one
//     MeshData ExtractGeometry(const Model& p_model);
//
//     // gltf splits vertices on uv/normal seams (a cube comes in as 24 verts, not 8).
//     // merges vertices closer than p_epsilon and drops triangles that collapse.
//     void WeldVertices(MeshData& p_mesh, float p_epsilon = 1e-5f);
//
//     // uniform scale + translate so the largest bounding box side == p_maxExtent and the
//     // box is centered on p_center. units are whatever p_maxExtent is in (use metres).
//     void NormalizeToWorkspace(MeshData& p_mesh, const glm::vec3& p_center, float p_maxExtent);
//
//     float SurfaceArea(const MeshData& p_mesh);
//     float TriangleArea(const MeshData& p_mesh, size_t p_triangle);
//
//     std::ostream& operator<<(std::ostream& os, const MeshData& mesh);
// } // namespace atg

namespace atg
{
    // one particle's path, sampled at a fixed rate
    struct ParticlePath
    {
        std::vector<glm::vec3> samples;   // metres, openmpd frame
    };

    struct Trajectory
    {
        float sampleRate;                 // samples per second
        std::vector<ParticlePath> particles;
    };

    // placeholder until the real pipeline exists
    Trajectory MakeCubeTrajectory(float p_side, float p_speed, float p_sampleRate);

    // writes one csv per particle in the tutorial 8 layout (x, y, z, 1)
    void WriteDescriptorCSV(const Trajectory& p_traj, const std::string& p_folder);
}
