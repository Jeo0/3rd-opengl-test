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

class Scene {
    private:
        std::unique_ptr<Shader> c_shader; 
        std::unique_ptr<Camera> c_camera; 
        std::vector<std::unique_ptr<Model>> c_sceneObjects; 

        // debug REMOVE
        float debug_time_iii = 0.0;
        bool debug_time_iii_flag = false;
        double bgColor[4] = {0.0f, 0.4f, 1.0f, 1.0f};

    public:
        Scene(int p_viewportHeight, int p_viewportWidth);
        ~Scene();

        bool LoadModel(const std::string& p_filePath);
        void Update(GLFWwindow* p_window, double p_deltaTime);
        void Render(GLFWwindow* p_window, double p_deltaTime);

        Camera* GetCamera() { return c_camera.get(); }

};
