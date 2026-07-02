#pragma once

#include <GLAD/glad.h>
#include <vector>

class EBO {
public:
  GLuint ID;
  // use GLsizeiptr when allocating the size of vertices
  // in bytes
  EBO(std::vector<GLuint> &indices);

  void Bind();
  void Unbind();
  void Delete();
};
