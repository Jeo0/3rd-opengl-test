#pragma once
#include "glm/vec2.hpp"

// ==============================
// camera layer
struct CameraInput {
    float forward = 0.0f;
    float right = 0.0f;
    float vertical = 0.0f;

    bool sprint = false;
    bool lookActive = false;

    glm::vec2 mouseDelta{0.0f};
};

struct InputFrame {
    CameraInput camera;

    // continuous actions: -1, 0, or +1
    float edgeWidthAxis = 0.0f;
    float pointSizeAxis = 0.0f;

    // one-frame actions
    bool togglePoints = false;
    bool togglePointXray = false;
    bool toggleUi = false;  // 
};


// ==============================
// ImGUI layer
struct UiCapture {
    bool mouse = false;
    bool keyboard = false;
};

