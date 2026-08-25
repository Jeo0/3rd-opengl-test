
#include "gl/Model.h"

Model::Model(const std::string& p_filePath){
}
Model::~Model(){
}

void Model::Draw(Shader& p_shader, Camera& p_camera){
    for (auto& ii_mesh : c_meshes) {
        ii_mesh->Draw(p_shader, p_camera);
    }
}

