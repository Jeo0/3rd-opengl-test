
#pragma once 
#include "gl/shaderClass.h"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float3.hpp"

class Camera {
public:
    glm::vec3 c_position;
    glm::vec3 c_orientation {glm::vec3(0.0, 0.0, -1.0)};
    glm::vec3 c_up {glm::vec3(0.0, 1.0, 0.0)};

    bool c_firstClick {true};
    float c_sensitivity {50.0};
    float c_speed {2.0};
    float c_delta {0.0};

    int c_height, c_width;

    Camera(int pHeight, int pWidth, glm::vec3 pPosition);
    ~Camera();

    // ====================
    void HandleInputs(GLFWwindow* pWindowID, float deltaTime);
    void Matrix(float pFOVdeg, float pNearPlane, float pFarPlane, Shader& pShader, const char* pUniform);
};
