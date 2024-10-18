#pragma once

#include <GLAD/glad.h>

class EBO{
public:
    GLuint ID;
    // use GLsizeiptr when allocating the size of vertices
    // in bytes
    EBO(GLuint* indices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};