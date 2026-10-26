
#pragma once 
#include "gl/shaderClass.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

class Camera {
public:
        // debug view, runtime tweakable with the keys in Scene::Update
        float c_fovDeg     = 45.0f;
        float c_edgeWidth  = 2.0f;                      // px, [ and ]
        float c_pointSize  = 10.0f;                     // px, - and =
        bool  c_showPoints = true;                      // P
        bool  c_pointsXray = false;                     // X, dots visible through the surface
        glm::vec3 c_edgeColor  {0.05f, 0.05f, 0.05f};
        glm::vec3 c_pointColor {1.0f, 0.85f, 0.1f};
        bool  c_keyWasDown[2] = {false, false};         // edge detection for the toggles

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


// bedroom:
// wardrobe 1 
// ottoman 1
// radiator 1
//
// hallway:
// desk 1 
// radiator 1 
//
// kitchen:
// kitchecn acceececrosy 4 
//
// living room:
// sofa 1 
// table 1 
// radaitor 1 
//
// office:
// office chair 1 
// pc 1 
// monitor 1 
// radiator 1 
//
// basement:
// misc  1 
// rug 1 
//
// bathroom: 
// trashbin 1
//
// dining room: 
// radiator 1 
//
// bathroom :
// trashbin 1  
//
// bedroom:
// ottoman 1 
// radiator 1 
//
// dressing room:
// mirror 1 
// radiator 1 
//
// bathroom:
// trashbin 1 
// light switch 1 
//
// guest room:
// radiator 1 
//
// hallway:
// desk 1 
// cabinet 1 
// radiator 1 
// rug 1 
//
// hallway: 
// desk 1 
// cabinet 1 
// radiator 1 
// ceiling light 1 
// light switch 1 
// rug 1 
//
// living room: 
// sofa 1 
// cabinet 3 
// radiator 1 
//
