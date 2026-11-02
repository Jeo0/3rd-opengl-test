// scratch harness: load a model, run every registered sampler at a few budgets,
// print one csv row per run so the results drop straight into a thesis table.
//
// Model's constructor uploads to the gpu (VAO/VBO/textures), so we still need a gl
// context -- just an invisible one.

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include "Model.h"
#include "atg/Geometry.h"
#include "atg/Sampling.h"

#include <iostream>

int main(int argc, char** argv) {
    std::string m_modelPath = (argc > 1) ? argv[1] : "resource/Models/simpleCube/simplecube.gltf";

    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* m_hidden = glfwCreateWindow(1, 1, "atg", NULL, NULL);
    glfwMakeContextCurrent(m_hidden);
    gladLoadGL();

    {
        Model m_model(m_modelPath);
        atg::MeshData m_mesh = atg::ExtractGeometry(m_model);

        // physical size inside the levitator, in metres. change to your workspace.
        atg::NormalizeToWorkspace(m_mesh, glm::vec3(0.0f), 0.05f);
        float m_area = atg::SurfaceArea(m_mesh);

        std::cerr << "INFO welded verts: " << m_mesh.positions.size()
                  << "  tris: " << m_mesh.indices.size() / 3
                  << "  area (m^2): " << m_area << "\n";

        std::cout << "particles,targetN,spacing_mm,sampler,N,coverageMean_mm,coverageMax_mm,"
                     "minSpacing_mm,spacingCV,tour_mm,drawable_mm\n";

        for (int iii_particles : {1, 2, 4, 8}) {
            atg::BudgetParams t_budgetParams;
            t_budgetParams.particleCount = iii_particles;
            atg::Budget t_budget = atg::EstimateBudget(m_area, t_budgetParams);

            for (const auto& [ii_name, ii_sampler] : atg::GetSamplers()) {
                atg::SampleParams t_params;
                t_params.targetCount = t_budget.pointCount;
                t_params.minSpacing  = t_budget.spacing;

                atg::PointSet t_points = ii_sampler(m_mesh, t_params);
                atg::SampleMetrics t_m = atg::Evaluate(m_mesh, t_points);

                std::cout << iii_particles << ',' << t_budget.pointCount << ',' << t_budget.spacing * 1e3f << ','
                          << ii_name << ',' << t_m.pointCount << ','
                          << t_m.coverageMean * 1e3f << ',' << t_m.coverageMax * 1e3f << ','
                          << t_m.minSpacing * 1e3f << ',' << t_m.spacingCV << ','
                          << t_m.tourLength * 1e3f << ',' << t_budget.drawableLength * 1e3f << '\n';
            }
        }
    } // Model (and its gl objects) die before the context does

    glfwDestroyWindow(m_hidden);
    glfwTerminate();
}

