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
    VAO VAO1;
    VBO VBO1;
    EBO EBO1;
    GLuint uniformID;
    GLuint textureID; 

    double bgColor[4] = {0.0f, 0.4f, 1.0f, 1.0f};
    float indexer = 0.001f;

    // timing 
    FrameLimit cLimiter {67.0};     // fps cap

    // possible tings 
    // Camera* camcam = nullptr;
    // by initial, no objects exists 
    // unless instantiated
    std::unique_ptr<Camera> camcam;
    std::unique_ptr<Settings> setset;

    // behaviours
    void Init();
        // Linking Attributes
        // Get uniforms

    void Update(double pDeltaTime);
        // Handle all logic and state changes here (separate from drawing)

    void Render();
        // Handle all OpenGL drawing commands here

    void Cleanup();

public:
    Core();
    ~Core();
    void Run();

};
