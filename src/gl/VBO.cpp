#include "VBO.h"

VBO::VBO(std::vector<Vertex> &vertices) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER,
               ID); // make an object the current data to be processed

  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // // originally and came from main file
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(),
               GL_STATIC_DRAW); // VBO
}

void VBO::Bind() {
  glBindBuffer(GL_ARRAY_BUFFER,
               ID); // make an object the current data to be processed
}
void VBO::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
void VBO::Delete() { glDeleteBuffers(1, &ID); }
