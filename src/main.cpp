
// https://docs.google.com/spreadsheets/d/1zJyXv94MFD7IR9agFAzWmw3_cmge6CDwsUQ2P-K-bjk/edit?usp=sharing
// https://drive.google.com/drive/folders/1UMsSN-CSXIbWKanc4ryfAqTwIto89cHD?usp=sharing

#include <iostream>
#include <fstream>
#include <vector>
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "glfwClass.h"


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
		// vertex 3;			color 3
		-0.0,	0.5,	0.,		.8f,	.1f,	.1f,			// top mid
		0.5,	-0.5,	0.,		.2f,	.8f,	.1f,			// bot right
		-0.5,	.5,		0.,		.2f,	.14f,	.8f,			// top left 
		0.3,	0.8,	0.,		.9f,	.5f,	1.f			// top right
	};

	// reduce redundancy with element buffer objects
	GLuint indices[] = {
		0,1,2,		// left triangle
		0,1,3		// right triangle


	};


	// object initialization 
	// window
	GLFW window(800,800, "titlebar", NULL, NULL);
	GLFWwindow* windowObj = window.ID;

	// use the initialized window object
	// context is probably equal to an object that holds the whole of opengl
	// but it is a bit more abstract than that
	// as it can hold and do many things
	glfwMakeContextCurrent(windowObj);




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
	//Shader shaderProgram("default.vert", "default.frag");
	






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
	glfwSwapBuffers(windowObj);


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
	VAO VAO1;
	VAO1.Bind();		// instead put it here; remember the constructor for VBO and EBO already has bind function
	VBO VBO1(vertices, sizeof(vertices));		// generate, bind, and link it to vertices
	EBO EBO1(indices, sizeof(indices));			// generate, bind, and link to indices

	// linking 
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// position @ 0 	// VBO
	// glEnableVertexAttribArray(0);													// position @ 0		// VBO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)) );
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	
	// we can access uniforms here	
	// scale is being accessed here
	GLuint uniformID = glGetUniformLocation(shaderProgram.ID, "scale");






	
	// for the window (background) flashing BW
	double array[4] = {0,0,0,0};
	float indexer = 0.001;

	while(!glfwWindowShouldClose(windowObj)){

		//////////////////////////////////////////
		// make white to black to white transition
		//////////////////////////////////////////
		glClearColor(array[0], array[1], array[2], array[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		//////////////////////////////////////////
		/// triangle
		//////////////////////////////////////////
		shaderProgram.Activate();		// when accessing a uniform, make sure its after the shader program compilation
		// assign some value for the uniform we accessed named "scale"
		glUniform1f(uniformID, array[0]);		// only 1 float
		// also it's 150% bigger
	



		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(windowObj);


		// determine when to descend and ascend
		for(int iii = 0; iii<4; iii++){
			array[iii] += indexer;
		}
		// set state
		if(array[0] >= 1.0f)
			indexer *= -1.0f;
		else if (array[0] <= 0.0f)
			indexer *= -1.f;


		// let glfw handle window events (resize, close, minimize, move)
		glfwPollEvents();
		
	}


	// cleaning 
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(windowObj);
	glfwTerminate();
}
