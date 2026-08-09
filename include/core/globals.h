
#pragma once
#include "GLAD/glad.h"
#include "VBO.h" // for Vertex struct
#include <vector>
// OPENGL
// a buffer for the triangle
extern std::vector<Vertex> g_vertices;
// extern GLfloat g_vertices[8*4];

// reduce redundancy with element buffer objects
extern std::vector<GLuint> g_indices;
// extern GLuint g_indices[3*2];
