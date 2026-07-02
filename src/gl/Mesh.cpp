#include "Mesh.h"
#include "EBO.h"
#include <vector>

Mesh::Mesh(std::vector<Vertex> &p_vertices, std::vector<GLuint> &p_indices,
           std::vector<Texture> &p_textures) {
    Mesh::c_vertices = p_vertices;
    Mesh::c_indices = p_indices;
    Mesh::c_textures = p_textures;
  VAOO.Bind();
  VBO VBOO(Mesh::c_vertices);
  EBO EBOO(Mesh::c_indices);
}

Mesh::~Mesh() {}

void Draw(Shader &p_shader, Camera &p_camera) {}
