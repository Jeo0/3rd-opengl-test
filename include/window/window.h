#pragma once
#include "GLFW/glfw3.h"
#include <array>

class Window {
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    GLFWwindow* ID;
    Window(int mheight, int mlength, const char* mtitlebar, GLFWmonitor* mmonitor, GLFWwindow* mshare, std::array <GLfloat, 4> mstdBgColor);
    ~Window();

    void HandleMouseInput();
};



