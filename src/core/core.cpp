#include "core/core.h"
#include "core/globals.h"


Core::Core() 
    : windowObj(800, 800, "title", NULL, NULL, {0.01f, 0.24f, 0.45f, 1.0f}),
      shaderProgram("resource/Shaders/default.vert", "resource/Shaders/default.frag"),
      VBO1(vertices, sizeof(vertices)),
      EBO1(indices, sizeof(indices)) 
{
    Init();
}

Core::~Core() {
        Cleanup();
    }

void Core::Run() {
    // The Main Game/Render Loop
    while (!glfwWindowShouldClose(windowObj.ID)) {
        Update();
        Render(); // buffer swap
        glfwPollEvents(); // polling is a must after buffer swap
    }
}

void Core::Init() {
    VAO1.Bind();
    VBO1.Bind();
    EBO1.Bind();
                // buffer data containing vertices, layout, 
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);     // from globals: vertices
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // from globals: colors
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // from globals: textures
    
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // uniforms
    uniformID = glGetUniformLocation(shaderProgram.ID, "scale");


    // textures
    std::string textureFile = "resource/Textures/gadem.jpg";
    int xxx, yyy, nnn;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(textureFile.c_str(), &xxx, &yyy, &nnn, 0);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xxx, yyy, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);


    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);



    GLuint tex0Uniform = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uniform, 0);
}

void Core::Update() {
/*  LOGIC TINGS */
    // mouse input ++++============================== goback here
    windowObj.HandleMouseInput();


    // make cursor scale up and down ting 
    for (int i = 0; i < 4; i++) {
        bgColor[i] += indexer;
    }
    
    if (bgColor[0] >= 1.0f || bgColor[0] <= 0.0f) {
        indexer *= -1.0f;
    }
}

void Core::Render() {
/*  RENDER TINGS */
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.Activate();
    glUniform1f(uniformID, bgColor[0]); // tanggalin to; the 'scale' depends on the bgcolor[0]'s value
    glBindTexture(GL_TEXTURE_2D, texture);

    // reset 
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(windowObj.ID);
}

void Core::Cleanup() {

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    glDeleteTextures(1, &texture);
    shaderProgram.Delete();
    
    glfwDestroyWindow(windowObj.ID);
    glfwTerminate();
}
