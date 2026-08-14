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
#include "core/Settings.h"
#include "core/Scene.h"

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "window/window.h"


class Core {
private:
    // declaration order matters! Window must be initialized first 
    // to establish the OpenGL context before VAO/VBO/Shaders are created.
    Window windowObj;

    // timing 
    FrameLimit cLimiter {67.0};     // fps cap


    std::unique_ptr<Settings> c_setset;
    std::unique_ptr<Scene> c_scene;

    // behaviours
    void Init();
        // Linking Attributes   // VAO VBO EBO
        // Get uniforms

    void Update(double p_deltaTime);
        // Handle all logic and state changes here (separate from drawing)

    void Render(double p_deltaTime);
        // Handle all OpenGL drawing commands here

    void Cleanup();

public:
    Core();
    ~Core();
    void Run();

};
