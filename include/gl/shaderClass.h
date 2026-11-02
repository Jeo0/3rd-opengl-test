#pragma once

#include <GLAD/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);        // outputs the contents
                                                            // of a text file as a string

class Shader{
public:
    GLuint ID;      // public id (i.e. reference)
    Shader(const char* p_vertFilename, const char* p_fragFilename);        // take in the shader source codes
    Shader(const char* p_vertFilename, const char* p_geomFilename, const char* p_fragFilename); // take in the shader source codes

    void Activate();
    void Delete();

private:
    void compileErrors(unsigned int p_shader, std::string p_type);
};
