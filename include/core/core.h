#pragma once
#include <iostream>
#include <array>
#include <fstream>
#include <memory>
#include <vector>
#include "core/FrameLimit.h"
#include "stb/stb_image.h"
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

// heap allocated objects
#include "Camera.h"
#include "core/Settings.h"
#include "Mesh.h"

#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "window/window.h"


class Core {
private:
    // declaration order matters! Window must be initialized first 
    // to establish the OpenGL context before VAO/VBO/Shaders are created.
    Window windowObj;
    Shader shaderProgram;

    double bgColor[4] = {0.0f, 0.4f, 1.0f, 1.0f};
    float indexer = 0.001f;

    // debug REMOVE
    float debug_time_iii = 0.0;
    bool debug_time_iii_flag = false;

    // timing 
    FrameLimit cLimiter {67.0};     // fps cap

    // possible tings 
    // Camera* camcam = nullptr;
    // by initial, no objects exists 
    // unless instantiated
    std::unique_ptr<Camera> camcam;
    std::unique_ptr<Settings> setset;
    std::vector<std::unique_ptr<Mesh>> c_sceneObjects;

    // behaviours
    void Init();
        // Linking Attributes   // VAO VBO EBO
        // Get uniforms

    void Update(double p_deltaTime);
        // Handle all logic and state changes here (separate from drawing)

    void Render(double p_deltaTime);
        // Handle all OpenGL drawing commands here

    void Cleanup();

    void AddObjectToScene(std::vector<Vertex>& p_vertices,
                         std::vector<GLuint>& p_indices,
                         const std::string& p_textureFile,
                         const glm::vec3& p_positionDefault = glm::vec3(0.0f));

public:
    Core();
    ~Core();
    void Run();

};
