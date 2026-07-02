#pragma once

#include "glm/ext/vector_float3.hpp"
#include <GLAD/glad.h>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;
  glm::vec3 textureUV;
};

class VBO {
public:
  GLuint ID;
  // use GLsizeiptr when allocating the size of vertices
  // in bytes
  VBO(std::vector<Vertex> &vertices);

  void Bind();
  void Unbind();
  void Delete();
};
