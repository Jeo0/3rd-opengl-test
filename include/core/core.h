#pragma once
#include <iostream>
#include <array>
#include <fstream>
#include <vector>
#include "stb/stb_image.h"
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "window/window.h"


class Core {
private:
    // 1. Core Objects
    // Note: Declaration order matters! Window must be initialized first 
    // to establish the OpenGL context before VAO/VBO/Shaders are created.
    Window windowObj;
    Shader shaderProgram;
    VAO VAO1;
    VBO VBO1;
    EBO EBO1;
    GLuint uniformID;
    GLuint textureID; 

    // 2. Core State
    // double bgColor[4] = {1.0f, 0.4f, 1.0f, 1.0f};
    double bgColor[4] = {0.0f, 0.4f, 1.0f, 1.0f};
    float indexer = 0.001f;

    // behaviours
    void Init();
        // Linking Attributes
        // Get uniforms

    void Update();
        // Handle all logic and state changes here (separate from drawing)

    void Render();
        // Handle all OpenGL drawing commands here

    void Cleanup();

public:
    Core();
    ~Core();
    void Run();

};
