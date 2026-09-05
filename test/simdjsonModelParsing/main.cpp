// this file is supposed to be Model.cpp
#include "VBO.h"
#include "core/simdjson.h"
#include "damnModel.h" // Model.h
#include "glm/gtc/quaternion.hpp"
#include <cstdint>

#define GLM_ENABLE_EXPERIMENTAL     // glm library says to enable dis shit as string cast is an experimental
#include "glm/gtx/string_cast.hpp" // for printing glm::mat4() using glm::to_string()
#include "glm/gtc/type_ptr.hpp" // glm::make_mat4()

#include <filesystem>
#include <fstream>


////////////////////////
////////////////////////
// helper functions 
////////////////////////
////////////////////////
std::vector<glm::vec3> GroupFloatsVec3(const std::vector<float>& p_positionVectors){
    // inputs are 9x9 expected
    std::vector<glm::vec3> m_allVectors;
    for(int ii=0; ii<(int)p_positionVectors.size(); ii+=3){
        m_allVectors.push_back(glm::vec3(p_positionVectors.at(ii+0), 
                                        p_positionVectors.at(ii+1), 
                                        p_positionVectors.at(ii+2)));
    }
    return m_allVectors;
}

// helper
std::vector<glm::vec2> GroupFloatsVec2(const std::vector<float>& p_textureVectors){
    std::vector<glm::vec2> m_vectors;
    for(int ii=0;  ii<(int)p_textureVectors.size(); ii+=2){
        m_vectors.push_back(glm::vec2(p_textureVectors.at(ii), p_textureVectors.at(ii+1)));
    }
    return m_vectors;
}

// helper
uint64_t GetUintOr(const simdjson::dom::element& p_obj, std::string_view p_key, uint64_t p_default) { uint64_t value = p_default;
    p_obj[p_key].get(value); // if error, value = p_default, else value = whatever p_obj[p_key] is
    return value;
}

// helper
std::vector<Vertex> AssembleVertices(const std::vector<glm::vec3>& p_positions, const std::vector<glm::vec3>& p_normals, const std::vector<glm::vec2>& p_textureUVs){
    std::vector<Vertex> m_outputAssembledVertices;
    // goback here ++++++++++++
    for(uint64_t ii=0; ii<(uint64_t)p_positions.size(); ii++){
        Vertex t_vert;
        t_vert.position = p_positions[ii];
        t_vert.normal = (ii < p_normals.size()) ? p_normals[ii] : glm::vec3(0.0f);
        t_vert.color = glm::vec3(1.0f, 1.0f, 1.0f);
        t_vert.textureUV = (ii < p_textureUVs.size()) ? p_textureUVs[ii] : glm::vec2(0.0f);
        m_outputAssembledVertices.push_back(t_vert);
    }
    return m_outputAssembledVertices;
}

// debug REMOVE LATER {
std::string getget_file_contents(const char* filename){
    std::ifstream in(filename, std::ios::binary);
    if (!in){
        std::cerr << "ERROR in getget_file_contents: unable to open file " <<  filename << std::endl;
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
// debug REMOVE LATER }

Model::Model(const std::string& p_filepath){
    c_filepath = p_filepath;
    std::string m_directory = std::filesystem::path(p_filepath).parent_path().string(); // long string of path
    // std::cout << "INFO c_filepath: " << c_filepath << std::endl;
    // std::cout << "INFO mdirectory: " << m_directory << std::endl;

    try {
        c_jsonData = c_parser.load(c_filepath);
    }
    catch (const std::runtime_error& eee){
        std::cerr << eee.what() << " ERROR SOMETHING WRONG WITH LOADING\n";
    }

    // load data 
    c_binaryData = LoadBinaryData(m_directory);

    // traverse node
    TraverseNode(0);
}
Model::~Model(){
}


// void Model::Draw(Shader& shader, Camera& camera) {
// 	// Go over all meshes and draw each one
// 	// for (unsigned int i = 0; i < c_meshes.size(); i++) {
// 	//        // c_meshes[i]->Mesh::Draw(shader, camera, matricesMeshes[i]);
// 	// }
// }


std::vector<unsigned char> Model::LoadBinaryData(const std::string& p_directory){
    // get the binary data's filepath and name
    // std::cout << "INFO loadbinarydata() PASSED" << std::endl;
    // std::cout << "INFO p_directory : " << p_directory << std::endl;
    // std::string m_uri = c_jsonData["buffers"][0]["uri"]; // the example we have will always be "scene.bin"
    std::string m_uri = std::string(c_jsonData["buffers"].at(0)["uri"]); // the example we have will always be "scene.bin"
    // std::cout << "INFO p_directory: " << p_directory << std::endl;
    // std::cout << "INFO c_filepath: " << c_filepath << std::endl;
    // std::cout << "INFO m_uri: " << m_uri << std::endl;

    // store the binary data 
    std::string m_bytesText = getget_file_contents((p_directory + "/" + m_uri).c_str());
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
    *   or just int
     */

    // std::cout << "INFO oke pass here traversenode\n";
    

    // auto m_currentNode = c_jsonData["nodes"][p_nodeIndex]; // 0th first
    simdjson::dom::element m_currentNode = c_jsonData["nodes"].at(p_nodeIndex); // 0th first

    //////////////////////////////////////////
    //////////////////////////////////////////
    // gather the matrix, scale, rotation, and translation to be processed
    // if current node has matrix
	glm::mat4 m_matNode = glm::mat4(1.0);
    simdjson::dom::array matrixField;
        // std::cout << "INFO empty m_matNode: " << glm::to_string(m_matNode) << std::endl;
    if (m_currentNode["matrix"].get(matrixField) == simdjson::SUCCESS){ // will return an error, should be catched??

        // std::cout << "INFO pass before m_matNode: " << glm::to_string(m_matNode) << std::endl;
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
        // std::cout << "INFO final m_matNode: " << glm::to_string(m_matNode) << std::endl;
    }

    // for the scale 
    glm::vec3 m_scaleNode(1.0);
    simdjson::dom::array t_scaleField;
        // std::cout << "INFO empty m_scaleNode: " << glm::to_string(m_scaleNode) << std::endl;
    if (m_currentNode["scale"].get(t_scaleField) == simdjson::SUCCESS){ // will return an error, should be catched??

        // std::cout << "INFO before m_scaleNode: " << glm::to_string(m_scaleNode) << std::endl;
        // copy 
        // std::array<double, 4> t_scaleArray;
        t_scaleField = m_currentNode["scale"].get_array();
        // std::cout << "INFO pass final m_scaleNode: " << glm::to_string(m_scaleNode) << std::endl;

        m_scaleNode = glm::vec3((double)t_scaleField.at(0), 
                (double)t_scaleField.at(1), 
                (double)t_scaleField.at(2));
    }

    // for the rotation 
    glm::quat m_rotationNode(1.0, 0.0, 0.0,0.0);
    simdjson::dom::array t_rotationField;
        // std::cout << "INFO empty m_rotationNode: " << glm::to_string(m_rotationNode) << std::endl;
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

        // std::cout << "INFO m_matrixNextNode: " << glm::to_string(m_matrixNextNode) << std::endl;

    // check for mesh
    simdjson::dom::element m_meshField;
    if(m_currentNode["mesh"].get(m_meshField) == simdjson::SUCCESS){
        uint64_t t_meshIndex = m_currentNode["mesh"];
        // std::cout << "INFO t_meshIndex: " << t_meshIndex << std::endl;

        LoadMesh(t_meshIndex, m_matrixNextNode);
    }

    // debug REMOVE LATER {
    // simdjson::dom::element name;
    // if(m_currentNode["name"].get(name) == simdjson::SUCCESS){
    //     std::string_view nameOut= m_currentNode["name"];
        // std::cout << "INFO nameOut: " << nameOut << std::endl;
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
    // std::cout << "INFO i'm loaded, p_meshidecx: " << p_meshIndex << std::endl;
    // std::cout << "INFO i'm loaded, p_transform: " << glm::to_string(p_transform) << std::endl;

    simdjson::dom::element m_accessors = c_jsonData["meshes"].at(p_meshIndex)["primitives"].at(0);

        // std::cout << "INFO beforeFloats: \n";
	// Get all accessor indices
    std::vector<float> t_positionVectors = GetFloats( c_jsonData["accessors"].at((uint64_t)m_accessors["attributes"]["POSITION"]));
    // debug REMOVE LATER
    // for(auto &xx: t_positionVectors){
    //     std::cout << "INFO t_positionVectors: " << xx << std::endl;
    // }
        // std::cout << "INFO beforeGroupFloatsVec3: \n";
    std::vector<glm::vec3> m_positions = GroupFloatsVec3(t_positionVectors);
    // debug REMOVE LATER
    // for(auto &xx: m_positions){
    //     std::cout << "INFO m_position: " << glm::to_string(xx)<< std::endl;
    // }
	// std::vector<float> posVec = getFloats(JSON["accessors"][
	//            JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"] ]);
	// std::vector<glm::vec3> positions = groupFloatsVec3(posVec);


    std::vector<float> t_normalVectors = GetFloats(c_jsonData["accessors"].at((uint64_t)m_accessors["attributes"]["NORMAL"]));
    std::vector<glm::vec3> m_normals = GroupFloatsVec3(t_normalVectors);
	// std::vector<float> normalVec = getFloats(JSON["accessors"][
	//            JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"] ]);
	// std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);

    
    std::vector<float> t_textureVectors = GetFloats(c_jsonData["accessors"].at((uint64_t)m_accessors["attributes"]["TEXCOORD_0"]));
    std::vector<glm::vec2> m_textureUVs = GroupFloatsVec2(t_textureVectors);
	// std::vector<float> texVec = getFloats(JSON["accessors"][
	//            JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"] ]);
	// std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);
	//


    // then combine all vertex components 
    std::vector<Vertex> m_finalVertices = AssembleVertices(m_positions, m_normals, m_textureUVs);
    std::vector<GLuint> m_finalIndices = GetIndices(c_jsonData["accessors"].at((uint64_t)m_accessors["indices"]));
    // goback here ++++++++++++++
    // std::cout << "INFO pass m_indices\n";
    // std::cout << "INFO m_indices: " << c_jsonData["accessors"].at((uint64_t)m_accessors["indices"]) << std::endl;
    // outputs: {"bufferView":0,"componentType":5125,"count":15528,"type":"SCALAR"}
    // for(auto& ii: m_indices){
    //     std::cout << "INFO m_index: " << ii << std::endl;
    // }

    std::vector<Texture> m_finalTextures = GetTextures();
    // std::cout << "INFO after return finalTextures" << std::endl;

	// // Combine all the vertex components and also get the indices and textures
	// std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
	// std::vector<GLuint> indices = getIndices(JSON["accessors"][
	//            JSON["meshes"][indMesh]["primitives"][0]["indices"];]);
	// std::vector<Texture> textures = getTextures();

	// Combine the vertices, indices, and textures into a mesh
	// c_meshes.push_back(Mesh(vertices, indices, textures));
    
}

std::vector<Texture> Model::GetTextures(){
	// std::vector<Texture> textures;
    std::vector<Texture> m_textures;
    std::cout << "INFO pass function GETTING TEXTURES" << std::endl; // debug REMOVE

    // debug REMOVE LATER {
    std::cout << "INFO c_jsonData[\"images\"]: " << c_jsonData["images"]<< std::endl;
    // std::cout << "INFO c_jsonData[\"images\"].size(): " << c_jsonData["images"].size() << std::endl;
    std::string damn = static_cast<std::string>(c_jsonData["images"].at(0)["uri"]);
    m_textures.push_back(
            Texture(
                "resource/Models/spear/" + damn, 
                "diffuse_tex_type", 
                0, 
                GL_RGBA, 
                GL_UNSIGNED_BYTE));
    std::cout << "INFO in GetTextures(): success pushback" << std::endl;
    return m_textures;
    // debug REMOVE LATER }


	// std::string fileStr = std::string(file);
	// std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	//
	// // Go over all images
	// for (unsigned int i = 0; i < JSON["images"].size(); i++)
	// {
	// 	// uri of current texture
	// 	std::string texPath = JSON["images"][i]["uri"];
	//
	// 	// Check if the texture has already been loaded
	// 	bool skip = false;
	// 	for (unsigned int j = 0; j < loadedTexName.size(); j++)
	// 	{
	// 		if (loadedTexName[j] == texPath)
	// 		{
	// 			textures.push_back(loadedTex[j]);
	// 			skip = true;
	// 			break;
	// 		}
	// 	}
	//
	// 	// If the texture has been loaded, skip this
	// 	if (!skip)
	// 	{
	// 		// Load diffuse texture
	// 		if (texPath.find("baseColor") != std::string::npos)
	// 		{
	// 			Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
	// 			textures.push_back(diffuse);
	// 			loadedTex.push_back(diffuse);
	// 			loadedTexName.push_back(texPath);
	// 		}
	// 		// Load specular texture
	// 		else if (texPath.find("metallicRoughness") != std::string::npos)
	// 		{
	// 			Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
	// 			textures.push_back(specular);
	// 			loadedTex.push_back(specular);
	// 			loadedTexName.push_back(texPath);
	// 		}
	// 	}
	// }
	//
    return m_textures;
}



std::vector<GLuint> Model::GetIndices(const simdjson::dom::element p_accessor){
    std::vector<GLuint> m_indices;
    // std::cout << "INFO pass function GETTING INDICES " << std::endl;

    // goback here ++++++++++++++++++
    // std::cout << "INFO p_accessor: " << p_accessor << std::endl;
    // outputs: {"bufferView":0,"componentType":5125,"count":15528,"type":"SCALAR"}
    uint64_t m_bufferViewIndex = GetUintOr(p_accessor, "bufferView", 0);
    uint64_t m_count = p_accessor["count"];
    uint64_t m_accByteOffset = GetUintOr(p_accessor, "byteOffset", 0);
    uint64_t m_componentType = p_accessor["componentType"];
    // debug REMOVE LATER {
    // std::cout << "INFO m_bufferViewInd: " << m_bufferViewIndex << std::endl;
    // std::cout << "INFO m_count: " << m_count << std::endl;
    // std::cout << "INFO m_accByteOffset: " << m_accByteOffset << std::endl;
    // std::cout << "INFO m_componentType: " << m_componentType << std::endl;
    // debug REMOVE LATER }



	// getting propertings from bufferView
    simdjson::dom::element m_bufferViewObj = c_jsonData["bufferViews"].at(m_bufferViewIndex);
    // std::cout << "INFO m_bufferView: " << m_bufferViewObj << std::endl;
    // outputs: {"buffer":0,"byteLength":62112,"name":"floatBufferViews","target":34963}
    uint64_t m_byteOffset = GetUintOr(m_bufferViewObj, "byteOffset", 0);
    
	// // Get properties from the bufferView
	// json bufferView = JSON["bufferViews"][buffViewInd];
	// unsigned int byteOffset = bufferView["byteOffset"];
	//    if(!byteOffset){
	//        bufferView = bufferView.value("byteOffset", 0);
	//    }

    // getting the actual indices
	// // Get indices with regards to their type: unsigned int, unsigned short, or short
	uint64_t t_beginningOfData = m_byteOffset + m_accByteOffset;
	if (m_componentType == 5125) {  // 5125 = unsigned int 
        for(uint64_t ii = t_beginningOfData;    ii < m_byteOffset + m_accByteOffset + m_count * 4;  ii +=4) {
			unsigned char t_cacheBytes[] = { 
                c_binaryData[ii+0],
                c_binaryData[ii+1],
                c_binaryData[ii+2],
                c_binaryData[ii+3]
            };
			unsigned int t_finalValue; 
			std::memcpy(&t_finalValue, t_cacheBytes, sizeof(unsigned int));
			m_indices.push_back((GLuint)t_finalValue);
		}
	}
	else if (m_componentType == 5123) { // 5123 = unsigned short
        for(uint64_t ii = t_beginningOfData;    ii < m_byteOffset + m_accByteOffset + m_count * 2;  ii +=2) {
			unsigned char t_cacheBytes[] = { 
                c_binaryData[ii+0],
                c_binaryData[ii+1]
            };
			unsigned short t_finalValue; 
			std::memcpy(&t_finalValue, t_cacheBytes, sizeof(unsigned short));
			m_indices.push_back((GLuint)t_finalValue);
		}
	}
	else if (m_componentType == 5122) { // 5122 = short
        for(uint64_t ii = t_beginningOfData;    ii < m_byteOffset + m_accByteOffset + m_count * 2;  ii +=2) {
			unsigned char t_cacheBytes[] = { 
                c_binaryData[ii+0],
                c_binaryData[ii+1]
            };
			short t_finalValue; 
			std::memcpy(&t_finalValue, t_cacheBytes, sizeof(short));
			m_indices.push_back((GLuint)t_finalValue);
		}
	}

	return m_indices;
}




std::vector<float> Model::GetFloats(const simdjson::dom::element p_accessor) {
    std::vector<float> m_floats;


    // =========================
	// get properties from the accessor
	// unsigned int buffViewInd = p_accessor.value("bufferView", 1); // from tutorial
    // this is equivalent to
    // uint64_t t_bufferViewField = 1;
    // uint64_t m_bufferViewIndex {};
    // p_accessor["bufferView"].get(t_bufferViewField);     // should return either an error or set the t_bufferViewField to a valid number
    // m_bufferViewIndex = t_bufferViewField;
    // or short hand here below
    uint64_t m_bufferViewIndex = GetUintOr(p_accessor, "bufferView", 1);
    uint64_t m_count = p_accessor["count"];
    uint64_t m_accessorByteOffset = GetUintOr(p_accessor, "byteOffset", 0);
    std::string_view m_type = p_accessor["type"];

	// Get properties from the accessor
	// unsigned int buffViewInd = p_accessor.value("bufferView", 1);
	// unsigned int count = accessor["count"];
	// unsigned int accByteOffset = accessor.value("byteOffset", 0);
	// std::string type = accessor["type"];



    // =========================
    // getting the properties from the bufferViews
    simdjson::dom::element m_bufferViewObj = c_jsonData["bufferViews"].at(m_bufferViewIndex);
    uint64_t m_byteOffset = GetUintOr(m_bufferViewObj, "byteOffset", 0);


    // // Get properties from the bufferView
    // json bufferView = JSON["bufferViews"][buffViewInd];
    // unsigned int byteOffset = bufferView["byteOffset"];
    // if(!byteOffset){
    //     bufferView = bufferView.value("byteOffset", 0);
    // };


    // =========================
    // interprest the type and store into numPerVert
    uint64_t m_numPerVert = 0;
    if(m_type == "SCALAR") m_numPerVert = 1;
    else if(m_type == "VEC2") m_numPerVert = 2;
    else if(m_type == "VEC3") m_numPerVert = 3;
    else if(m_type == "VEC4") m_numPerVert = 4;
    else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");


    // // Interpret the type and store it into numPerVert
    // unsigned int numPerVert;
    // if (type == "SCALAR") numPerVert = 1;
    // else if (type == "VEC2") numPerVert = 2;
    // else if (type == "VEC3") numPerVert = 3;
    // else if (type == "VEC4") numPerVert = 4;
    // else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");


    // =========================
    // do the processing: getting data through all the properties from before
    uint64_t t_beginningOfData = m_byteOffset + m_accessorByteOffset;
    uint64_t t_lengthOfData = m_count * 4 * m_numPerVert;
    for(uint64_t ii = t_beginningOfData;  ii<t_beginningOfData + t_lengthOfData; ii += 4){
        unsigned char t_bytes[] = {
            c_binaryData[ii+0],
            c_binaryData[ii+1],
            c_binaryData[ii+2],
            c_binaryData[ii+3]
        };
        float t_cacheValue;
        std::memcpy(&t_cacheValue,t_bytes, sizeof(float));
        m_floats.push_back(t_cacheValue);
    }
    return m_floats;

    // // Go over all the bytes in the data at the correct place using the properties from above
    // unsigned int beginningOfData = byteOffset + accByteOffset;
    // unsigned int lengthOfData = count * 4 * numPerVert;
    // for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
    // {
    // 	unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
    // 	float value;
    // 	std::memcpy(&value, bytes, sizeof(float));
    // 	floatVec.push_back(value);
    // }
    //
    // return floatVec;
}

int main() {
    Model what("resource/Models/spear/scene.gltf");
}
