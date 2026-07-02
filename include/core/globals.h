
#pragma once
#include "GLAD/glad.h"
#include <vector>
// OPENGL
// a buffer for the triangle
extern std::vector<GLfloat> vertices;
// extern GLfloat vertices[8*4];

// reduce redundancy with element buffer objects
extern std::vector<GLuint> indices;
// extern GLuint indices[3*2];
