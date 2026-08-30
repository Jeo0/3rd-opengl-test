#pragma once 

#include "core/simdjson.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"

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


        std::vector<unsigned char> LoadBinaryData(const std::string& p_directory);
        void TraverseNode(unsigned int p_nodeIndex, glm::mat4 p_matrix = glm::mat4(1.0));

    public:
        Model(const std::string& p_filepath);
        ~Model();

};
