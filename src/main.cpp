
// https://docs.google.com/spreadsheets/d/1zJyXv94MFD7IR9agFAzWmw3_cmge6CDwsUQ2P-K-bjk/edit?usp=sharing
// https://drive.google.com/drive/folders/1UMsSN-CSXIbWKanc4ryfAqTwIto89cHD?usp=sharing

#include <iostream>
#include <fstream>
#include <vector>
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include "shaderClass.h"

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
		-0.0,	0.5,	0.,			// top mid
		0.5,	-0.5,	0.,			// bot right
		-0.5,	.5,		0.,			// top left 
		0.3,	0.8,	0.			// top right
	};

	// reduce redundancy with element buffer objects
	GLuint indices[] = {
		0,1,2,		// left triangle
		0,1,3		// right triangle


	};

	/*
	// OPENGL
	// a buffer for the triangle
	GLfloat vertices[] = {
		-0.0,	0.5,	0.,			// top mid
		0.5,	-0.5,	0.,			// bot right
		-0.5,	.5,		0.,			// top left 
		0.3,	0.8,	0.			// top right
		0.8,	0.,		0.			// rightmost
	};

	// reduce redundancy with element buffer objects
	GLfloat indices[] = {
		0,1,2,		// left triangle
		0,1,3		// right triangle
		1,4,3


	};
	*/

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



	///////////////////////////////////////////////////////////
	///	location AA1
	///
	/// this gets reduced using shaderclass 
	/// from create shader
	/// - shader source (vert, frag)
	/// - compile (vert, frag)
	/// - create program
	/// - attach program (vert, frag)
	/// - link program
	/// - delete shader (vert, frag)
	/// 
	///
	///////////////////////////////////////////////////////////


	// always refer to shaderProgram with its ID
	Shader shaderProgram("resource/Shaders/default.vert", "resource/Shaders/default.frag");
	






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
	//
	// Index buffer = element 
	GLuint VAO, VBO, EBO;			// array of references

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// binding
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// make an object the current data to be processed
	
	// storing the vertices to the current binded object (VBO and EBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);			// VBO
																						//
	
	// same happens for the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);												
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// EBO

	// configuration: let opengl know how to read VBO
	// a vertex attribute pointer allows the ability 
	// to communicate with a vertex shader from the outside
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// position @ 0 
	glEnableVertexAttribArray(0);													// position @ 0
	
	// unbind the buffer to be safe from accidental changes (for good practice)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);






	
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
		glUseProgram(shaderProgram.ID);
		glBindVertexArray(VAO);				// not necessary since we only have 1 object & 1 vao
											// (it it the usual practice everytime regardless)
		//glDrawArrays(GL_TRIANGLES, 0, 3);	// specify the primitives (triangles)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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


		// // print
		// std::cout << "array[iii] {";
		// for(int iii = 0; iii<4; iii++){
		// 	std::cout << array[iii] << ", ";
		// }
		// std::cout << "}" << std::endl;




		// let glfw handle window events (resize, close, minimize, move)
		glfwPollEvents();
		
	}


	// cleaning 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram.ID);

	glfwDestroyWindow(window);
	glfwTerminate();
}
