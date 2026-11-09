#include "atg/Geometry.h"
#include "Model.h"

#include <cmath>
#include <limits>
#include <unordered_map>

namespace atg {

MeshData ExtractGeometry(const Model& p_model) {
    MeshData m_out;

    for (const auto& iii_mesh : p_model.GetMeshes()) {
        // indices of this mesh start after everything merged so far
        uint32_t t_base = (uint32_t)m_out.positions.size();

        for (const Vertex& ii_vert : iii_mesh->c_vertices) {
            glm::vec4 t_world = iii_mesh->c_modelPos * glm::vec4(ii_vert.position, 1.0f);
            m_out.positions.push_back(glm::vec3(t_world));
        }
        for (GLuint ii_index : iii_mesh->c_indices) {
            m_out.indices.push_back(t_base + (uint32_t)ii_index);
        }
    }

    WeldVertices(m_out);
    return m_out;
}

void WeldVertices(MeshData& p_mesh, float p_epsilon) {
    // snap each position to an integer grid cell of size p_epsilon and hash the cell.
    // good enough for seam duplicates, which are bit-identical or nearly so.
    struct CellKey {
        int64_t x, y, z;
        bool operator==(const CellKey& p_other) const {
            return x == p_other.x && y == p_other.y && z == p_other.z;
        }
    };
    struct CellHash {
        size_t operator()(const CellKey& p_key) const {
            return (size_t)(p_key.x * 73856093) ^ (size_t)(p_key.y * 19349663) ^ (size_t)(p_key.z * 83492791);
        }
    };

    std::unordered_map<CellKey, uint32_t, CellHash> m_cellToNewIndex;
    std::vector<glm::vec3> m_newPositions;
    std::vector<uint32_t>  m_oldToNew(p_mesh.positions.size());

    for (size_t iii = 0; iii < p_mesh.positions.size(); iii++) {
        const glm::vec3& t_pos = p_mesh.positions[iii];
        CellKey t_key {
            (int64_t)std::llround(t_pos.x / p_epsilon),
            (int64_t)std::llround(t_pos.y / p_epsilon),
            (int64_t)std::llround(t_pos.z / p_epsilon)
        };

        auto t_found = m_cellToNewIndex.find(t_key);
        if (t_found != m_cellToNewIndex.end()) {
            m_oldToNew[iii] = t_found->second;
        } else {
            uint32_t t_newIndex = (uint32_t)m_newPositions.size();
            m_cellToNewIndex[t_key] = t_newIndex;
            m_newPositions.push_back(t_pos);
            m_oldToNew[iii] = t_newIndex;
        }
    }

    std::vector<uint32_t> m_newIndices;
    m_newIndices.reserve(p_mesh.indices.size());
    for (size_t iii = 0; iii + 2 < p_mesh.indices.size(); iii += 3) {
        uint32_t t_a = m_oldToNew[p_mesh.indices[iii + 0]];
        uint32_t t_b = m_oldToNew[p_mesh.indices[iii + 1]];
        uint32_t t_c = m_oldToNew[p_mesh.indices[iii + 2]];
        if (t_a == t_b || t_b == t_c || t_a == t_c) continue; // collapsed into a line/point

        m_newIndices.push_back(t_a);
        m_newIndices.push_back(t_b);
        m_newIndices.push_back(t_c);
    }

    p_mesh.positions = std::move(m_newPositions);
    p_mesh.indices   = std::move(m_newIndices);
}

void NormalizeToWorkspace(MeshData& p_mesh, const glm::vec3& p_center, float p_maxExtent) {
    if (p_mesh.positions.empty()) return;

    glm::vec3 m_min( std::numeric_limits<float>::max());
    glm::vec3 m_max(-std::numeric_limits<float>::max());
    for (const glm::vec3& iii_pos : p_mesh.positions) {
        m_min = glm::min(m_min, iii_pos);
        m_max = glm::max(m_max, iii_pos);
    }

    glm::vec3 m_size = m_max - m_min;
    float m_largest = std::max(m_size.x, std::max(m_size.y, m_size.z));
    if (m_largest <= 0.0f) return;

    float m_scale = p_maxExtent / m_largest;
    glm::vec3 m_boxCenter = 0.5f * (m_min + m_max);

    for (glm::vec3& iii_pos : p_mesh.positions) {
        iii_pos = (iii_pos - m_boxCenter) * m_scale + p_center;
    }
}

float TriangleArea(const MeshData& p_mesh, size_t p_triangle) {
    const glm::vec3& t_a = p_mesh.positions[p_mesh.indices[p_triangle * 3 + 0]];
    const glm::vec3& t_b = p_mesh.positions[p_mesh.indices[p_triangle * 3 + 1]];
    const glm::vec3& t_c = p_mesh.positions[p_mesh.indices[p_triangle * 3 + 2]];
    return 0.5f * glm::length(glm::cross(t_b - t_a, t_c - t_a));
}

float SurfaceArea(const MeshData& p_mesh) {
    float m_total = 0.0f;
    for (size_t iii = 0; iii < p_mesh.indices.size() / 3; iii++) {
        m_total += TriangleArea(p_mesh, iii);
    }
    return m_total;
}


// { 
//     // 1. Get the total number of vertices
//     size_t vertexCount = mesh.positions.size();
//
//     // 2. Get the total number of indices and triangles
//     size_t indexCount = mesh.indices.size();
//     size_t triangleCount = indexCount / 3; 
//
//     // --- Example: Limit printing to the first 5 elements ---
//     size_t maxVerticesToPrint = std::min<size_t>(5, vertexCount);
//     size_t maxTrianglesToPrint = std::min<size_t>(5, triangleCount);
//
//     std::cout << "--- Vertices ---\n";
//     for (size_t i = 0; i < maxVerticesToPrint; ++i) {
//         const auto& v = mesh.positions[i];
//         std::cout << "Vertex " << i << ": (" << v.x << ", " << v.y << ", " << v.z << ")\n";
//     }
//
//     std::cout << "\n--- Triangles ---\n";
//     for (size_t i = 0; i < maxTrianglesToPrint; ++i) {
//         // Each triangle uses 3 consecutive indices
//         size_t idx = i * 3;
//         std::cout << "Triangle " << i << ": [" 
//                   << mesh.indices[idx] << ", " 
//                   << mesh.indices[idx + 1] << ", " 
//                   << mesh.indices[idx + 2] << "]\n";
//     }
// }


std::ostream& operator<<(std::ostream& os, const MeshData& p_mesh) {
    os << "========== MeshData ==========\n";

    // Positions
    os << "Positions (" << p_mesh.positions.size() << "):\n";
    
    // size_t m_maxElements = 12; 
    // for (size_t ii = 0; ii < m_maxElements; ++ii) {
    for (size_t ii = 0; ii < p_mesh.positions.size(); ++ii) {
        const auto& pos = p_mesh.positions[ii];

        os << "  [" << std::setw(4) << ii << "] "
           << "X: " << std::setw(10) << pos.x << " | "
           << "Y: " << std::setw(10) << pos.y << " | "
           << "Z: " << std::setw(10) << pos.z << '\n';
    }


    // Indices
    os << "\nIndices (" << p_mesh.indices.size() << "):\n";

    // for (size_t ii = 0; ii < m_maxElements; ii += 3) {
    for (size_t ii = 0; ii < p_mesh.indices.size(); ii += 3) {
        os << "  Triangle " << std::setw(4) << ii / 3 << ": ";

        for (size_t jj = ii; jj < std::min(ii + 3, p_mesh.indices.size()); ++jj) {
            os << std::setw(6) << p_mesh.indices[jj] << ' ';
        }
        os << '\n';
    }
    os << "==============================\n";
    return os;
}


} // namespace atg

