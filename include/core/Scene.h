// handles the shader 
// camera 
// and objects

#pragma once
#include <string>
#include <vector>

#include "shaderClass.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Model.h"
// #include "Mesh.h"
#include "Render.h"
#include "Events.h" 

class Scene {
    private:
        std::unique_ptr<Shader> c_shader; 
        std::unique_ptr<Shader> c_pointShader;  // vertex dots


        std::unique_ptr<Camera> c_camera; 
        std::vector<std::unique_ptr<Model>> c_sceneObjects; 
        std::vector<std::unique_ptr<RenderPass>> c_passes; 

        // Non-owning handles into c_passes, used by Scene::Update for shortcuts.
        SurfacePass* c_surfacePass = nullptr;
        PointOverlayPass* c_pointOverlayPass = nullptr;

//#ifdef DEBUG_SHOW
        // debug REMOVE
        float debug_time_iii = 0.0;
        bool debug_time_iii_flag = false;
        double bgColor[4] = {0.0f, 0.4f, 1.0f, 1.0f};
//#endif

    public:
        Scene(int p_viewportHeight, int p_viewportWidth);
        ~Scene();

        bool LoadModel(const std::string& p_filePath);
        void Update(GLFWwindow* p_window, double p_deltaTime);
        void Render(GLFWwindow* p_window, double p_deltaTime);

        Camera* GetCamera() { return c_camera.get(); }

};
