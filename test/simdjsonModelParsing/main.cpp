// this file is supposed to be Model.cpp
#include "damnModel.h" // Model.h

#define GLM_ENABLE_EXPERIMENTAL     // glm library says to enable dis shit as string cast is an experimental, along with quaternion that I dont even use
#include "glm/gtx/string_cast.hpp" // for printing glm::mat4() using glm::to_string()
#include "glm/gtc/type_ptr.hpp" // glm::make_mat4()

#include <filesystem>
#include <fstream>

std::string get_file_contents(const char* filename){
    std::ifstream in(filename, std::ios::binary);
    if (!in){
        std::cerr << "ERROR in get_file_contents: unable to open file " <<  filename << std::endl;
        throw std::runtime_error(std::string("unable to open file ") + filename);       
    }
    else{
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());

        // read
        in.seekg(0,std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
}        


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



std::vector<unsigned char> Model::LoadBinaryData(const std::string& p_directory){
    // get the binary data's filepath and name
    // std::string m_uri = c_jsonData["buffers"][0]["uri"]; // the example we have will always be "scene.bin"
    std::string m_uri = std::string(c_jsonData["buffers"].at(0)["uri"]); // the example we have will always be "scene.bin"
    // std::cout << "INFO p_directory: " << p_directory << std::endl;
    // std::cout << "INFO c_filepath: " << c_filepath << std::endl;
    // std::cout << "INFO m_uri: " << m_uri << std::endl;

    // store the binary data 
    std::string m_bytesText = get_file_contents((p_directory + "/" + m_uri).c_str());
    // std::cout << "INFO m_bytesText: " << m_bytesText << std::endl;

    return std::vector<unsigned char>(m_bytesText.begin(), m_bytesText.end());
}

void Model::TraverseNode(unsigned int p_nodeIndex, glm::mat4 p_matrix){
    // auto m_currentNode = c_jsonData["nodes"][p_nodeIndex]; // 0th first
    simdjson::dom::element m_currentNode = c_jsonData["nodes"].at(p_nodeIndex); // 0th first

    // if current node has matrix
	glm::mat4 m_matNode = glm::mat4(1.0);
    simdjson::dom::array matrixField;
    if (m_currentNode["matrix"].get(matrixField) == simdjson::SUCCESS){ // will return an error, should be catched??

        // copy
        std::array<double, 16> matrixArray;
        for(int ii=0; ii<matrixArray.size(); ii++){
            matrixArray[ii] = matrixField.at(ii);
        }

        m_matNode = glm::make_mat4(matrixArray.data());
    }
    // debug REMOVE LATER
    std::cout << "INFO m_matNode: " << glm::to_string(m_matNode) << std::endl;
    
    
    // check for children first
    // if(["children"])

}

int main() {
    Model what("resource/Models/spear/scene.gltf");
}
