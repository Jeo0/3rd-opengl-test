#pragma once 
#include "gl/Mesh.h"
#include "gl/Texture.h"
#include "gl/shaderClass.h"

#include "core/simdjson.h"

#include <string>
#include <vector>


class Model {
    private:
        std::string c_filename;

        std::vector<unsigned char> c_data;
        simdjson::ondemand::parser c_parser;

        std::vector<std::unique_ptr<Mesh>> c_meshes;

        std::vector<unsigned char> LoadBinaryData();


    public:
        Model(const std::string& p_filePath);
        ~Model();

        void Draw(Shader& p_shader, Camera& p_camera);
};
