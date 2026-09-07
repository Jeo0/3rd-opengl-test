
#pragma once 
#include "gl/shaderClass.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

class Camera {
public:
    glm::vec3 c_position;
    // glm::vec3 c_orientation {glm::vec3(0.0, 0.0, -1.0)};
    // glm::vec3 c_up {glm::vec3(0.0, 1.0, 0.0)};
    glm::vec3 c_orientation {glm::vec3((double)0.0, (double)0.0, (double)-1.0)};
    glm::vec3 c_up {glm::vec3((double)0.0, (double)1.0, (double)0.0)};
    glm::mat4 c_cameraMatrix {glm::mat4(1.0)};

    bool c_firstClick {true};
    float c_sensitivity {50.0};
    float c_speed {100.0};

    int c_height, c_width;

    Camera(int p_height, int p_width, glm::vec3 p_position);
    ~Camera();

    // ====================
    void HandleInputs(GLFWwindow* p_windowID, double p_deltaTime);
    void UpdateMatrix(float p_FOVdeg, float p_nearPlane, float p_farPlane);
	void Matrix(Shader& p_shader, std::string p_uniform);
};
