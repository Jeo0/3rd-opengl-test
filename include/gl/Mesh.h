
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "core/Texture.h"
#include "shaderClass.h"
#include <vector>

class Mesh {
public:
  std::vector<Vertex> c_vertices;
  std::vector<GLuint> c_indices;
  std::vector<Texture> c_textures;

  // We gonna store here in VAOO what we will be drawing
  VAO VAOO;

  Mesh(std::vector<Vertex> &p_vertices, std::vector<GLuint> &p_indices,
       std::vector<Texture> &p_textures);
  ~Mesh();

  void Draw(Shader &p_shader, Camera &p_camera);
};
