#include "core/InputHandler.h"

bool InputHandler::IsDown(GLFWwindow* p_window, int p_key) {
    return glfwGetKey(p_window, p_key) == GLFW_PRESS;
}

bool InputHandler::WasPressed(GLFWwindow* p_window, int p_key, bool& p_previousState) {
    const bool m_currentState = IsDown(p_window, p_key);

    const bool m_pressed =
        m_currentState && !p_previousState;

    p_previousState = m_currentState;

    return m_pressed;
}


InputFrame InputHandler::Poll(GLFWwindow* p_window) {
    InputFrame m_input;

    // ============================
    // camera movement

    m_input.camera.forward =
        IsDown(p_window, GLFW_KEY_W)
        - IsDown(p_window, GLFW_KEY_S);

    m_input.camera.right =
        IsDown(p_window, GLFW_KEY_D)
        - IsDown(p_window, GLFW_KEY_A);

    m_input.camera.vertical =
        IsDown(p_window, GLFW_KEY_SPACE)
        - IsDown(p_window, GLFW_KEY_LEFT_CONTROL);

    m_input.camera.sprint =
        IsDown(p_window, GLFW_KEY_LEFT_SHIFT);


    // ============================
    // surface pass

    m_input.edgeWidthAxis =
        IsDown(p_window, GLFW_KEY_RIGHT_BRACKET)
        - IsDown(p_window, GLFW_KEY_LEFT_BRACKET);


    // ============================
    // point overlay pass

    m_input.pointSizeAxis =
        IsDown(p_window, GLFW_KEY_EQUAL)
        - IsDown(p_window, GLFW_KEY_MINUS);

    m_input.togglePoints =
        WasPressed(p_window, GLFW_KEY_P, c_previousP);

    m_input.togglePointXray =
        WasPressed(p_window, GLFW_KEY_X, c_previousX);


    // ============================
    // mouse input

    const bool m_leftMouseDown =
        glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT)
        == GLFW_PRESS;

    if (m_leftMouseDown) {

        if (!c_mouseCaptured) {
            glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            c_mouseCaptured = true;
            c_firstMouse = true;
        }

        double m_mouseX;
        double m_mouseY;

        glfwGetCursorPos(p_window, &m_mouseX, &m_mouseY);

        if (c_firstMouse) {
            c_lastMouseX = m_mouseX;
            c_lastMouseY = m_mouseY;
            c_firstMouse = false;
        }
        else {
            m_input.camera.mouseDelta.x = static_cast<float>(m_mouseX - c_lastMouseX);
            m_input.camera.mouseDelta.y = static_cast<float>(m_mouseY - c_lastMouseY);
        }

        c_lastMouseX = m_mouseX;
        c_lastMouseY = m_mouseY;

        m_input.camera.lookActive = true;
    }
    else if (c_mouseCaptured) {
        glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        c_mouseCaptured = false;
        c_firstMouse = true;
    }

    return m_input;
}
