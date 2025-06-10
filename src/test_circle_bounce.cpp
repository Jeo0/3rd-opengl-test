// this project replaced the WHAT/prev/previousmain.what_cleaner_recent

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>
#include <iostream>
#include <vector>

GLFWwindow *StartGLFW();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInputs(GLFWwindow *window);
std::vector<float> GenerateCircleVertices(float centerX, float centerY,
                                          float radius, int circleResolution);

constexpr float screenWidth = 800.0f;
constexpr float screenHeight = 600.0f;

int main() {
  GLFWwindow *window = StartGLFW();

  // Shader shaderProgram("resource/Shaders/default.vert",
  // "resource/Shaders/default.frag");

  // for a circle
  float centerX = screenWidth / 2.0f;
  float centerY = screenHeight / 2.0f;
  float radius = 123.0f;
  int circleResolution = 100;

  while (!glfwWindowShouldClose(window)) {

    processInputs(window); // inputs

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set to some bluish green

    glfwSwapBuffers(
        window); // present the processed things to the buffer for the next loop
    glfwPollEvents(); // inputs
  }
}

GLFWwindow *StartGLFW() {
  if (!glfwInit()) {
    std::cerr << "failed to initialize glfw lmao" << std::endl;
    glfwTerminate();
    return nullptr;
  }

  // set the settings
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  // intialize the object itself
  GLFWwindow *window = glfwCreateWindow((int)screenWidth, (int)screenHeight,
                                        "what_simulator", NULL, NULL);
  glfwMakeContextCurrent(window);

  // load glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }

  // for handling window resizing
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

// for handling window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInputs(GLFWwindow *window) {
  // get out of loop once we press escape key
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

std::vector<float> GenerateCircleVertices(float centerX, float centerY,
                                          float radius, int circleResolution) {
  std::vector<float> vertices{centerX, centerY};
  float centerXNDC = (centerX / screenWidth) * 2.0 - 1.0;
  float centerYNDC = (centerY / screenHeight) * 2.0 - 1.0;

  for (int iii = 0; iii <= circleResolution; iii++) {
    // float angle = 2.0 * 3.141592653589 * ((float)iii / circleResolution);
    float angle =
        2.0f * 3.141592653589 * (static_cast<float>(iii) / circleResolution);
    float x = centerX + cos(angle) * radius;
    float y = centerY + sin(angle) * radius;

    // Normalize to NDC
    float xNDC = (x / screenWidth) * 2.0f - 1.0f;
    float yNDC = (y / screenHeight) * 2.0f - 1.0f;

    // glVertex2f(x, y);
    // glVertex2f(xNDC, yNDC);
    vertices.push_back(xNDC);
    vertices.push_back(yNDC);
  }

  return vertices;
}
