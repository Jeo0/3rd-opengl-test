#pragma once 

#include "core/simdjson.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"

// #include "Mesh.h"
// #include "NoMesh.h"
#include "gl/Texture.h"
#include "GLAD/glad.h"


#include <cmath>
#include <cstdlib>
#include <stdatomic.h>
#include <string>
#include <type_traits>
#include <vector>


class Model {
    private:
        std::string c_filepath;

        // we use the dom instead of on demand as stated here: https://arxiv.org/pdf/2312.17149 
        // dom is for materializing the data we will be parsing into memory
        // ondemand is just as is, on demand, kind of doing things and then redoing things later
        simdjson::dom::parser c_parser; 
        simdjson::dom::element c_jsonData;
        std::vector<unsigned char> c_binaryData;

        // std::vector<std::unique_ptr<Mesh>> c_meshes;
        std::vector<std::shared_ptr<Texture>> c_loadedTextures;


        std::vector<unsigned char> LoadBinaryData(const std::string& p_directory);
        void TraverseNode(unsigned int p_nodeIndex, const glm::mat4& p_matrix = glm::mat4(1.0));
        void LoadMesh(unsigned int p_meshIndex, const glm::mat4& p_transform);
        
        std::vector<float> GetFloats(const simdjson::dom::element p_accessor);
        std::vector<GLuint> GetIndices(const simdjson::dom::element p_accessor);
        std::vector<Texture> GetTextures();

    public:
        Model(const std::string& p_filepath);
        ~Model();

        // void Draw(Shader &p_shader, Camera& p_camera);
};
