#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <vector>
//#define GLFW_DLL

void glfwWindowHints();
void gladInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow* window);
const short int SCR_WIDTH = 640;
const short int SCR_HEIGHT = 400;
/*	 trying to store the shader source into a separate file
 *	 and accessing it using file streams
 *	 but not working yet
std::ifstream ifs("../include/verts/data.vert");
std::string content( 	(std::istreambuf_iterator<char>(ifs)),
						(std::istreambuf_iterator<char>()	));
char *vertShaderSrc = "#version 330 core\n"
"layout (location = 0) in vec3 aPos; void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }\0";
*/

// trying to change this dynamically
//std::vector<float> vertices{	0.0,	0.5,	0.0,
float	vertices[]	= {	0.0,	0.5,	0.0,	// top left
						-0.5,	0.0,	0.0,	// left
						0.5,	-0.5,	0.0,	// bot right
						0.5,	0.0,	0.0};	// right
unsigned int indices[] = {	0,1,2,
							1,2,3};

/*
const char* vertShaderSrc = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragShaderSrc = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
    "	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"} \0";
*/

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


int previous_main(){
	if(!glfwInit()) { std::cerr<<"failed to initialize"<<std::endl; return -1;}
	glfwWindowHints();

	/*	 trying to store the shader source into a separate file
	 *	 and accessing it using file streams
	 *	 but not working yet
	for(char x: content)
		std::cout << x << std::endl;
	char** p = &vertShaderSrc;
	while(p != NULL){
		std::cout << *p << std::endl;
		p++;
	}
	*/


	

	// intiialize window
	GLFWwindow* window;
	window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"title", NULL, NULL);
	if(!window) {std::cerr<<"failed to create window" << std::endl; glfwTerminate(); return -1;}
	glfwMakeContextCurrent(window);

	

	// intiialize opengl function ptrs
	gladInit();
	// intiialize viewport for opengl
	// when resizing the window, resize the glViewport
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
	

	// generating buffers and binding the location of that buffer 
	// glarraybuffer is for vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);												// step 0: copy the vertices array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);		// in a buffer for opengl 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// step 1: set the vertex attribute ptrs
	glEnableVertexAttribArray(0);

	/* 		alternative for handling mutliple vbos	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);				// step 0 : bind the array object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// step 1: copy our vertices array in a buffer for opengl to use
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);		// in a buffer for opengl 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// set the vertex attribute ptrs
	glEnableVertexAttribArray(0);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// set the vertex attribute ptrs
	glEnableVertexAttribArray(0);
			


	/////////////////////////////////////////////////////////////////////////////////////////////
	///	shader program and compilation
	// for the vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(-1);
	}
	success = NULL; 
	for(int iii=0; iii<512; iii++)	infoLog[iii] = NULL;


	// for fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(-1);
	}
	success = NULL; 
	for(int iii=0; iii<512; iii++)	infoLog[iii] = NULL;

	// combining both shader into 1
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(-1);
	}

	glUseProgram(shaderProgram);
	glClear(GL_COLOR_BUFFER_BIT);								// reset the render
	glfwSwapBuffers(window); 									// Swap front and back buffers 



	/////////////////////////////////////////////////////////////////////////////////////////////
	// main loop
	while(!glfwWindowShouldClose(window)){
		

		// process inputs
		processInput(window);

		// render
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


		glClear(GL_COLOR_BUFFER_BIT);								// reset the render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);						// bg
		glDrawArrays(GL_TRIANGLES, 0, 3);							// triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);		// rect
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		// Poll for and process events 
		glfwPollEvents();

		//glBindVertexArray(0);		// reset the bind

		glfwSwapBuffers(window); // Swap front and back buffers 
	}

	glfwTerminate();
	return 0;


}
void glfwWindowHints(){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		// 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		// 6
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void gladInit(){
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}    
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
} 
void processInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
