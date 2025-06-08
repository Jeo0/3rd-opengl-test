#include "VAO.h"

VAO::VAO(){
	glGenVertexArrays(1, &ID);														// VAO
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset){
    VBO.Bind();

    // original
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// position @ 0 	// VBO
	// glEnableVertexAttribArray(0);													// position @ 0		// VBO


	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);		// position @ 0 	// VBO
	glEnableVertexAttribArray(layout);													// position @ 0		// VBO
    VBO.Unbind();
}


void VAO::Bind(){
	glBindVertexArray(ID);															// VAO
}

void VAO::Unbind(){
    glBindVertexArray(0);
}

void VAO::Delete(){
	glDeleteVertexArrays(1, &ID);	// VAO
}