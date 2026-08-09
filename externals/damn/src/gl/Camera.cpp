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



Camera::Camera(int pHeight, int pWidth, glm::vec3 pPosition) {
    c_height = pHeight;
    c_width = pWidth;
}

Camera::~Camera() {}

void Camera::Matrix(float pFOVdeg, float pNearPlane, float pFarPlane, Shader& pShader, const char* pUniform){
    glm::mat4 mview = glm::mat4(1.0);
    glm::mat4 mprojection = glm::mat4(1.0);
    
    mview = glm::lookAt(c_position, c_position + c_orientation, c_up);

    // enables perspective 
    mprojection = glm::perspective(glm::radians(pFOVdeg), (float)c_width/c_height, pNearPlane, pFarPlane);

    glUniformMatrix4fv(glGetUniformLocation(pShader.ID, pUniform), 1, GL_FALSE, glm::value_ptr(mprojection * mview));


}


void Camera::HandleInputs(GLFWwindow* pWindowID, float deltaTime){

	// Handles key inputs
	if (glfwGetKey(pWindowID, GLFW_KEY_W) == GLFW_PRESS)
	{
		c_position += c_speed * deltaTime * c_orientation;
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_A) == GLFW_PRESS)
	{
		c_position += c_speed * deltaTime * -glm::normalize(glm::cross(c_orientation, c_up));
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_S) == GLFW_PRESS)
	{
		c_position += c_speed * deltaTime * -c_orientation;
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_D) == GLFW_PRESS)
	{
		c_position += c_speed * deltaTime * glm::normalize(glm::cross(c_orientation, c_up));
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		c_position += c_speed * deltaTime * c_up;
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		c_position += c_speed * deltaTime * -c_up;
	}
	if (glfwGetKey(pWindowID, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		// previously a flat tiny constant to fake-compensate for the missing
		// deltaTime scaling above; now that movement is genuinely per-second,
		// sprint is just a multiplier on the base speed.
		c_speed = 4.0f;
	}
	else if (glfwGetKey(pWindowID, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		c_speed = 2.0f;
	}



    // mouse inputs
	if (glfwGetMouseButton(pWindowID, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		// glfwSetInputMode(pWindowID, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // not working on wayland
		glfwSetInputMode(pWindowID, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Prevents camera from jumping on the first click
		if (cFirstClick)
		{
			glfwSetCursorPos(pWindowID, (c_width / 2), (c_height / 2));
			cFirstClick = false;
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
