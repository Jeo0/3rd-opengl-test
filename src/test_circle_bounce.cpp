// this project replaced the WHAT/prev/previousmain.what_cleaner_recent

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

GLFWwindow *StartGLFW();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInputs(GLFWwindow *window);
std::vector<float> GenerateCircleVertices(float centerX, float centerY,
                                          float radius, int circleResolution);
std::string LoadShaderSource(const char* filePath);
void openglPlsCompileThis(GLuint &shaderWhat, std::string shaderType);

constexpr float screenWidth = 800.0f;
constexpr float screenHeight = 600.0f;




int main() {
    GLFWwindow *window = StartGLFW();

    // Shader shaderProgram("resource/Shaders/default.vert",
    // "resource/Shaders/default.frag");


    // compiling shaders
    // SHADER SOURCE VERTEX
    std::cout << "SHADER SOURCES:" << std::endl; // debug
    std::string vertSrcString = LoadShaderSource("resource/Shaders/default.vert");
    const char* vertSrc = vertSrcString.c_str();
    // .c_str() = string ---------> const char*
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // compiling vertex shader
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    openglPlsCompileThis(vertexShader, "VERTEXSHADER");

    // SHADER SOURCE FRAGMENT
    std::string fragSrcString = LoadShaderSource("resource/Shaders/default.frag");
    const char* fragSrc = fragSrcString.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // compiling fragment shader
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    openglPlsCompileThis(fragmentShader, "FRAGMENTSHADER");

    // linking both using shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    // using the program and deleting watever
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    ////////////////////
    ////////////////////
    ////////////////////
    // ANOTHER PIPELINE
    ////////////////////
    ////////////////////
    ////////////////////

    // WHAT OBJECTSSSSS
    // generate the vertices of the circle
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float radius = 123.0f;
    int circleResolution = 100;
    std::vector<float> verticesOfTheCircle = GenerateCircleVertices(centerX, centerY, radius, circleResolution); 

    // we put the vertices on the gpu's memory
    // make sure the vertices are normalized before entering the vbo
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesOfTheCircle.size() * sizeof(float), verticesOfTheCircle.data(), GL_STATIC_DRAW);
    // .data = vector<float> ----------> float array []
    // glBufferData(GL_ARRAY_BUFFER< verticesOfTheCircle.size() * sizeof(float), verticesOfTheCircle.data(), GL_STATIC_DRAW); // correct way
    // let openGL know what to do with the vertices

    constexpr int stride = 2;
    GLboolean normalizeTheVertices = GL_FALSE;
    // GLboolean normalizeTheVertices = GL_TRUE;
    glVertexAttribPointer(0, stride, GL_FLOAT, normalizeTheVertices, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    



    while (!glfwWindowShouldClose(window)) {

        processInputs(window); // inputs

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set to some bluish green
        glClear(GL_COLOR_BUFFER_BIT);

        // now draw the circle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, verticesOfTheCircle.size() * sizeof(float));
        // glDrawArrays(GL_TRIANGLE_FAN, 0, verticesOfTheCircle.size());
        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(verticesOfTheCircle.size() / 2));

        glfwSwapBuffers(window); // present the processed things to the buffer for the next loop
        glfwPollEvents(); // inputs
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
}




GLFWwindow *StartGLFW() {
    if (!glfwInit()) {
        std::cerr << "failed to initialize glfw lmao" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // set the settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // intialize the object itself
    GLFWwindow *window = glfwCreateWindow((int)screenWidth, (int)screenHeight,
                                          "what_simulator", NULL, NULL);
    glfwMakeContextCurrent(window);

    // load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    // for handling window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

// for handling window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInputs(GLFWwindow *window) {
    // get out of loop once we press escape key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::vector<float> GenerateCircleVertices(float centerX, float centerY,
                                          float radius, int circleResolution) {
    std::vector<float> vertices{};
    vertices.push_back(centerX);
    vertices.push_back(centerY);
    // float centerXNDC = (centerX / screenWidth) * 2.0 - 1.0;
    // float centerYNDC = (centerY / screenHeight) * 2.0 - 1.0;

    for (int iii = 0; iii <= circleResolution; iii++) {
        // float angle = 2.0 * 3.141592653589 * ((float)iii / circleResolution);
        float angle =
            2.0f * 3.141592653589 * (static_cast<float>(iii) / circleResolution);
        float x = centerX + cos(angle) * radius;
        float y = centerY + sin(angle) * radius;

        // Normalize to NDC
        // float xNDC = (x / screenWidth) * 2.0f - 1.0f;
        // float yNDC = (y / screenHeight) * 2.0f - 1.0f;

        // glVertex2f(x, y);
        // glVertex2f(xNDC, yNDC);
        // vertices.push_back(xNDC);
        // vertices.push_back(yNDC);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    return vertices;
}

std::string LoadShaderSource(const char* filePath){
    std::ifstream file(filePath);           // Open file
    std::stringstream buffer;
    buffer << file.rdbuf();                 // Read all content into buffer

    // debug
    // std::cout << "LOADSHADERSOURCE:" << std::endl;
    // std::cout << "buffer.str() = " << buffer.str() << std::endl;

    return buffer.str();                    // Return as std::string
}

void openglPlsCompileThis(GLuint &shaderWhat, std::string shaderType){
    glCompileShader(shaderWhat);
    int success;          // starting the check if compile is successful
    char infoLog[1024];
    glGetShaderiv(shaderWhat, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderWhat, 1024, NULL, infoLog);
        std::cerr << "\n===============" << std::endl;
        std::cerr << "ERROR " << shaderType << " COMPILATION FAILED\n" << infoLog << std::endl;
    }
}
