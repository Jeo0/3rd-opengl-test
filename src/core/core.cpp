#include "core/core.h"
#include "GLFW/glfw3.h"
#include "core/Texture.h"
#include "core/globals.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <string_view>

// 3d#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <glm/ext/matrix_clip_space.hpp>

#include "Camera.h"

Core::Core()
    : windowObj(800, 800, "gademNoice", NULL, NULL,
                {0.01f, 0.24f, 0.45f, 1.0f}),
      shaderProgram("resource/Shaders/default.vert",
                    "resource/Shaders/default.frag"),
      VBO1(vertices), EBO1(indices) {
  Init();
}

Core::~Core() { Cleanup(); }

void Core::Run() {
  // The Main Game/Render Loop
  while (!glfwWindowShouldClose(windowObj.ID)) {
    float mDeltaTime = cLimiter.StartFrame();
    Update(mDeltaTime);
    Render();         // buffer swap
    glfwPollEvents(); // polling is a must after buffer swap

    cLimiter.EndFrame();
  }
}

void Core::Init() {

  // ============================
  // uniforms
  uniformID = glGetUniformLocation(shaderProgram.ID, "scale");

  // ============================
  // textures
  std::string parentDir =
      (std::filesystem::current_path().std::filesystem::path::parent_path())
          .string();
  std::string textureFile = "/git/resource/Textures/gadem.jpg";
  // std::string textureFile = "/git/resource/Textures/popcat.png";
  // std::string textureFile = "/git/resource/Textures/pak.jpg";
  // std::cout << "USERINFO: opening dis ting -> " << parentDir << textureFile
  // << std::endl;

  // usage of textures
    Texture textextures[] {
        Texture(parentDir, textureFile, textureID, GL_TEXTURE0,
                       GL_RGBA, GL_UNSIGNED_BYTE);
    };
  simpleSquare.TexUnit(shaderProgram, "tex0",
                       0); // usage with uniform and whatever

  // ============================
  // open gl opengl settings
  glEnable(GL_DEPTH_TEST);

  // ============================
  // object instantiations
  camcam = std::make_unique<Camera>(windowObj.Height, windowObj.Width,
                                    glm::vec3(0.0f, 0.0f, 2.0f));
  setset = std::make_unique<Settings>(cLimiter);

    cube_thing = std::make_unique<Mesh>(vertices, indices, textextures);
}

void Core::Update(double pDeltaTime) {
  /*  LOGIC TINGS */
  // settings
  setset->LoadSettings(cLimiter);

  // mouse input ++++============================== goback here
  windowObj.HandleMouseInput();
  camcam->HandleInputs(windowObj.ID, pDeltaTime); // goback here
  // try have a pointer in the core class pointing to a camera object
  // so we can use camcam
  camcam->Matrix(90.0, 0.1, 100.0, shaderProgram, "camMatrix");

  // debug ===================== REMOVE
  std::cout << "FPS: " << (1.0 / pDeltaTime) << '\n';

  shaderProgram.Activate();

  // 3d tings
  glm::mat4 model = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);
  glm::mat4 proj = glm::mat4(1.0);

  view = glm::translate(view, glm::vec3(0.0, -0.5, -2.0));
  proj = glm::perspective((float)(glm::radians(45.0)),
                          (float)(windowObj.Height / windowObj.Width), 0.1f,
                          100.0f);

  int modelLoc = glGetUniformLocation(shaderProgram.ID, "Umodel");
  int viewLoc = glGetUniformLocation(shaderProgram.ID, "Uview");
  int projLoc = glGetUniformLocation(shaderProgram.ID, "Uprojection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));


}

void Core::Render() {
  /*  RENDER TINGS */
  glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]); // tanggalin to
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.Activate();
  glUniform1f(uniformID,
              bgColor[0]); // tanggalin to; the 'uniform scale' depends on the
                           // bgcolor[0]'s value which changes overtime

  glBindTexture(GL_TEXTURE_2D, textureID); // texture.Bind()

  // reset
  VAO1.Bind();
  // glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]),
  // GL_UNSIGNED_INT, 0);
  glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT,
                 0);

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
