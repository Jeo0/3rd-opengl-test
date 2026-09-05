#include "gl/Texture.h"
#include "stb/stb_image.h"
#include <GL/gl.h>
#include <iostream>

Texture::Texture(const std::string& p_imageLoc,
                    const std::string& p_textureType,
                    GLuint p_slot,  // add  this to the header  // goback here ============
                    GLenum p_imageFormat, // non existent in jgl demo; instead, use GL_RGBA
                    GLenum p_pixelType // non existent in jgl demo; instead use GL_UNSIGNED_BYTE
                    ){
    c_type = p_textureType;

    // loading texture
    int widthImg, heightImg, numColChann;

    std::cout << "INFO passing Texture constructor;\n\
        p_imageLoc.c_str():" << p_imageLoc.c_str() << std::endl;

    // actual image texture
    unsigned char* bytes = stbi_load(p_imageLoc.c_str(), &widthImg, &heightImg, &numColChann, 4); 
    if(!bytes){
        std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
        return;
    }   

    // debug  REMOVE
    std::cout << p_imageLoc.c_str() << std::endl;

    // openGL functions
    // generating texture
    // control this ting
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &c_ID);               // generate opengl texture obj


    glActiveTexture(GL_TEXTURE0 + p_slot);               // assign texture to a texture unit
    c_unit = p_slot;
    glBindTexture(GL_TEXTURE_2D, c_ID);
    


    auto m_filter = GL_NEAREST;
    // auto m_filter = GL_LINEAR;
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter); // configures the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);   // repeat texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // glTextureSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xxx, yyy, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xxx, yyy, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);       // jpg
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);   // jpg png square
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, p_imageFormat, p_pixelType, bytes);   // jpg png square
    glGenerateMipmap(GL_TEXTURE_2D);
    // ++++===================== goback here

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);


}



void Texture::TexUnit(Shader& p_shaderProgram, const std::string& p_uniformTexture, GLint p_uniformUnit){

    GLuint tex0Uniform = glGetUniformLocation(p_shaderProgram.ID, p_uniformTexture.c_str());
    p_shaderProgram.Activate();
    glUniform1i(tex0Uniform, p_uniformUnit);
}



Texture::~Texture(){
    // this->Delete();
    Delete();
}

void Texture::Bind(){
    glActiveTexture(GL_TEXTURE0 + c_unit);               // assign texture to a texture unit
    glBindTexture(GL_TEXTURE_2D, c_ID);
}

void Texture::Unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete(){
    glDeleteTextures(1, &c_ID);
}
