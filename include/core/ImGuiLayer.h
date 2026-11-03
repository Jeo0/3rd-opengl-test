#pragma once
#include "GLFW/glfw3.h"
#include "core/InputFrame.h"   // UiCapture

class ImGuiLayer {
private:
    bool c_visible = true;

public:
    ImGuiLayer(GLFWwindow* p_window, const char* p_glslVersion = "#version 450");
    ~ImGuiLayer();

    // owns a global context, copying it would double-shutdown
    ImGuiLayer(const ImGuiLayer&) = delete;
    ImGuiLayer& operator=(const ImGuiLayer&) = delete;

    void BeginFrame();
    void EndFrame();                    // call BEFORE glfwSwapBuffers

    UiCapture GetCapture() const;
    void SetMouseEnabled(bool p_enabled);

    bool IsVisible() const { return c_visible; }
    void ToggleVisible() { c_visible = !c_visible; }
};
