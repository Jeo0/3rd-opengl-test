#include "atg/Sampling.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

namespace atg {

// ============================================================
// budget
// ============================================================
Budget EstimateBudget(float p_surfaceArea, const BudgetParams& p_params) {
    Budget m_out {};
    m_out.drawableLength = p_params.particleCount * p_params.maxSpeed
                         * p_params.refreshPeriod * p_params.pathEfficiency;

    if (p_surfaceArea <= 0.0f || m_out.drawableLength <= 0.0f) return m_out;

    m_out.spacing    = p_surfaceArea / m_out.drawableLength;
    m_out.pointCount = std::max(p_params.particleCount,
                                (int)std::floor(m_out.drawableLength / m_out.spacing));
    return m_out;
}

// ============================================================
// samplers
// ============================================================
PointSet SampleRandomSurface(const MeshData& p_mesh, const SampleParams& p_params) {
    PointSet m_out;
    size_t m_triCount = p_mesh.indices.size() / 3;
    if (m_triCount == 0 || p_params.targetCount <= 0) return m_out;

    // cumulative area so bigger triangles get proportionally more points
    std::vector<float> m_cdf(m_triCount);
    float m_running = 0.0f;
    for (size_t iii = 0; iii < m_triCount; iii++) {
        m_running += TriangleArea(p_mesh, iii);
        m_cdf[iii] = m_running;
    }

    std::mt19937 m_rng(p_params.seed);
    std::uniform_real_distribution<float> m_unit(0.0f, 1.0f);

    m_out.reserve(p_params.targetCount);
    for (int iii = 0; iii < p_params.targetCount; iii++) {
        float t_pick = m_unit(m_rng) * m_running;
        size_t t_tri = std::lower_bound(m_cdf.begin(), m_cdf.end(), t_pick) - m_cdf.begin();
        t_tri = std::min(t_tri, m_triCount - 1);

        const glm::vec3& t_a = p_mesh.positions[p_mesh.indices[t_tri * 3 + 0]];
        const glm::vec3& t_b = p_mesh.positions[p_mesh.indices[t_tri * 3 + 1]];
        const glm::vec3& t_c = p_mesh.positions[p_mesh.indices[t_tri * 3 + 2]];

        // sqrt trick gives uniform barycentrics; without it points bunch at vertex a
        float t_r1 = std::sqrt(m_unit(m_rng));
        float t_r2 = m_unit(m_rng);
        m_out.push_back((1.0f - t_r1) * t_a + t_r1 * (1.0f - t_r2) * t_b + t_r1 * t_r2 * t_c);
    }
    return m_out;
}

const std::map<std::string, SamplerFn>& GetSamplers() {
    static const std::map<std::string, SamplerFn> s_samplers {
        {"randomSurface", SampleRandomSurface},
        // {"voxelGrid",      SampleVoxelGrid},
        // {"farthestPoint",  SampleFarthestPoint},
        // {"poissonDisk",    SamplePoissonDisk},
        // {"curvatureFps",   SampleCurvatureWeightedFps},
    };
    return s_samplers;
}

// ============================================================
// metrics
// ============================================================
namespace {
    // brute force is fine: N is tens to hundreds, reference is ~20k
    size_t NearestIndex(const PointSet& p_points, const glm::vec3& p_query, float& p_outDist2) {
        size_t m_best = 0;
        p_outDist2 = std::numeric_limits<float>::max();
        for (size_t iii = 0; iii < p_points.size(); iii++) {
            glm::vec3 t_d = p_points[iii] - p_query;
            float t_dist2 = glm::dot(t_d, t_d);
            if (t_dist2 < p_outDist2) { p_outDist2 = t_dist2; m_best = iii; }
        }
        return m_best;
    }
}

SampleMetrics Evaluate(const MeshData& p_mesh, const PointSet& p_samples,
                       int p_referenceCount, uint32_t p_seed) {
    SampleMetrics m_out {};
    m_out.pointCount = (int)p_samples.size();
    if (p_samples.empty()) return m_out;

    // coverage: how far is the surface from being represented
    SampleParams t_refParams;
    t_refParams.targetCount = p_referenceCount;
    t_refParams.seed = p_seed;
    PointSet m_reference = SampleRandomSurface(p_mesh, t_refParams);

    double m_sum = 0.0;
    for (const glm::vec3& iii_ref : m_reference) {
        float t_dist2;
        NearestIndex(p_samples, iii_ref, t_dist2);
        float t_dist = std::sqrt(t_dist2);
        m_sum += t_dist;
        m_out.coverageMax = std::max(m_out.coverageMax, t_dist);
    }
    m_out.coverageMean = m_reference.empty() ? 0.0f : (float)(m_sum / m_reference.size());

    // spacing: nearest neighbour of each sample among the other samples
    std::vector<float> m_nn(p_samples.size(), std::numeric_limits<float>::max());
    for (size_t iii = 0; iii < p_samples.size(); iii++) {
        for (size_t jjj = iii + 1; jjj < p_samples.size(); jjj++) {
            float t_dist = glm::length(p_samples[iii] - p_samples[jjj]);
            m_nn[iii] = std::min(m_nn[iii], t_dist);
            m_nn[jjj] = std::min(m_nn[jjj], t_dist);
        }
    }
    if (p_samples.size() > 1) {
        m_out.minSpacing = *std::min_element(m_nn.begin(), m_nn.end());
        double t_mean = 0.0, t_var = 0.0;
        for (float iii_d : m_nn) t_mean += iii_d;
        t_mean /= m_nn.size();
        for (float iii_d : m_nn) t_var += (iii_d - t_mean) * (iii_d - t_mean);
        t_var /= m_nn.size();
        m_out.spacingCV = t_mean > 0.0 ? (float)(std::sqrt(t_var) / t_mean) : 0.0f;
    }

    // tour: cheap proxy for "can a particle actually draw this in one refresh".
    // the real ordering stage (nn/tsp/hilbert) replaces this later.
    std::vector<bool> m_visited(p_samples.size(), false);
    size_t m_current = 0;
    m_visited[0] = true;
    for (size_t iii = 1; iii < p_samples.size(); iii++) {
        float t_best = std::numeric_limits<float>::max();
        size_t t_next = m_current;
        for (size_t jjj = 0; jjj < p_samples.size(); jjj++) {
            if (m_visited[jjj]) continue;
            float t_dist = glm::length(p_samples[jjj] - p_samples[m_current]);
            if (t_dist < t_best) { t_best = t_dist; t_next = jjj; }
        }
        m_visited[t_next] = true;
        m_out.tourLength += t_best;
        m_current = t_next;
    }
    m_out.tourLength += glm::length(p_samples[m_current] - p_samples[0]); // close the loop

    return m_out;
}

} // namespace atg

