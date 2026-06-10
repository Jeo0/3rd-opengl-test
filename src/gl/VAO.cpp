#include "VAO.h"

VAO::VAO(){
	glGenVertexArrays(1, &ID);														// VAO
}

void VAO::LinkAttrib(VBO& p_VBO, GLuint p_layout, GLuint p_numComponents, GLenum p_type, GLsizeiptr p_stride, void* p_offset){
    p_VBO.Bind();

    // original
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);		// position @ 0 	// VBO
	// glEnableVertexAttribArray(0);													// position @ 0		// VBO


	glVertexAttribPointer(p_layout, p_numComponents, p_type, GL_FALSE, p_stride, p_offset);		// position @ 0 	// VBO
	glEnableVertexAttribArray(p_layout);													// position @ 0		// VBO
    p_VBO.Unbind();
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
