#include "core/Texture.h"
#include "stb/stb_image.h"

Texture::Texture(std::string& p_parentDir,
                    std::string& p_textureFile,
                    GLuint& p_textureID,
                    GLenum p_activeTexture,

                    GLenum p_targetParam,
                    GLint p_internalImageFormat,
                    GLenum p_imageFormat,
                    GLenum p_imageType
                    ){
    int widthImg, heightImg, numColChann;
    unsigned char* bytes = stbi_load((p_parentDir + p_textureFile).c_str(), &widthImg, &heightImg, &numColChann, 4);
    if(!bytes){
        std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
    }   

    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &p_textureID);               // generate opengl texture obj
    glActiveTexture(p_activeTexture);               // assign texture to a texture unit
    glBindTexture(GL_TEXTURE_2D, p_textureID);
    

    auto i_filter = GL_NEAREST;
    // auto i_filter = GL_LINEAR;
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, i_filter); // configures the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(p_targetParam, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(p_targetParam, GL_TEXTURE_MAG_FILTER, i_filter);

    glTexParameteri(p_targetParam, GL_TEXTURE_WRAP_S, GL_REPEAT);   // repeat texture
    glTexParameteri(p_targetParam, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTextureSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xxx, yyy, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xxx, yyy, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);       // jpg
    // glTexImage2D(p_targetParam, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);   // jpg png square
    glTexImage2D(p_targetParam, 0, p_internalImageFormat, widthImg, heightImg, 0, p_imageFormat, p_imageType, bytes);   // jpg png square
    glGenerateMipmap(p_targetParam);
    // ++++===================== goback here

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    stbi_image_free(bytes);
    glBindTexture(p_targetParam, 0);


}



void Texture::TexUnit(Shader& p_shaderProgram, std::string p_uniformTexture, GLint p_uniformVariable){
    GLuint tex0Uniform = glGetUniformLocation(p_shaderProgram.ID, p_uniformTexture.c_str());
    p_shaderProgram.Activate();
    glUniform1i(tex0Uniform, p_uniformVariable);
}



Texture::~Texture(){}
