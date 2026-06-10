
#include "core/globals.h"
// OPENGL
// a buffer for the triangle

// =========================================
// cursor shape
// =========================================
// GLfloat vertices[8*4] = {
//     // vertex 3;			color 3                         texture -0.0,	0.5,	0.,		.8f,	.1f,	.1f,			1.0f, 1.0f, // top mid
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
// GLfloat vertices[8*4] = {
//     // vertex 3;			color 3                         texture
//     -0.5,   -0.5,	0.,		.8f,	.1f,	.1f,			0.0f, 0.0f, // low left
//     -0.5,	0.5,	0.,		.2f,	.8f,	.1f,			0.0f, 1.0f, // up left
//     0.5,	    0.5,	0.,		.2f,	.14f,	.8f,			1.0f, 1.0f, // up right
//     0.5,    -0.5,	0.,		.9f,	.5f,	1.f,			1.0f, 0.0f  // low right
// };
//
// // reduce redundancy with element buffer objects
// GLuint indices[3*2] = {
//     0,2,1,		// up left triangle
//     0,3,2		// low right triangle
// };
//


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



// =========================================
// pyramid
// =========================================
GLfloat vertices[8*5] = {
    // vertex 3;			    color 3                         texture
    0.0,   0.5,	    0.0,		.8f,	.1f,	.1f,			2.5f, 5.0f, // top
    -0.5,	0.0,	-0.3,		.2f,	.8f,	.1f,			0.0f, 0.0f, // front left
    0.5,	0.0,	-0.3,		.2f,	.14f,	.8f,			5.0f, 0.0f, // front right
    0.5,    0.0,	0.3,		.9f,	.5f,	1.f,			0.0f, 0.0f, // back right
    -0.5,    0.0,	0.3,		.9f,	.5f,	1.f,			0.0f, 0.0f  // back left
	// -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	// -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	//  0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	//  0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	//  0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// reduce redundancy with element buffer objects
GLuint indices[3*6] = {
    0,1,2,      // front 
    0,1,4,      // left 
    0,4,3,      // back 
    0,3,2,      // right 
    1,2,3,      // bot right side triangle 
    3,4,1       // bot left sidei triangle
	// 0, 1, 2,
	// 0, 2, 3,
	// 0, 1, 4,
	// 1, 2, 4,
	// 2, 3, 4,
	// 3, 0, 4
};
