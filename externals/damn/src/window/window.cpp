#include "GLAD/glad.h"
#include "glm/ext/matrix_transform.hpp"
#include "window/window.h"
#include <cerrno>
#include <iostream>


// private 
// ++++=========================== goback here
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // if (key == GLFW_KEY_E && action == GLFW_PRESS)
        //
}

// public
Window::Window(int mheight, int mlength, const char* mtitlebar, GLFWmonitor* mmonitor, GLFWwindow* mshare, std::array <GLfloat, 4> mstdBgColor){
    // 1 settings 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2 create window
    ID = glfwCreateWindow(mheight, mlength, mtitlebar, mmonitor, mshare);

	if(ID == NULL){
		std::cerr << "fail to create glfw window" << std::endl;
		glfwTerminate();
        throw(errno);
	}
    glfwMakeContextCurrent(ID);
    glfwSwapInterval(1); // vsync on by default; see SetVSync()
    
    // 3 glad 
    // tell glad to load the needed configurations for opengl
    // gladLoadGL();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // 4 opengl settings 
    // tell opengl that we want to set the viewport (where opengl will render in)
    // -- from	0,0 	:	bot-left window
    // -- to 	800,800	:	top-right window
    Height = mheight;
    Width = mlength;
    glViewport(0,0, Height, Width);

	// tell opengl to render static (r,g,b,a) 
    glClearColor(mstdBgColor[0], mstdBgColor[1], mstdBgColor[2], mstdBgColor[3]);

	// specify that we want to use the command on the color buffer
	glClear(GL_COLOR_BUFFER_BIT );

	// tell the window to swap the processed opengl buffer (the above) 
	// with the front buffer which is just empty white screen
	glfwSwapBuffers(ID);
}


Window::~Window() {
    glfwDestroyWindow(ID);
    glfwTerminate();
}

// ++++===================== goback here
void Window::HandleMouseInput(){
    //glfwSetKeyCallback(ID, key_callback);
}

void Window::SetVSync(bool enabled) {
    // glfwSwapInterval is itself cross-platform: on Windows it's WGL_EXT_swap_control,
    // on Linux/Wayland (Hyprland included) it goes through EGL's eglSwapInterval, which
    // GLFW maps onto the compositor's frame-callback presentation timing. Either way,
    // this only affects how SwapBuffers blocks -- it never touches FrameLimiter's clock
    // or sleep, so an explicit FPS cap below the refresh rate still works the same
    // regardless of this setting.
    glfwSwapInterval(enabled ? 1 : 0);
}
