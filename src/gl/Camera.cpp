#include "Camera.h"
#include "core/core.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include <cstdlib>

#define GLM_ENABLE_EXPERIMENTAL // warning experimental feature
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


void Camera::HandleInputs(GLFWwindow* pWindowID, double pDeltaTime){

	// Handles key inputs
	if (glfwGetKey(pWindowID, GLFW_KEY_W) == GLFW_PRESS) {
		c_position += c_speed * c_orientation * static_cast<float>(pDeltaTime);
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_A) == GLFW_PRESS) {
		c_position += 
            c_speed * -glm::normalize(glm::cross(c_orientation, c_up)) * static_cast<float>(pDeltaTime) ;
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_S) == GLFW_PRESS) {
		c_position += c_speed * -c_orientation * static_cast<float>(pDeltaTime);
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_D) == GLFW_PRESS) {
		c_position += 
            c_speed * glm::normalize(glm::cross(c_orientation, c_up)) * static_cast<float>(pDeltaTime);
	}

	if (glfwGetKey(pWindowID, GLFW_KEY_SPACE) == GLFW_PRESS) {
		c_position += c_speed * c_up * static_cast<float>(pDeltaTime);
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		c_position += c_speed * -c_up * static_cast<float>(pDeltaTime);
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		// c_speed = 0.1f;
		c_speed = 100.0f * pDeltaTime;
	}
	else if (glfwGetKey(pWindowID, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		// c_speed = 0.04f;
		c_speed = 20.0f * pDeltaTime;
	}



    // mouse inputs
	if (glfwGetMouseButton(pWindowID, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		// glfwSetInputMode(pWindowID, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // not working on wayland
		glfwSetInputMode(pWindowID, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Prevents camera from jumping on the first click
		if (c_firstClick)
		{
			glfwSetCursorPos(pWindowID, (c_width / 2), (c_height / 2));
			c_firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(pWindowID, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = c_sensitivity * (float)(mouseY - (c_height / 2)) / c_height;
		float rotY = c_sensitivity * (float)(mouseX - (c_width / 2)) / c_width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation =  glm::rotate(c_orientation, 
                                                glm::radians(-rotX),  
                                                glm::normalize(glm::cross(c_orientation, c_up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, c_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			c_orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		c_orientation = glm::rotate(c_orientation, glm::radians(-rotY), c_up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(pWindowID, (c_width / 2), (c_height / 2));
	}
	else if (glfwGetMouseButton(pWindowID, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(pWindowID, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		c_firstClick = true;
	}

}
