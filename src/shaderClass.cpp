#include "shaderClass.h"


std::string get_file_contents(const char* filename){
    std::ifstream in(filename, std::ios::binary);
    if (!in){
        std::cerr << "unable to open file " <<  filename << std::endl;
        throw(errno);
    }
    else{
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0,std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
}        


Shader::Shader(const char* vertFile, const char* fragFile){
    // get those in a string
    std::string vertexSrcCode = get_file_contents(vertFile);
    std::string fragmentSrcCode = get_file_contents(fragFile);      

    // however, we want it to be charater arrays
    const char* vertexSource = vertexSrcCode.c_str();
    const char* fragmentSource = fragmentSrcCode.c_str();



	///////////////////////////////////////////////////////////
	///	from main.cpp
    /// location AA1
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
	// vertex and fragment shader objects
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX"); // error handling

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT"); // error handling


	// "wrap the vertex and fragment shader objects up"
	// using shader program
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// wrap them up by linking
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");



	// we can now safely delete the shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


void Shader::Activate(){
	glUseProgram(ID);
}

void Shader::Delete(){
	glDeleteProgram(ID);
}


void Shader::compileErrors(unsigned int shader, const char* type){
	GLint hasCompiled;
	char infoLog[1024];
	if(type != "PROGRAM"){	// compilation
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if(hasCompiled == GL_FALSE){
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
		}

	} else{		// linking 
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if(hasCompiled == GL_FALSE){
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}

	}
}