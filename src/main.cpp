#include "core/core.h"
#include "core/globals.h"


int main()
{
    Core nani;
    nani.Run();
}

// int nrAttributes;
// glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
// std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// object initialization 
	// window
//     int mwidth = 800, mheight = 800;
//     std::string mtitle = "title";
//     std::array <GLfloat, 4> stdBgColor = {0.01, 0.24, 0.45, 1.0};
//     Window windowObj(mwidth, mheight, mtitle.c_str(), NULL, NULL, stdBgColor);
//
//
// 	///////////////////////////////////////////////////////////
// 	///
// 	/// render proper
// 	///
// 	///////////////////////////////////////////////////////////
//
//
//
// 	// processing vertices
// 	// telling opengl what to do with the specified vertices
// 	// transferring data in buffers
// 	Shader shaderProgram("resource/Shaders/default.vert", "resource/Shaders/default.frag");
// 	//Shader shaderProgram("default.vert", "default.frag");
//
// 	VAO VAO1;
// 	VAO1.Bind();		// instead put it here; remember the constructor for VBO and EBO already has bind function
// 	VBO VBO1(vertices, sizeof(vertices));		// generate, bind, and link it to vertices
// 	EBO EBO1(indices, sizeof(indices));			// generate, bind, and link to indices
//
// 	// linking 
// 	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// position @ 0 	// VBO
// 	// glEnableVertexAttribArray(0);													// position @ 0		// VBO
// 	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
// 	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)) );
// 	VAO1.Unbind();
// 	VBO1.Unbind();
// 	EBO1.Unbind();
//
// 	// we can access uniforms here	
// 	// scale is being accessed here
// 	GLuint uniformID = glGetUniformLocation(shaderProgram.ID, "scale");
//
//
// 	// for the window (background) flashing BW
// 	double array[4] = {0,0,0,0};
// 	float indexer = 0.001;
//
// 	while(!glfwWindowShouldClose(windowObj.ID)){
//
// 		//////////////////////////////////////////
// 		// make white to black to white transition
// 		//////////////////////////////////////////
// 		glClearColor(array[0], array[1], array[2], array[3]);
// 		glClear(GL_COLOR_BUFFER_BIT);
//
// 		//////////////////////////////////////////
// 		/// triangle
// 		//////////////////////////////////////////
// 		shaderProgram.Activate();		// when accessing a uniform, make sure its after the shader program compilation
// 		// assign some value for the uniform we accessed named "scale"
// 		glUniform1f(uniformID, array[0]);		// only 1 float
// 		// also it's 150% bigger
//
//
//
//
// 		VAO1.Bind();
// 		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 		glfwSwapBuffers(windowObj.ID);
//
//
// 		// determine when to descend and ascend
// 		for(int iii = 0; iii<4; iii++){
// 			array[iii] += indexer;
// 		}
// 		// set state
// 		if(array[0] >= 1.0f)
// 			indexer *= -1.0f;
// 		else if (array[0] <= 0.0f)
// 			indexer *= -1.f;
//
//
// 		// let glfw handle window events (resize, close, minimize, move)
// 		glfwPollEvents();
//
// 	}
//
//
// 	// cleaning 
// 	VAO1.Delete();
// 	VBO1.Delete();
// 	EBO1.Delete();
// 	shaderProgram.Delete();
//
// 	glfwDestroyWindow(windowObj.ID);
// 	glfwTerminate();
// }
