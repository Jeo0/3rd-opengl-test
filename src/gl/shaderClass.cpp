#include "gl/shaderClass.h"



std::string get_file_contents(const char* filename){
    std::ifstream in(filename, std::ios::binary);
    if (!in){
        std::cerr << "ERROR in get_file_contents: unable to open file " <<  filename << std::endl;
        throw std::runtime_error(std::string("unable to open file ") + filename);       
    }
    else{
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());

        // read
        in.seekg(0,std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
		/*  
		 *std::streampos size = in.tellg();
        if (size > 0) {
            contents.resize(static_cast<size_t>(size));
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
        
		*/
        return contents;
    }
}        


Shader::Shader(const char* p_vertFilename, const char* p_fragFilename) {
    // get those in a string
    std::string vertexSrcCode = get_file_contents(p_vertFilename);
    std::string fragmentSrcCode = get_file_contents(p_fragFilename);      

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


// 3 inputs
// including geometry shaders
Shader::Shader(const char* p_vertFilename, const char* p_geomFilename, const char* p_fragFilename){
    // get those in a string
    std::string vertexSrcCode = get_file_contents(p_vertFilename);
    std::string geometrySrcCode  = get_file_contents(p_geomFilename);
    std::string fragmentSrcCode = get_file_contents(p_fragFilename);      

    // however, we want it to be charater arrays
    const char* vertexSource = vertexSrcCode.c_str();
    const char* geometrySource = geometrySrcCode.c_str();
    const char* fragmentSource = fragmentSrcCode.c_str();

	// vertex and fragment shader objects
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX"); // error handling

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT"); // error handling
                                               //
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &geometrySource, NULL);
	glCompileShader(geometryShader);
	compileErrors(geometryShader, "GEOMETRY"); // error handling


	// "wrap the vertex and fragment shader objects up"
	// using shader program
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glAttachShader(ID, geometryShader);

	// wrap them up by linking
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");



	// we can now safely delete the shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);

}

void Shader::Activate(){
	glUseProgram(ID);
}

void Shader::Delete(){
	glDeleteProgram(ID);
}




void Shader::compileErrors(unsigned int p_shader, std::string p_type){

    // ============== goback here
    // glGetUniformLocation only accepts a linked PROGRAM id; calling it on a shader
    // object (VERTEX/FRAGMENT/GEOMETRY) just raises GL_INVALID_OPERATION, so the old
    // debug print here was removed
    // std::cout << "p_type: " << p_type << ": " << glGetUniformLocation(p_shader, "u_diffuse_tex_type0") << std::endl;
	GLint hasCompiled;
	char infoLog[1024];
	if(p_type != "PROGRAM"){	// compilation
		glGetShaderiv(p_shader, GL_COMPILE_STATUS, &hasCompiled);
		if(hasCompiled == GL_FALSE){
			glGetShaderInfoLog(p_shader, 1024, NULL, infoLog);
			// std::cerr << "SHADER_COMPILATION_ERROR for:" << p_type << "\n" << std::endl;
			std::cerr << "SHADER_COMPILATION_ERROR for: " << p_type << "\n" << infoLog << std::endl;
		}

	// } else{		// linking 
	// 	glGetShaderiv(p_shader, GL_COMPILE_STATUS, &hasCompiled);
	// 	// glGetProgramiv(p_shader, GL_LINK_STATUS, &hasCompiled);
	// 	if(hasCompiled == GL_FALSE){
	// 		glGetShaderInfoLog(p_shader, 1024, NULL, infoLog);
	// 		std::cerr << "SHADER_LINKING_ERROR for:" << p_type << "\n" << std::endl;
	//
	// 		// glGetProgramInfoLog(p_shader, 1024, NULL, infoLog);
	// 		// std::cerr << "PROGRAM_SHADER_LINKING_ERROR for: " << p_type << "\n" << infoLog << std::endl;
	// 	}
	//
	// }
    } else {        // linking 
        glGetProgramiv(p_shader, GL_LINK_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE){
            glGetProgramInfoLog(p_shader, 1024, NULL, infoLog);
            std::cerr << "SHADER_LINKING_ERROR for: " << p_type << "\n" << infoLog << std::endl;
        }
    }
}
