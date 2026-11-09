#pragma once
#include "GLFW/glfw3.h"
#include <array>

class Window {
    // REMOVE LATER
// private:
//     void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    GLFWwindow* ID;
    Window(int mheight, int mlength, const char* mtitlebar, GLFWmonitor* mmonitor, GLFWwindow* mshare, std::array <GLfloat, 4> mstdBgColor);
    ~Window();


    int c_height = 0, c_width = 0;

    bool PollResize();
    bool IsMinimized() const {return (c_height == 0) || (c_width == 0); };

    // REMOVE LATER
    // void HandleMouseInput();
};



