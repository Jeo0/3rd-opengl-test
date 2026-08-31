// this file is supposed to be Model.cpp
#include "damnModel.h" // Model.h
#include "glm/gtc/quaternion.hpp"

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

void Model::TraverseNode(unsigned int p_nodeIndex, const glm::mat4& p_matrix){
    /*
    *   oke here are the rules after some long ass time
    *   simdjson::dom::element type when you expect it to be of anything of SINGLE element (double, int, string, watever etc.)
    *   simdjson::dom::array type when expecting to be given a series of watevers element
    *   
    *   std::string_view type to read the value of an simdjson::dom::element when it is a string
    *   uint64_t type to read the value of an simdjson::dom::element when integers
     */


    // auto m_currentNode = c_jsonData["nodes"][p_nodeIndex]; // 0th first
    simdjson::dom::element m_currentNode = c_jsonData["nodes"].at(p_nodeIndex); // 0th first

    //////////////////////////////////////////
    //////////////////////////////////////////
    // gather the matrix, scale, rotation, and translation to be processed
    // if current node has matrix
	glm::mat4 m_matNode = glm::mat4(1.0);
    simdjson::dom::array matrixField;
        std::cout << "INFO empty m_matNode: " << glm::to_string(m_matNode) << std::endl;
    if (m_currentNode["matrix"].get(matrixField) == simdjson::SUCCESS){ // will return an error, should be catched??

        std::cout << "INFO before m_matNode: " << glm::to_string(m_matNode) << std::endl;
        // // copy
        // // method 1
        std::array<double, 16> t_matrixArray;
        for(auto ii=0; ii<(int)t_matrixArray.size(); ii++){
            t_matrixArray[ii] = matrixField.at(ii);
        }
        m_matNode = glm::make_mat4(t_matrixArray.data());

        // method 2
        // double t_matrixArray[16];
        // for(auto ii=0; ii<16; ii++){
        //     t_matrixArray[ii] = matrixField.at(ii);
        // }
        // m_matNode = glm::make_mat4(t_matrixArray);

        // debug REMOVE LATER
        std::cout << "INFO final m_matNode: " << glm::to_string(m_matNode) << std::endl;
    }

    // for the scale 
    glm::vec3 m_scaleNode(1.0);
    simdjson::dom::array t_scaleField;
        std::cout << "INFO empty m_scaleNode: " << glm::to_string(m_scaleNode) << std::endl;
    if (m_currentNode["scale"].get(t_scaleField) == simdjson::SUCCESS){ // will return an error, should be catched??

        std::cout << "INFO before m_scaleNode: " << glm::to_string(m_scaleNode) << std::endl;
        // copy 
        // std::array<double, 4> t_scaleArray;
        t_scaleField = m_currentNode["scale"].get_array();
        std::cout << "INFO final m_scaleNode: " << glm::to_string(m_scaleNode) << std::endl;

        m_scaleNode = glm::vec3((double)t_scaleField.at(0), 
                (double)t_scaleField.at(1), 
                (double)t_scaleField.at(2));
    }

    // for the rotation 
    glm::quat m_rotationNode(1.0, 0.0, 0.0,0.0);
    simdjson::dom::array t_rotationField;
        std::cout << "INFO empty m_rotationNode: " << glm::to_string(m_rotationNode) << std::endl;
    if(m_currentNode["rotation"].get(t_rotationField) == simdjson::SUCCESS){
        t_rotationField = m_currentNode["rotation"].get_array();

        // gltf stores quaternions as [x, y, z, w]; glm::quat constructor wants (w, x, y, z)
        m_rotationNode = glm::quat((double)t_rotationField.at(3),
                (double)t_rotationField.at(0),
                (double)t_rotationField.at(1),
                (double)t_rotationField.at(2));
    }

    // for the translation
    glm::vec3 m_translationNode(0.0);
    simdjson::dom::array t_translationField;
    if(m_currentNode["translation"].get(t_translationField) == simdjson::SUCCESS){
        t_translationField = m_currentNode["translation"].get_array();

        m_translationNode = glm::vec3((double)t_translationField.at(0),
                (double)t_translationField.at(1),
                (double)t_translationField.at(2));
    }

    // processing
    glm::mat4 m_translate = glm::translate(glm::mat4(1.0), m_translationNode);
    glm::mat4 m_rotate = glm::mat4_cast(m_rotationNode);
    glm::mat4 m_scale = glm::scale(glm::mat4(1.0), m_scaleNode);

    // multiply all along with the current matrix; order matters 
    glm::mat4 m_matrixNextNode = p_matrix * m_matNode * m_translate * m_rotate * m_scale;

        std::cout << "INFO m_matrixNextNode: " << glm::to_string(m_matrixNextNode) << std::endl;

    // check for mesh
    simdjson::dom::element m_meshField;
    if(m_currentNode["mesh"].get(m_meshField) == simdjson::SUCCESS){
        uint64_t t_meshIndex = m_currentNode["mesh"];
        std::cout << "INFO t_meshIndex: " << t_meshIndex << std::endl;

        LoadMesh(t_meshIndex, m_matrixNextNode);
    }

    // debug REMOVE LATER {
    // simdjson::dom::element name;
    // if(m_currentNode["name"].get(name) == simdjson::SUCCESS){
    //     std::string_view nameOut= m_currentNode["name"];
    //     std::cout << "INFO nameOut: " << nameOut << std::endl;
    // }
    // debug REMOVE LATER }

    // now for the children but we recurse
    simdjson::dom::element m_childrenField;
    if(m_currentNode["children"].get(m_childrenField) == simdjson::SUCCESS){
        simdjson::dom::array t_children = m_currentNode["children"].get_array();
        for (simdjson::dom::element ii_child : t_children) {
            uint64_t t_childIndex = ii_child;
            TraverseNode(t_childIndex, m_matrixNextNode);
        }
    }

}

void Model::LoadMesh(unsigned int p_meshIndex, const glm::mat4& p_transform){
    std::cout << "INFO i'm loaded, p_meshidecx: " << p_meshIndex << std::endl;
    std::cout << "INFO i'm loaded, p_transform: " << glm::to_string(p_transform) << std::endl;
    
}

int main() {
    Model what("resource/Models/spear/scene.gltf");
}
