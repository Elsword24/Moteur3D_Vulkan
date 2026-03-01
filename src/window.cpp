#include "window.h"


void Window::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);
}

int Window::WindowClosed()
{
	return glfwWindowShouldClose(m_window);
}

void Window::PollEvent()
{
	glfwPollEvents();
}

int Window::GetKey(int key)
{
	return glfwGetKey(m_window, key);
}

void Window::cleanup()
{
	glfwDestroyWindow(m_window);

	glfwTerminate();
}

void Window::WaitEvents()
{
	glfwWaitEvents();
}

void Window::GetFramebufferSize(int& width, int& height)
{
	glfwGetFramebufferSize(m_window, &width, &height);
}

GLFWwindow* Window::getGLFWWindow() const
{
	return m_window;
}


void Window::setUserPointer(void* ptr)
{
	glfwSetWindowUserPointer(m_window, ptr);
}

void* Window::getUserPointer() const
{
	return glfwGetWindowUserPointer(m_window);
}

void Window::setMouseCallback(GLFWcursorposfun callback)
{
	glfwSetCursorPosCallback(m_window, callback);
}

void Window::setScrollCallback(GLFWscrollfun callback)
{
	glfwSetCursorPosCallback(m_window, callback);
}


//vk::Extent2D Window::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
//{
//	if (capabilities.currentExtent.width != 0xFFFFFFFF)
//	{
//		return capabilities.currentExtent;
//	}
//	int width, height;
//
//	GetFramebufferSize(width, height);
//
//	return {
//		std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
//		std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height) };
//}