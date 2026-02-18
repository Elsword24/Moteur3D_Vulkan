#pragma once
#include "input.h"


constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

class Window
{
public:
	Window() 
	{

	}

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(m_window, this);
	}

	int WindowClosed()
	{
		return glfwWindowShouldClose(m_window);
	}

	void PollEvent()
	{
		glfwPollEvents();
	}

	int GetKey(int key)
	{
		return glfwGetKey(m_window, key);
	}

	void cleanup()
	{
		glfwDestroyWindow(m_window);

		glfwTerminate();
	}

	void WaitEvents()
	{
		glfwWaitEvents();
	}

	void GetFramebufferSize(int& width, int& height)
	{
		glfwGetFramebufferSize(m_window, &width, &height);
	}

	GLFWwindow* getGLFWWindow() const
	{
		return m_window;
	}
	

	void setUserPointer(void* ptr)
	{
		glfwSetWindowUserPointer(m_window, ptr);
	}

	void* getUserPointer() const
	{
		return glfwGetWindowUserPointer(m_window);
	}

	void setMouseCallback(GLFWcursorposfun callback)
	{
		glfwSetCursorPosCallback(m_window, callback);
	}

	void setScrollCallback(GLFWscrollfun callback)
	{
		glfwSetCursorPosCallback(m_window, callback);
	}

private:
	GLFWwindow* m_window = nullptr;
	bool framebufferResized = false;
};