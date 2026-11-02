#include "core/core.h"
#include "GLFW/glfw3.h"
#include <filesystem>
// #include "gl/Texture.h"
// #include "core/globals.h"
// #include <iostream>
// #include <memory>
// #include "gl/VBO.h"
//
// // 3d
// // #include "glm/glm.hpp"
// #include "glm/detail/qualifier.hpp"
// #include "glm/gtc/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"
// // #include "glm/ext/matrix_clip_space.hpp"
// #include "glm/ext/matrix_float4x4.hpp"
// #include "glm/ext/matrix_transform.hpp"
// #include "glm/ext/vector_float3.hpp"
// #include "glm/trigonometric.hpp"
// #include <glm/ext/matrix_clip_space.hpp>
// #include <utility>
//
// #include "Camera.h"

Core::Core() :  windowObj(800, 800, "gademNoice", NULL, NULL, {0.01f, 0.24f, 0.45f, 1.0f})
{
    Init();
}

Core::~Core() { Cleanup(); }

void Core::Run() {
    while (!glfwWindowShouldClose(windowObj.ID)) {
        glfwPollEvents();           // poll first so InputHandler::Poll reads this frame's state

        float mDeltaTime = cLimiter.StartFrame();
        Update(mDeltaTime);
        Render(mDeltaTime);         // buffer swap

        cLimiter.EndFrame();
    }
}

void Core::Init() {

    // ============================
    // uniforms
    // uniformID = glGetUniformLocation(shaderProgram.ID, "scale");


    // ============================
    // opengl settings
    glEnable(GL_DEPTH_TEST);

    // ============================
    // object instantiations
    c_scene = std::make_unique<Scene>(windowObj.Height, windowObj.Width);
    c_setset = std::make_unique<Settings>(cLimiter);
    c_inputHandler = std::make_unique<InputHandler>();


    std::string linuxm_models[] {
        "resource/Models/tricycle/scene.gltf",
        "resource/Models/simpleCube/cube2.gltf",
        "resource/Models/simpleCube/simplecube.gltf",
        "resource/Models/spear/scene.gltf"
    };
    c_scene->LoadModel(linuxm_models[3]);

    // std::string windowsm_models[] {
    //     "resource\\Models\\tricycle\\scene.gltf",
    //     "resource\\Models\\simpleCube\\cube2.gltf",
    //     "resource\\Models\\simpleCube\\simplecube.gltf",
    //     "resource\\Models\\spear\\scene.gltf"
    // };
    // c_scene->LoadModel(windowsm_models[3]);
}


void Core::Update(double p_deltaTime) {
    /*  LOGIC TINGS */
    // settings
    c_setset->LoadSettings(cLimiter);

    // input layer
    const InputFrame m_input = c_inputHandler->Poll(windowObj.ID);

    // scene only ever sees the snapshot, never the GLFWwindow*
    c_scene->Update(m_input, p_deltaTime);

    // this no work i dont even have any implementation for dis shit. I'm letting the camera handle the input lmao
    // windowObj.HandleMouseInput(); // core handles the window 
}



void Core::Render(double p_deltaTime) {
    /*  RENDER TINGS */
    c_scene->Render(windowObj.ID, p_deltaTime);
}

void Core::Cleanup() {
    glfwDestroyWindow(windowObj.ID);
    glfwTerminate();
}

