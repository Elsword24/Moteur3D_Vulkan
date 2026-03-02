//#include "camera.h"
//
//void Camera::processKeyboard(CameraMovement direction, float deltaTime)
//{
//	float velocity = m_movementSpeed * deltaTime;
//
//	switch (direction)
//	{
//	case CameraMovement::FORWARD:
//		m_position += m_front * velocity;
//		break;
//	case CameraMovement::BACKWARD:
//		m_position -= m_front * velocity;
//		break;
//	case CameraMovement::LEFT:
//		m_position += m_right * velocity;
//		break;
//	case CameraMovement::RIGHT:
//		m_position -= m_right * velocity;
//		break;
//	case CameraMovement::UP:
//		m_position += m_up * velocity;
//		break;
//	case CameraMovement::DOWN:
//		m_position -= m_up * velocity;
//		break;
//	}
//
//}
//
//void Camera::processInput(Window& window, Camera& camera, float deltaTime)
//{
//
//	if (window.GetKey(input::keyPressed(input::KEY::KEY_Z)) == input::inputState::PRESSED)
//	{
//		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
//	}
//	if (window.GetKey(input::keyPressed(input::KEY::KEY_S)) == input::inputState::PRESSED)
//	{
//		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
//	}
//	if (window.GetKey(input::keyPressed(input::KEY::KEY_D)) == input::inputState::PRESSED)
//	{
//		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
//	}
//	if (window.GetKey(input::keyPressed(input::KEY::KEY_Q)) == input::inputState::PRESSED)
//	{
//		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
//	}
//
//	if (window.GetKey(input::keyPressed(input::KEY::KEY_W)) == input::inputState::PRESSED)
//		camera.processKeyboard(CameraMovement::UP, deltaTime);
//	if (window.GetKey(input::keyPressed(input::KEY::KEY_X)) == input::inputState::PRESSED)
//		camera.processKeyboard(CameraMovement::DOWN, deltaTime);
//}
//
//void Camera::setupInputCallbacks()
//{
//	glfwSetWindowUserPointer(m_window->getGLFWWindow(), this);
//	glfwSetCursorPosCallback(m_window->getGLFWWindow(), mouseCallback);
//	glfwSetScrollCallback(m_window->getGLFWWindow(), scrollCallback);
//	glfwSetInputMode(m_window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//}
//
//void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos)
//{
//
//	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
//	static bool firstMouse = true;
//	static float lastX = 400.0f, lastY = 300.0f;
//
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = ypos - lastY;
//
//	lastX = xpos;
//	lastY = ypos;
//
//	cam->processMouseMovement(xoffset, yoffset);
//
//}
//void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	auto cameraptr = (glfwGetWindowUserPointer(window));
//	Camera* camera = static_cast<Camera*> (cameraptr);
//	camera->processMouseScroll(yoffset);
//}
//
//void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
//{
//	xOffset *= m_mouseSensitivity;
//	yOffset *= m_mouseSensitivity;
//
//	m_yaw += xOffset;
//	m_pitch += yOffset;
//
//	if (constrainPitch)
//	{
//		m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
//	}
//
//	updateCameraVectors();
//}
//
//void Camera::processMouseScroll(float yOffset)
//{
//	m_zoom -= yOffset;
//	m_zoom = std::clamp(m_zoom, 0.0f, 60.0f);
//}
//
//void Camera::updateCameraVectors()
//{
//	glm::vec3 newFront;
//	newFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
//	newFront.y = sin(glm::radians(m_pitch));
//	newFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
//	m_front = glm::normalize(newFront);
//
//	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
//	m_up = glm::normalize(glm::cross(m_right, m_front));
//}