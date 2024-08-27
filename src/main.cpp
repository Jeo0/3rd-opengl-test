
#include <iostream>
#include <fstream>
#include <vector>
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

// vertex and shader source
constexpr auto vertShaderSrc = R"(#version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    })";

constexpr auto fragShaderSrc = R"(#version 330 core
	out vec4 FragColor;
	void main() {
    	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	})";

int main()
{

	///////////////////////////////////////////////////////////
	///
	///
	/// window boilerplate
	///
	///
	///////////////////////////////////////////////////////////
	glfwInit();

	// set the settings which opengl version will glfw use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// OPENGL
	// a buffer
	//GLfloat vertices[] = {

	//};

	// object initialization 
	GLFWwindow* window = glfwCreateWindow(800,800, "titlebar", NULL, NULL);
	if(window == NULL){
		std::cerr << "fail to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// use the initialized window object
	// context is probably equal to an object that holds the whole of opengl
	// but it is a bit more abstract than that
	// as it can hold and do many things
	glfwMakeContextCurrent(window);




	///////////////////////////////////////////////////////////
	///
	///
	/// window subboilerplate
	///
	///
	///////////////////////////////////////////////////////////


	// tell glad to load the needed configurations for opengl
	gladLoadGL();

	// tell opengl that we want to set the viewport (the are where opengl will render in)
	// -- from	0,0 	:	bot-left window
	// -- to 	800,100	:	top-right window
	glViewport(0,0, 800, 800);

	// tell opengl to render static (r,g,b,a) 
	glClearColor(0.01f, 0.24f, 0.45f, 1.0f);

	// specify that we want to use the command on the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// tell the window to swap the processed opengl buffer (the above) 
	// with the front buffer which is just empty white screen
	glfwSwapBuffers(window);



	
	double array[4] = {0,0,0,0};
	double indexer = 0.001;
	bool state = true;			// true = ascend;	false = descend

	while(!glfwWindowShouldClose(window)){
		// let glfw handle window events (resize, close, minimize, move)
		glfwPollEvents();

		//////////////////////////////////////////
		// make white to black to white transition
		//////////////////////////////////////////
		glClearColor(array[0], array[1], array[2], array[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

		// determine when to descend and ascend
		if(state == false){
			for(int iii = 0; iii<4; iii++){
				array[iii] -= indexer;
			}

		}
		else if(state == true){ 
			for(int iii = 0; iii<4; iii++){
				array[iii] += indexer;
			}
		}


		// set state
		if(array[0] >= 1.0f){
			state = false;
		}
		else if(array[0] <= 0.0f){
			state = true;
		}


		// print
		std::cout << "array[iii] {";
		for(int iii = 0; iii<4; iii++){
			std::cout << array[iii] << ", ";
		}
		std::cout << "}" << std::endl;
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
