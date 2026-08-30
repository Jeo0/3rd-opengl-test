
#include "gl/Model.h"
#include "core/simdjson.h"

#include "gl/shaderClass.h" // for the get_file_contents() helper
#include "glm/gtc/type_ptr.hpp"

#include <filesystem>
#include <array>

Model::Model(const std::string& p_filepath){
    c_filepath = p_filepath;
    std::string m_directory = std::filesystem::path(p_filepath).parent_path().string(); // long string of path

    c_jsonData = c_parser.load(c_filepath);

    // load data 
    c_binaryData = LoadBinaryData(m_directory);

    // traverse node
    TraverseNode(0);
}
Model::~Model(){
}

void Model::Draw(Shader& p_shader, Camera& p_camera){
    for (auto& ii_mesh : c_meshes) {
        ii_mesh->Draw(p_shader, p_camera);
    }
}


std::vector<unsigned char> Model::LoadBinaryData(const std::string& p_directory){
    // get the binary data's filepath and name
    // std::string m_uri = c_jsonData["buffers"][0]["uri"]; // the example we have will always be "scene.bin"
    std::string m_uri = std::string(c_jsonData["buffers"].at(0)["uri"]); // the example we have will always be "scene.bin"
    // debug REMOVE LATER
    // std::cout << "INFO p_directory: " << p_directory << std::endl; 
    // std::cout << "INFO c_filepath: " << c_filepath << std::endl;
    // std::cout << "INFO m_uri: " << m_uri << std::endl;
    // outputs: 
    // INFO p_directory: resource/Models/spear
    // INFO c_filepath: resource/Models/spear/scene.gltf
    // INFO m_uri: scene.bin

    // store the binary data 
    std::string m_bytesText = get_file_contents((p_directory + "/" + m_uri).c_str());

    return std::vector<unsigned char>(m_bytesText.begin(), m_bytesText.end());
}

void Model::TraverseNode(unsigned int p_nodeIndex, glm::mat4 p_matrix){
    // auto m_currentNode = c_jsonData["nodes"][p_nodeIndex]; // 0th first
    // simdjson::dom::element m_currentNode = c_jsonData["nodes"][p_nodeIndex]; // 0th first
    simdjson::dom::element m_currentNode = c_jsonData["nodes"].at(p_nodeIndex); // 0th first

    // if current node has matrix
    // std::cout << m_currentNode["matrix"];
    // outputs the matrix array
	glm::mat4 m_matNode = glm::mat4(1.0);
    if(m_currentNode["matrix"]) {
        simdjson::dom::array matrixIterator = m_currentNode["matrix"].get_array();
        std::array<double, 16> matrixArray;

        // copy
        for(int ii=0; ii<matrixArray.size(); ii++){
            matrixArray[ii] = matrixIterator[ii];
        }

        // m_matNode = glm::make_mat4(matrixArray);
        m_matNode = glm::make_mat4(matrixArray.data()); // we glm::make_mat4() needs the pointer, not the whole object
    }
    
    // check for children 
    // if(["children"])

}
