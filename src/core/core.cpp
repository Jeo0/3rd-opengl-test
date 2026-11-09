#include "core/core.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "atg/Sampling.h"

Core::Core() :  windowObj(800, 800, "gademNoice", NULL, NULL, {0.01, 0.24, 0.45, 1.0})
{
    Init();
}

Core::~Core() { Cleanup(); }

void Core::Run() {
    while (!glfwWindowShouldClose(windowObj.ID)) {
        glfwPollEvents();           // poll first so InputHandler::Poll reads this frame's state

        // window resize
        if(windowObj.PollResize() && !windowObj.IsMinimized()){
            glViewport(0, 0, windowObj.c_width, windowObj.c_height);
            c_scene->Resize(windowObj.c_width, windowObj.c_height);
        }
        if (windowObj.IsMinimized()) {      // 0x0 framebuffer -> aspect would divide by zero
            glfwWaitEvents();               // sleep until restored instead of spinning
            continue;
        }

        // updates WantCaptureMouse/Keyboard for this frame's Poll
        c_imguiLayer->BeginFrame();     

        float mDeltaTime = cLimiter.StartFrame();
        Update(mDeltaTime);
        Render(mDeltaTime);         // buffer swap

        cLimiter.EndFrame();
    }
}

void Core::Init() {
    // ============================
    // ImGui 
    c_imguiLayer = std::make_unique<ImGuiLayer>(windowObj.ID);


    // ============================
    // opengl settings
    glEnable(GL_DEPTH_CLAMP);   // stops the clipping of camera; it clamps the depth instead
                                // debug REMOVE
    glEnable(GL_DEPTH_TEST);

    // ============================
    // object instantiations
    c_scene = std::make_unique<Scene>(windowObj.c_height, windowObj.c_width);
    c_setset = std::make_unique<Settings>(cLimiter);
    c_inputHandler = std::make_unique<InputHandler>();


    std::string linuxm_models[] {
        "resource/Models/simple_cube/cube2.gltf",                   // 0
        "resource/Models/simple_cube/simple_cube.gltf",             // 1
        "resource/Models/simple_cone/simple_cone.gltf",             // 2
        "resource/Models/simple_cylinder/simple_cylinder.gltf",     // 3
        "resource/Models/simple_sphere/simple_sphere.gltf",         // 4
        "resource/Models/simple_torus/simple_torus.gltf",           // 5
        "resource/Models/simple_triangularPrism/simple_triangularPrism.gltf",  // 6

        "resource/Models/complex_tricycle/scene.gltf",              // 7
        "resource/Models/complex_spear/scene.gltf",                 // 8
        "resource/Models/complex_metaCube/complexmetaCube.gltf",    // 9
        "resource/Models/complex_suzanne/complex_suzanne.gltf",     // 10
        "resource/Models/complex_metaCube/complexmetaCube.gltf"    // 11
    };
    c_scene->LoadModel(linuxm_models[8]);


    // std::string windowsm_models[] {
    //     "resource\\Models\\tricycle\\scene.gltf",
    //     "resource\\Models\\simpleCube\\cube2.gltf",
    //     "resource\\Models\\simpleCube\\simplecube.gltf",
    //     "resource\\Models\\spear\\scene.gltf"
    // };
    // c_scene->LoadModel(windowsm_models[3]);
}


void Core::DrawUI() {
    ImGui::Begin("core");
    ImGui::Text("%.1f fps (%.2f ms)", ImGui::GetIO().Framerate, 1000.0 / ImGui::GetIO().Framerate);

    float m_fps = cLimiter.GetTargetFPS();      // add this getter
    if (ImGui::SliderFloat("fps cap", &m_fps, 0.0, 240.0, "%.0f")) cLimiter.SetTargetFPS(m_fps);

    if (ImGui::Button("reload settings.json")) c_setset->LoadSettings(cLimiter);

    ImGui::End();
}


void Core::Update(double p_deltaTime) {
    /*  LOGIC TINGS */

    // input layer
    const InputFrame m_input = c_inputHandler->Poll(windowObj.ID, c_imguiLayer->GetCapture());
    c_imguiLayer->SetMouseEnabled(!c_inputHandler->IsMouseCaptured());

    // scene only ever sees the snapshot, never the GLFWwindow*
    c_scene->Update(m_input, p_deltaTime);

    // ============================
    // ui (immediate mode, so building widgets is just more logic)
    if (m_input.toggleUi) c_imguiLayer->ToggleVisible();
    if (c_imguiLayer->IsVisible()) {
        DrawUI();           // core ui
        c_scene->DrawUI();  // scene ui
    }
}



void Core::Render(double p_deltaTime) {
    /*  RENDER TINGS */
    // c_scene->Render(windowObj.ID, p_deltaTime);
    c_scene->Render(p_deltaTime);       // no swap in here anymore
    c_imguiLayer->EndFrame();           // ui on top of the scene
    glfwSwapBuffers(windowObj.ID);
}

void Core::Cleanup() {
    c_imguiLayer.reset();
    c_scene.reset();

    glfwDestroyWindow(windowObj.ID);
    glfwTerminate();
}


