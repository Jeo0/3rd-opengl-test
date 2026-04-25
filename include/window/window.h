#pragma once
#include "GLFW/glfw3.h"
#include <array>

class Window {

public:
    GLFWwindow* ID;
    Window(int mheight, int mlength, const char* mtitlebar, GLFWmonitor* mmonitor, GLFWwindow* mshare, std::array <GLfloat, 4> mstdBgColor);
    ~Window();
};



