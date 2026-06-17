
#pragma once 
#include "gl/shaderClass.h"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float3.hpp"

class Camera {
public:
    glm::vec3 cPosition;
    glm::vec3 cOrientation {glm::vec3(0.0, 0.0, -1.0)};
    glm::vec3 cUp {glm::vec3(0.0, 1.0, 0.0)};

    bool cFirstClick {true};
    float cSensitivity {100.0};
    float cSpeed {4.0};

    int cHeight, cWidth;

    Camera(int pHeight, int pWidth, glm::vec3 pPosition);
    ~Camera();

    // ====================
    void HandleInputs(GLFWwindow* pWindowID);
    void Matrix(float pFOVdeg, float pNearPlane, float pFarPlane, Shader& pShader, const char* pUniform);
};
