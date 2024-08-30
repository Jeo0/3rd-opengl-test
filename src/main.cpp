
// https://docs.google.com/spreadsheets/d/1zJyXv94MFD7IR9agFAzWmw3_cmge6CDwsUQ2P-K-bjk/edit?usp=sharing
// https://drive.google.com/drive/folders/1UMsSN-CSXIbWKanc4ryfAqTwIto89cHD?usp=sharing
/*
 *--------- part 1
[check]  2 mobile processors
[ ] desktop processor
[ ] server processor

---------- part 2.1
[check] datasheet ng ipad?? wala ako makita, pero eto meron https://www.cpu-world.com/CPUs/A5/Apple-A5%20(iPad).html
[d ko sure kung meron man etong ipad] heat sinks / cooling solutions for each processor

----------- part 2.2
[ ] datasheet ng phone ni kobi
[ ] heat sinks / cooling solution nung processor


-------------- part 3
[ ] antistaticwriststrap
[check] thermal paste
*/

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
	// a buffer for the triangle
	GLfloat vertices[] = {
		-0.4,	-0.1,	0.,
		0.5,	-0.5,	0.,
		-0.5,	.5,		0.
	};

	// object initialization 
	// window
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




	// vertex and fragment shader objects
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragmentShader);


	// "wrap the vertex and fragment shader objects up"
	// using shader program
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// wrap them up by linking
	glLinkProgram(shaderProgram);



	// we can now safely delete the shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	






	///////////////////////////////////////////////////////////
	///
	///
	/// render proper
	///
	///
	///////////////////////////////////////////////////////////

	// tell opengl to render static (r,g,b,a) 
	glClearColor(0.01f, 0.24f, 0.45f, 1.0f);

	// specify that we want to use the command on the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// tell the window to swap the processed opengl buffer (the above) 
	// with the front buffer which is just empty white screen
	glfwSwapBuffers(window);


	// processing vertices
	// telling opengl what to do with the specified vertices
	// transferring data in buffers
	//
	//
	// VAOs
	// why: opengl does not know where the buffer data is located
	// it stores pointers to VBOs
	// and it tells opengl how to interpret them
	// it also allows for quicker switching of different VBOs.
	// also generate VAOs first before VBOs
	//
	GLuint VAO, VBO;			// array of references

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// binding
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// make an object the current data to be processed
	
	// storing the vertices to the current binded object (VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// configuration: let opengl know how to read VBO
	// a vertex attribute pointer allows the ability 
	// to communicate with a vertex shader from the outside
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// position @ 0 
	glEnableVertexAttribArray(0);													// position @ 0
	
	// unbind the buffer to be safe from accidental changes (for good practice)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);






	
	// for the window (background) flashing BW
	double array[4] = {0,0,0,0};
	double indexer = 0.001;
	bool state = true;			// true = ascend;	false = descend

	while(!glfwWindowShouldClose(window)){

		//////////////////////////////////////////
		// make white to black to white transition
		//////////////////////////////////////////
		glClearColor(array[0], array[1], array[2], array[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		//////////////////////////////////////////
		/// triangle
		//////////////////////////////////////////
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);				// not necessary since we only have 1 object & 1 vao
											// (it it the usual practice everytime regardless)
		glDrawArrays(GL_TRIANGLES, 0, 3);	// specify the primitives (triangles)
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




		// let glfw handle window events (resize, close, minimize, move)
		glfwPollEvents();
		
	}


	// cleaning 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();
}
