#pragma once
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS


#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

//#define GLFW_INCLUDE_VULKAN        // REQUIRED only for GLFW CreateWindowSurface.
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>


#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"


enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class Camera
{
public:
	//Camera(
	//	glm::vec3 front,
	//	glm::vec3 right,
	//	glm::vec3 worldUp,
	//	float movementSpeed,
	//	float mouseSensitivity,
	//	float zoom
	//) : m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	//	m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
	//	m_yaw(-90.0f),
	//	m_pitch(0.0f),
	//	m_front(front),
	//	m_right(right),
	//	m_worldUp(worldUp),
	//	m_movementSpeed(movementSpeed),
	//	m_mouseSensitivity(mouseSensitivity),
	//	m_zoom(zoom)
	//{
	//}

	Camera(Window* window) : m_window(window)
	{
		m_position = glm::vec3(0.0f, 0.0f, 3.0f);
		m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_yaw = -90.0f;
		m_pitch = 0.0f;
		m_movementSpeed = 0.5f;
		m_mouseSensitivity = 0.1f;
		m_zoom = 65.0f;

		updateCameraVectors();
	}


	void processKeyboard(CameraMovement direction, float deltaTime);
	void processInput(Window& window, Camera& camera, float deltaTime);

	void setupInputCallbacks();
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void updateCameraVectors();
	void processMouseScroll(float yOffset);

	glm::vec3 getPosition() const { return m_position; }
	glm::vec3 getFront() const { return m_front; }
	float getZoom() const { return m_zoom; }

	glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}
	glm::mat4 getProjectionMatrix(float aspectRatio, float nearPlane = 0.1f, float farPlane = 100.0f) const
	{
		return glm::perspective(glm::radians(m_zoom), aspectRatio, nearPlane, farPlane);
	}

	void SetPosition(glm::vec3 position)
	{
		m_position = position;
	}
	void SetFront(glm::vec3 front)
	{
		m_front = front;
	}
	void SetRight(glm::vec3 right)
	{
		m_right = right;
	}
	void SetUp(glm::vec3 up)
	{
		m_up = up;
	}

	glm::vec3 GetWordlUp()
	{
		return m_worldUp;
	}
	glm::vec3 GetRight()
	{
		return m_right;
	}

private:
	glm::vec3 m_position;
	glm::vec3 m_front = { 1,1,1 };
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;
	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

	Window* m_window;
};
