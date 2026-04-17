#pragma once

#include <GLAD/glad.h>

class VBO{
public:
    GLuint ID;
    // use GLsizeiptr when allocating the size of vertices
    // in bytes
    VBO(GLfloat* vertices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};