#include "core/core.h"
#include "core/Texture.h"
#include "core/globals.h"
#include <filesystem>
#include <iostream>
#include <string_view>

// 3d#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"


Core::Core() 
    : windowObj(800, 800, "gademNoice", NULL, NULL, {0.01f, 0.24f, 0.45f, 1.0f}),
      shaderProgram("resource/Shaders/default.vert", "resource/Shaders/default.frag"),
      VBO1(vertices, sizeof(vertices)),
      EBO1(indices, sizeof(indices)) 
{
    Init();
}

Core::~Core() {
        Cleanup();
    }

void Core::Run() {
    // The Main Game/Render Loop
    while (!glfwWindowShouldClose(windowObj.ID)) {
        Update();
        Render(); // buffer swap
        glfwPollEvents(); // polling is a must after buffer swap
    }
}

void Core::Init() {
    VAO1.Bind();
    VBO1.Bind();
    EBO1.Bind();
                // buffer data containing vertices, layout, 
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);     // from globals: vertices
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // from globals: colors
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // from globals: textures
    
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // ============================
    // uniforms
    uniformID = glGetUniformLocation(shaderProgram.ID, "scale");


    // ============================
    // textures
    std::string parentDir = (std::filesystem::current_path().std::filesystem::path::parent_path()).string();
    std::string textureFile = "/git/resource/Textures/gadem.jpg";
    // std::string textureFile = "/git/resource/Textures/popcat.png";
    // std::string textureFile = "/git/resource/Textures/pak.jpg";
    std::cout << "opening dis ting -> " << parentDir << textureFile << std::endl;

    // usage of textures
    Texture simpleSquare(parentDir, textureFile, textureID, GL_TEXTURE0, 
                            GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    simpleSquare.TexUnit(shaderProgram, "tex0", 0);   // usage with uniform and whatever
}

void Core::Update() {
/*  LOGIC TINGS */
    // mouse input ++++============================== goback here
    windowObj.HandleMouseInput();
    
    shaderProgram.Activate();


    // 3d tings 
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);
    view = glm::translate(view, glm::vec3(0.0, -0.5, -2.0));

    proj = glm::perspective((float)(glm::radians(45.0)), (float)(windowObj.Height / windowObj.Width), 0.1f, 100.0f);
    
    int modelLoc = glGetUniformLocation(shaderProgram.ID, "Umodel");
    int viewLoc = glGetUniformLocation(shaderProgram.ID, "Uview");
    int projLoc = glGetUniformLocation(shaderProgram.ID, "Uprojection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));



    // make cursor scale up and down ting 
    for (int i = 0; i < 4; i++) {
        bgColor[i] += indexer;
    }

    if (bgColor[0] >= 1.0f || bgColor[0] <= 0.0f) {
        indexer *= -1.0f;
    }
}

void Core::Render() {
/*  RENDER TINGS */
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]); // tanggalin to 
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.Activate();
    glUniform1f(uniformID, bgColor[0]); // tanggalin to; the 'uniform scale' depends on the bgcolor[0]'s value which changes overtime
    
    glBindTexture(GL_TEXTURE_2D, textureID); // texture.Bind()

    // reset 
    VAO1.Bind();
    // glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(windowObj.ID);
}

void Core::Cleanup() {

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    glDeleteTextures(1, &textureID);
    shaderProgram.Delete();
    
    glfwDestroyWindow(windowObj.ID);
    glfwTerminate();
}
