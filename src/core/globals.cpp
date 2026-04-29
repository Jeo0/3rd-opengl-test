
#include "core/globals.h"
// OPENGL
// a buffer for the triangle

// =========================================
// cursor shape
// =========================================
// GLfloat vertices[8*4] = {
//     // vertex 3;			color 3                         texture
//     -0.0,	0.5,	0.,		.8f,	.1f,	.1f,			1.0f, 1.0f, // top mid
//     0.5,	-0.5,	0.,		.2f,	.8f,	.1f,			0.0f, 0.0f, // bot right
//     -0.5,	.5,		0.,		.2f,	.14f,	.8f,			0.0f, 1.0f, // top left 
//     0.3,	0.8,	0.,		.9f,	.5f,	1.f,			1.0f, 0.0f // top right
// };

// GLuint indices[3*2] = {
//     0,1,2,		// up left triangle
//     0,2,3		// low right triangle
// };



// =========================================
// square shape
// =========================================
GLfloat vertices[8*4] = {
    // vertex 3;			color 3                         texture
    -0.5,   -0.5,	0.,		.8f,	.1f,	.1f,			0.0f, 0.0f, // low left
    -0.5,	0.5,	0.,		.2f,	.8f,	.1f,			0.0f, 1.0f, // up left
    0.5,	0.5,	0.,		.2f,	.14f,	.8f,			1.0f, 1.0f, // up right
    0.5,    -0.5,	0.,		.9f,	.5f,	1.f,			1.0f, 0.0f  // low right
};

// reduce redundancy with element buffer objects
GLuint indices[3*2] = {
    0,2,1,		// up left triangle
    0,3,2		// low right triangle
};



// =========================================
// copy paste from https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%206%20-%20Textures/Main.cpp
// =========================================
// GLfloat vertices[8*4] = {
//  //     COORDINATES     /        COLORS      /   TexCoord  //
// 	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
// 	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
// 	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
// 	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
// };
//
// // Indices for vertices order
// GLuint indices[3*2] = {
// 	0, 2, 1, // Upper triangle
// 	0, 3, 2 // Lower triangle
// };
