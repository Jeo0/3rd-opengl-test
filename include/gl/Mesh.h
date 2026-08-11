
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "gl/Texture.h"
#include "shaderClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <vector>

class Mesh {
public:
  std::vector<Vertex> c_vertices;
  std::vector<GLuint> c_indices;
  std::vector<std::unique_ptr<Texture>> c_textures;
  glm::mat4 c_modelPos = glm::mat4(1.0);       // position of the mesh within the scene

  // We gonna store here in VAOO what we will be drawing
  VAO VAOO;



  Mesh(std::vector<Vertex> &p_vertices, std::vector<GLuint> &p_indices,
       std::vector<std::unique_ptr<Texture>> p_textures);
  ~Mesh();

  void Draw(Shader &p_shader, Camera& p_camera);
  void SetPosition(const glm::vec3& p_position);
  
};
