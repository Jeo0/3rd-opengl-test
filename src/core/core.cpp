#include "core/core.h"
#include "GLFW/glfw3.h"
#include "gl/Texture.h"
#include "core/globals.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <string_view>

#include "gl/VBO.h"

// 3d
// #include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <utility>

#include "Camera.h"

Core::Core() :  windowObj(800, 800, "gademNoice", NULL, NULL, {0.01f, 0.24f, 0.45f, 1.0f}),
                shaderProgram("resource/Shaders/default.vert", "resource/Shaders/default.frag") 
{
    Init();
}

Core::~Core() { Cleanup(); }

void Core::Run() {
    // The Main Game/Render Loop
    while (!glfwWindowShouldClose(windowObj.ID)) {
        float mDeltaTime = cLimiter.StartFrame();
        Update(mDeltaTime);
        Render(mDeltaTime);         // buffer swap
        glfwPollEvents(); // polling is a must after buffer swap

        cLimiter.EndFrame();
    }
}

void Core::Init() {

    // ============================
    // uniforms
    // uniformID = glGetUniformLocation(shaderProgram.ID, "scale");

    // ============================
    // textures
    std::string parentDir =
        (std::filesystem::current_path().std::filesystem::path::parent_path()).string();
    std::string textureFile = "/git/resource/Textures/gadem.jpg";
    // std::string textureFile = "/git/resource/Textures/popcat.png";
    // std::string textureFile = "/git/resource/Textures/pak.jpg";
    // std::cout << "USERINFO: opening dis ting -> " << parentDir << textureFile
    // << std::endl;

    // usage of textures
    // basis of the thing
    std::vector<Texture> textextures {
        // correct
        Texture((parentDir + textureFile), "diffuse_tex_type", 0, GL_RGBA, GL_UNSIGNED_BYTE) 
            // refactor here
            // "diffuse" instead of the current 
    };
    // simpleSquare.TexUnit(shaderProgram, "tex0", 0); // usage with uniform and whatever

    // ============================
    // open gl opengl settings
    glEnable(GL_DEPTH_TEST);

    // ============================
    // object instantiations
    //
    camcam = std::make_unique<Camera>(windowObj.Height, windowObj.Width, glm::vec3(0.0f, 0.0f, 2.0f));
    setset = std::make_unique<Settings>(cLimiter);

    cube_thing = std::make_unique<Mesh>(g_vertices, g_indices, textextures);
    shaderProgram.Activate();
}


void Core::Update(double p_deltaTime) {
    /*  LOGIC TINGS */
    // settings
    setset->LoadSettings(cLimiter);
    shaderProgram.Activate();

    // mouse input ++++============================== goback here
    windowObj.HandleMouseInput();
    camcam->HandleInputs(windowObj.ID, p_deltaTime); 
    camcam->UpdateMatrix(45.0, 1.0, 100.0);

    // debug ===================== REMOVE
    std::cout << "FPS: " << (1.0 / p_deltaTime) << '\n';

}


void Core::Render(double p_deltaTime) {
    // opengl things
    // hide dis shit {
    /*  RENDER TINGS */
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]); // tanggalin to
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // hide dis shit }

    cube_thing->Draw(shaderProgram, *camcam);

    glfwSwapBuffers(windowObj.ID);
}

void Core::Cleanup() {

    shaderProgram.Delete();

    glfwDestroyWindow(windowObj.ID);
    glfwTerminate();
}
