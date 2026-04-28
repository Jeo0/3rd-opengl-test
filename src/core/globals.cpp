
#include "core/globals.h"
// OPENGL
// a buffer for the triangle
GLfloat vertices[8*4] = {
    // vertex 3;			color 3                         texture
    -0.0,	0.5,	0.,		.8f,	.1f,	.1f,			1.0f, 1.0f, // top mid
    0.5,	-0.5,	0.,		.2f,	.8f,	.1f,			0.0f, 0.0f, // bot right
    -0.5,	.5,		0.,		.2f,	.14f,	.8f,			0.0f, 1.0f, // top left 
    0.3,	0.8,	0.,		.9f,	.5f,	1.f,			1.0f, 0.0f // top right
};

// reduce redundancy with element buffer objects
GLuint indices[3*2] = {
    0,1,2,		// left triangle
    0,1,3		// right triangle
};
