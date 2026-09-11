
#include "core/Scene.h"
#include <iostream>


Scene::Scene(int p_viewportHeight, int p_viewportWidth) {
    c_camera = std::make_unique<Camera>(p_viewportHeight, p_viewportHeight, glm::vec3(0.0f, 0.0f, 2.0f));
    c_shader = std::make_unique<Shader>("resource/Shaders/default.vert", "resource/Shaders/default.frag");
}

Scene::~Scene() {
  c_shader->Delete();
}



bool Scene::LoadModel(const std::string& p_filePath){
    try {
        c_sceneObjects.push_back(std::make_unique<Model>(p_filePath));
        return true;
    } catch (std::exception& eee){ 
        std::cerr << "Scene::LoadModel failed on '" << p_filePath << "': " << eee.what() << std::endl;
        return false;
    }
}


void Scene::Update(GLFWwindow* p_window, double p_deltaTime){
    // debug REMOVE
    
    debug_time_iii += (p_deltaTime);
    if(debug_time_iii > 4.0 && !(debug_time_iii_flag)) {
        std::string textureDir = "resource/Textures/";
        LoadModel((textureDir + "popcat.png"));
        debug_time_iii_flag = true;
        std::cout << "loaded" << std::endl;
    }

    // inputs
    c_camera->HandleInputs(p_window, p_deltaTime); 
    c_camera->UpdateMatrix(45.0, 1.0, 100.0);

    // debug  REMOVE
    std::cout << "FPS: " << (1.0 / p_deltaTime) << '\n';

}

void Scene::Render(GLFWwindow* p_window, double p_deltaTime) { 
    // opengl things
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]); // tanggalin to
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw all the objects now
    c_shader->Activate();
    for(auto& iii_object: c_sceneObjects){
        iii_object->Draw(*c_shader, *c_camera);
    }

    glfwSwapBuffers(p_window); // or p_window.ID
}


