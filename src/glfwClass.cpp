#include "glfwClass.h"
#include <cerrno>
#include <iostream>

GLFW::GLFW(int height, int length, const char* titlebar, GLFWmonitor* monitor, GLFWwindow* share){
    ID = glfwCreateWindow(height, length, titlebar, monitor, share);

	if(ID == NULL){
		std::cerr << "fail to create glfw window" << std::endl;
		glfwTerminate();
        throw(errno);
	}
}