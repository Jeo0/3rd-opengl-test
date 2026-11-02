#pragma once

#include "GLFW/glfw3.h"
#include "core/InputFrame.h"

class InputHandler {
private:
    bool c_previousP = false;
    bool c_previousX = false;

    bool c_mouseCaptured = false;
    bool c_firstMouse = true;

    double c_lastMouseX = 0.0;
    double c_lastMouseY = 0.0;

    static bool IsDown(GLFWwindow* p_window, int p_key);

    static bool WasPressed(
        GLFWwindow* p_window,
        int p_key,
        bool& p_previousState
    );

public:
    InputFrame Poll(GLFWwindow* p_window);
};
