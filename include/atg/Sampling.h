// atg layer 1 -- point budget, samplers, and the metrics used to compare them.
// every sampler has the SAME signature so a test harness can loop over all of them.

#pragma once
#include "atg/Geometry.h"

#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace atg {

    // ------------------------------------------------------------
    // budget: how many points the hardware can actually draw
    // ------------------------------------------------------------
    struct BudgetParams {
        int   particleCount  = 1;       // simultaneous traps used for this shape (<= MAX_PRIMITIVES)
        float maxSpeed       = 1.0f;    // m/s, take from your own measurements, not the papers
        float refreshPeriod  = 0.1f;    // s, one full redraw of the shape (persistence of vision)
        float pathEfficiency = 0.6f;    // 0..1, loss from slowing down at corners + tour overhead
    };

    struct Budget {
        float drawableLength;   // m of path all particles can cover in one refresh
        float spacing;          // m between neighbouring samples so the path just covers the area
        int   pointCount;       // target N handed to the samplers
    };

    // N comes from physics: path length L = K*v*T*eff, and covering area A with a path of
    // length L leaves strips about s = A/L apart, so N ~= L/s = L^2/A.
    Budget EstimateBudget(float p_surfaceArea, const BudgetParams& p_params);

    // ------------------------------------------------------------
    // samplers
    // ------------------------------------------------------------
    struct SampleParams {
        int      targetCount = 32;
        float    minSpacing  = 0.0f;    // m, used by poisson-disk style samplers; 0 = ignore
        uint32_t seed        = 1;       // fixed seed so every run of the thesis tables is reproducible
    };

    using SamplerFn = std::function<PointSet(const MeshData&, const SampleParams&)>;

    // baseline: area-weighted uniform random points on the surface (not the vertices)
    PointSet SampleRandomSurface(const MeshData& p_mesh, const SampleParams& p_params);

    // name -> sampler. add new algorithms here and the harness picks them up.
    const std::map<std::string, SamplerFn>& GetSamplers();

    // ------------------------------------------------------------
    // metrics
    // ------------------------------------------------------------
    struct SampleMetrics {
        int   pointCount;
        float coverageMean;     // m, avg distance from a dense surface point to its nearest sample
        float coverageMax;      // m, worst case of the above (one-sided hausdorff) -> "holes"
        float minSpacing;       // m, closest pair of samples
        float spacingCV;        // stddev/mean of nearest-neighbour distance, 0 = perfectly even
        float tourLength;       // m, greedy nearest-neighbour tour through all samples (closed)
    };

    // p_referenceCount dense random points stand in for the continuous surface
    SampleMetrics Evaluate(const MeshData& p_mesh, const PointSet& p_samples,
            int p_referenceCount = 20000, uint32_t p_seed = 12345);

} // namespace atg

