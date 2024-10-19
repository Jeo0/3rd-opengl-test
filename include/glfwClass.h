#pragma once
#include "GLFW/glfw3.h"

class GLFW {
public:
    GLFWwindow* ID;
    GLFW(int height, int length, const char* titlebar, GLFWmonitor* monitor, GLFWwindow* share);
};