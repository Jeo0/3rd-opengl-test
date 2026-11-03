#include "core/ImGuiLayer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

ImGuiLayer::ImGuiLayer(GLFWwindow* p_window, const char* p_glslVersion) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& m_io = ImGui::GetIO();
    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_io.IniFilename = "resource/imgui.ini";

    ImGui::StyleColorsDark();

    // true = imgui installs its glfw callbacks and chains any that already exist
    ImGui_ImplGlfw_InitForOpenGL(p_window, true);
    ImGui_ImplOpenGL3_Init(p_glslVersion);     // matches the 4.5 core context in Window
}

ImGuiLayer::~ImGuiLayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::EndFrame() {
    // must run every frame NewFrame ran, even when nothing is visible
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UiCapture ImGuiLayer::GetCapture() const {
    const ImGuiIO& m_io = ImGui::GetIO();
    return { m_io.WantCaptureMouse, m_io.WantCaptureKeyboard };
}

void ImGuiLayer::SetMouseEnabled(bool p_enabled) {
    ImGuiIO& m_io = ImGui::GetIO();
    if (p_enabled) m_io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    else           m_io.ConfigFlags |=  ImGuiConfigFlags_NoMouse;
}
