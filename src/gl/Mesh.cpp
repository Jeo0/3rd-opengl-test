#include "Mesh.h"
//#include <string>
#include "EBO.h"
#include <GL/gl.h>
#include <vector>
#include "glm/gtc/type_ptr.hpp"

Mesh::Mesh(std::vector<Vertex> &p_vertices, std::vector<GLuint> &p_indices,
           std::vector<std::shared_ptr<Texture>> p_textures) {
    Mesh::c_vertices = p_vertices;
    Mesh::c_indices = p_indices;
    Mesh::c_textures = std::move(p_textures);
    VAOO.Bind();
    VBO VBOO(Mesh::c_vertices);
    EBO EBOO(Mesh::c_indices);


    // buffer data containing vertices, layout,
    // VAOO.LinkAttrib(VBOO, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0); // from globals: vertices
    // VAOO.LinkAttrib(VBOO, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float))); // from globals: colors
    // VAOO.LinkAttrib(VBOO, 2, 2, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float))); // from globals: textures
    VAOO.LinkAttrib(VBOO, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, position));
    VAOO.LinkAttrib(VBOO, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, color));
    VAOO.LinkAttrib(VBOO, 2, 2, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, textureUV));

    VAOO.Unbind();
    VBOO.Unbind();
    EBOO.Unbind();
}

Mesh::~Mesh() {}

void Mesh::Draw(Shader &p_shader, Camera& p_camera) {
    // to access the uniforms, go bind here
    p_shader.Activate();
    VAOO.Bind();

    unsigned int m_numberDiffuse = 0;
    for (unsigned int iii = 0;  iii<c_textures.size(); iii++) {
        std::string m_num {};
        std::string m_type {c_textures[iii]->c_type};

        // debug REMOVE
        // std::cout << "type of texture: " << c_textures[iii]->c_type << std::endl;

        if(m_type == "diffuse_tex_type"){
            m_numberDiffuse++;
            m_num = std::to_string(m_numberDiffuse);
        }

        // (m_type + m_num).c_str()     i.e. = diffuse1     diffuse2    diffuse3
        // c_textures[iii]->TexUnit(p_shader, ("u_" + m_type + m_num), iii);  
        c_textures[iii]->TexUnit(p_shader, ("u_" + m_type + m_num), c_textures[iii]->c_unit);
        c_textures[iii]->Bind();
    }


    // take care of the camera matrix
    glUniform3f(glGetUniformLocation(p_shader.ID, "u_camPos"), 
                p_camera.c_position.x, 
                p_camera.c_position.y,
                p_camera.c_position.z);
    p_camera.Matrix(p_shader, "u_camMatrix");

    // mesh's own place in the scene
    glUniformMatrix4fv(glGetUniformLocation(p_shader.ID, "u_modelPos"), 1, GL_FALSE, glm::value_ptr(c_modelPos));

    // then drawing of the actual mesh
    glDrawElements(GL_TRIANGLES, c_indices.size(), GL_UNSIGNED_INT, 0);

}

void Mesh::SetPosition(const glm::vec3& p_position){
    c_modelPos = glm::translate(glm::mat4(1.0), p_position);
}

void Mesh::SetTransform(const glm::mat4& p_transform){
    c_modelPos = p_transform;
}
