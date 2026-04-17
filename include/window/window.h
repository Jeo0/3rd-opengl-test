#pragma once
#include "GLFW/glfw3.h"

class Window {
public:
    GLFWwindow* ID;
    Window(int mheight, int mlength, const char* mtitlebar, GLFWmonitor* mmonitor, GLFWwindow* mshare);
    ~Window();
};



