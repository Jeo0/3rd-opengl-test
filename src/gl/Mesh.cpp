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


    // buffer data containing vertices, layout,
    VAOO.LinkAttrib(VBOO, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0); // from globals: vertices
    VAOO.LinkAttrib(VBOO, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float))); // from globals: colors
    VAOO.LinkAttrib(VBOO, 2, 2, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float))); // from globals: textures

    VAOO.Unbind();
    VBOO.Unbind();
    EBOO.Unbind();
}

Mesh::~Mesh() {}

void Draw(Shader &p_shader, Camera &p_camera) {

}
