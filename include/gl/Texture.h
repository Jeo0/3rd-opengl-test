#pragma once

#include "GLAD/glad.h"
#include "shaderClass.h"
#include <iostream>

class Texture {
private:
public:
    std::string c_type;
    GLuint c_ID;
    GLuint c_unit;

    // Texture(std::string& p_parentDir,
    //                 std::string& p_textureFile,
    //                 GLuint& p_textureID,
    //                 GLenum p_activeTexture,
    //
    //                 GLenum p_targetParam,
    //                 GLint p_internalImageFormat,
    //                 GLenum p_imageFormat,
    //                 GLenum p_imageType);
    Texture(const std::string& p_imageLoc,
            const std::string& p_textureType,
            GLuint      p_slot,
            GLenum      p_imageFormat, // non existent in jgl demo; instead, use GL_RGBA
            GLenum      p_pixelType); // non existent in jgl demo; instead use GL_UNSIGNED_BYTE
    ~Texture();

    void Bind();
    void Unbind();
    void Delete();
    void TexUnit(Shader& p_shaderProgram, const std::string& p_uniformTexture, GLint p_uniformVariable);
};
