#pragma once

#include "GLAD/glad.h"
#include "shaderClass.h"
#include <iostream>

class Texture {
private:

public:
    Texture(std::string& p_parentDir,
                    std::string& p_textureFile,
                    GLuint& p_textureID,
                    GLenum p_activeTexture,

                    GLenum p_targetParam,
                    GLint p_internalImageFormat,
                    GLenum p_imageFormat,
                    GLenum p_imageType);
    ~Texture();

    void Bind();
    void TexUnit(Shader& p_shaderProgram, std::string p_uniformTexture, GLint p_uniformVariable);
};
