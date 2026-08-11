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
#include "glm/detail/qualifier.hpp"
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
    // open gl opengl settings
    glEnable(GL_DEPTH_TEST);

    // ============================
    // object instantiations
    //
    camcam = std::make_unique<Camera>(windowObj.Height, windowObj.Width, glm::vec3(0.0f, 0.0f, 2.0f));
    setset = std::make_unique<Settings>(cLimiter);

    // mesh handles the texture
    std::string textureFolder = "/git/resource/Textures/";
    AddObjectToScene(g_quadVertices, g_quadIndices, (textureFolder + "popcat.png"));

    shaderProgram.Activate();
}


void Core::Update(double p_deltaTime) {
    /*  LOGIC TINGS */
    // settings
    setset->LoadSettings(cLimiter);
    // shaderProgram.Activate();

    // debug REMOVE
    debug_time_iii += (p_deltaTime);
    if(debug_time_iii > 2.0 && !(debug_time_iii_flag)) {
        std::string textureDir = "/git/resource/Textures/";
        AddObjectToScene(g_vertices, g_indices, (textureDir + "gadem.jpg"), glm::vec3(1.3f, -1.0f, 0.4f));
        debug_time_iii_flag = true;
        std::cout << "loaded" << std::endl;
    }

    // mouse input ++++============================== goback here
    windowObj.HandleMouseInput();
    camcam->HandleInputs(windowObj.ID, p_deltaTime); 
    camcam->UpdateMatrix(45.0, 1.0, 100.0);

    // debug ===================== REMOVE
    std::cout << "FPS: " << (1.0 / p_deltaTime) << '\n';

}


void Core::Render(double p_deltaTime) {
    /*  RENDER TINGS */
    // opengl things
    // hide dis shit {
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]); // tanggalin to
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // hide dis shit }

    // draw all the objects now
    for(auto& iii_object: c_sceneObjects){
        iii_object->Draw(shaderProgram, *camcam);
    }

    glfwSwapBuffers(windowObj.ID);
}

void Core::Cleanup() {

    shaderProgram.Delete();

    glfwDestroyWindow(windowObj.ID);
    glfwTerminate();
}

void Core::AddObjectToScene(std::vector<Vertex>& p_vertices,
                           std::vector<GLuint>& p_indices,
                           const std::string& p_textureFile,
                           const glm::vec3& p_position) {

    std::string parentDir =
        (std::filesystem::current_path().std::filesystem::path::parent_path()).string();

    // debug REMOVE
    std::cout << "INFO dir: " << (parentDir + p_textureFile) << std::endl;

    std::vector<std::unique_ptr<Texture>> m_textures;
    m_textures.push_back(
            std::make_unique<Texture>((parentDir + p_textureFile), "diffuse_tex_type", 0, GL_RGBA, GL_UNSIGNED_BYTE)
            );

    auto m_newMesh = std::make_unique<Mesh>(p_vertices, p_indices, std::move(m_textures));
    m_newMesh->SetPosition(p_position);
    c_sceneObjects.push_back(std::move(m_newMesh));

}
