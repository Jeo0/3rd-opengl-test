#include "Camera.h"
// #include "core/core.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include <cstdlib>

#define GLM_ENABLE_EXPERIMENTAL 
#include "glm/gtx/rotate_vector.hpp" // for rotate
#include "glm/gtx/vector_angle.hpp" // for angle
#include "glm/geometric.hpp"



Camera::Camera(int p_height, int p_width, glm::vec3 p_position) {
    c_height = p_height;
    c_width = p_width;
    c_position = p_position;
}

Camera::~Camera() {}

void Camera::Matrix(Shader& p_shader, std::string p_uniform) {
    glUniformMatrix4fv(glGetUniformLocation(p_shader.ID, p_uniform.c_str()), 1, GL_FALSE, glm::value_ptr(c_cameraMatrix));

}


void Camera::UpdateMatrix(float p_FOVdeg, float p_nearPlane, float p_farPlane){
    glm::mat4 mview = glm::mat4(1.0);
    glm::mat4 mprojection = glm::mat4(1.0);
    
    // makes camera look in the right direction from the right position
    mview = glm::lookAt(c_position, c_position + c_orientation, c_up);

    // enables perspective 
    mprojection = glm::perspective(glm::radians(p_FOVdeg), (float)c_width/c_height, p_nearPlane, p_farPlane);

    // now set the object's own matrix
    c_cameraMatrix = mprojection * mview;

}

void Camera::Update(const CameraInput& p_input, double p_deltaTime) {
    const float m_deltaTime = static_cast<float>(p_deltaTime);

    // ============================
    // movement
    const glm::vec3 m_right = glm::normalize(glm::cross(c_orientation, c_up));

    glm::vec3 m_movement(0.0f);

    m_movement += c_orientation * p_input.forward;
    m_movement += m_right * p_input.right;
    m_movement += c_up * p_input.vertical;

    // avoid faster diagonal movement.
    if (glm::length(m_movement) > 0.0f) {
        m_movement = glm::normalize(m_movement);
    }

    const float m_speed = c_moveSpeed * (p_input.sprint ? c_sprintMultiplier : 1.0f);

    c_position += m_movement * m_speed * m_deltaTime;


    // ============================
    // camera rotation
    if (p_input.lookActive) {
        const float m_yaw = glm::radians(-p_input.mouseDelta.x * c_sensitivity);
        const float m_pitch = glm::radians(-p_input.mouseDelta.y * c_sensitivity);
        const glm::vec3 m_rightAxis = glm::normalize(glm::cross(c_orientation, c_up));
        const glm::vec3 m_newOrientation = glm::rotate(c_orientation, m_pitch, m_rightAxis);

        if (std::abs( glm::angle(m_newOrientation, c_up) 
            - glm::radians(90.0f)) 
            <= glm::radians(85.0f)) {
            c_orientation = m_newOrientation;
        }

        c_orientation = glm::rotate(c_orientation, m_yaw, c_up);
    }
}

// void Camera::Update(const CameraInput& p_input, double p_deltaTime) {
// void Camera::HandleInputs(GLFWwindow* p_window, double p_deltaTime){
//
// 	// Handles key inputs
// 	if (glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS) {
// 		c_position += c_speed * c_orientation * static_cast<float>(p_deltaTime);
// 	}
// 	if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS) {
// 		c_position += 
//             c_speed * -glm::normalize(glm::cross(c_orientation, c_up)) * static_cast<float>(p_deltaTime) ;
// 	}
// 	if (glfwGetKey(p_window, GLFW_KEY_S) == GLFW_PRESS) {
// 		c_position += c_speed * -c_orientation * static_cast<float>(p_deltaTime);
// 	}
// 	if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS) {
// 		c_position += 
//             c_speed * glm::normalize(glm::cross(c_orientation, c_up)) * static_cast<float>(p_deltaTime);
// 	}
//
// 	if (glfwGetKey(p_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
// 		c_position += c_speed * c_up * static_cast<float>(p_deltaTime);
// 	}
// 	if (glfwGetKey(p_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
// 		c_position += c_speed * -c_up * static_cast<float>(p_deltaTime);
// 	}
// 	if (glfwGetKey(p_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
// 		// c_speed = 0.1f;
// 		c_speed = 100.0f * p_deltaTime * 10.0;
// 	}
// 	else if (glfwGetKey(p_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
// 	{
// 		// c_speed = 0.04f;
// 		c_speed = 20.0f * p_deltaTime;
// 	}
//
//
//
//     // mouse inputs
// 	if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
// 	{
// 		// Hides mouse cursor
// 		// glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // not working on wayland
// 		glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
// 		// Prevents camera from jumping on the first click
// 		if (c_firstClick)
// 		{
// 			glfwSetCursorPos(p_window, (c_width / 2), (c_height / 2));
// 			c_firstClick = false;
// 		}
//
// 		// Stores the coordinates of the cursor
// 		double mouseX;
// 		double mouseY;
// 		// Fetches the coordinates of the cursor
// 		glfwGetCursorPos(p_window, &mouseX, &mouseY);
//
// 		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
// 		// and then "transforms" them into degrees 
// 		float rotX = c_sensitivity * (float)(mouseY - (c_height / 2)) / c_height;
// 		float rotY = c_sensitivity * (float)(mouseX - (c_width / 2)) / c_width;
//
// 		// Calculates upcoming vertical change in the Orientation
// 		glm::vec3 newOrientation =  glm::rotate(c_orientation, 
//                                                 glm::radians(-rotX),  
//                                                 glm::normalize(glm::cross(c_orientation, c_up)));
//
// 		// Decides whether or not the next vertical Orientation is legal or not
// 		if (abs(glm::angle(newOrientation, c_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
// 		{
// 			c_orientation = newOrientation;
// 		}
//
// 		// Rotates the Orientation left and right
// 		c_orientation = glm::rotate(c_orientation, glm::radians(-rotY), c_up);
//
// 		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
// 		glfwSetCursorPos(p_window, (c_width / 2), (c_height / 2));
// 	}
// 	else if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
// 	{
// 		// Unhides cursor since camera is not looking around anymore
// 		glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
// 		// Makes sure the next time the camera looks around it doesn't jump
// 		c_firstClick = true;
// 	}
//
//
//
//     // debug view sizes: hold to grow/shrink, rate is per second
//     float m_rate = 10.0f * (float)p_deltaTime;
//     if (glfwGetKey(p_window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) c_edgeWidth += m_rate;
//     if (glfwGetKey(p_window, GLFW_KEY_LEFT_BRACKET)  == GLFW_PRESS) c_edgeWidth -= m_rate;
//     if (glfwGetKey(p_window, GLFW_KEY_EQUAL)         == GLFW_PRESS) c_pointSize += 2.0f * m_rate;
//     if (glfwGetKey(p_window, GLFW_KEY_MINUS)         == GLFW_PRESS) c_pointSize -= 2.0f * m_rate;
//     c_edgeWidth = glm::clamp(c_edgeWidth, 0.0f, 20.0f);
//     c_pointSize = glm::clamp(c_pointSize, 1.0f, 64.0f);
//
//     // toggles fire once per press, not every frame the key is held
//     bool m_pDown = glfwGetKey(p_window, GLFW_KEY_P) == GLFW_PRESS;
//     bool m_xDown = glfwGetKey(p_window, GLFW_KEY_X) == GLFW_PRESS;
//     if (m_pDown && !c_keyWasDown[0]) c_showPoints = !c_showPoints;
//     if (m_xDown && !c_keyWasDown[1]) c_pointsXray = !c_pointsXray;
//     c_keyWasDown[0] = m_pDown;
//     c_keyWasDown[1] = m_xDown;
//
// }
