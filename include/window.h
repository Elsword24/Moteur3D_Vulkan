#pragma once
#include "input.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

class Window
{
public:
	Window(){}

	void initWindow();
	int WindowClosed();
	void PollEvent();
	int GetKey(int key);
	void cleanup();
	void WaitEvents();
	void GetFramebufferSize(int& width, int& height);
	GLFWwindow* getGLFWWindow() const;
	void setUserPointer(void* ptr);
	void* getUserPointer() const;
	void setMouseCallback(GLFWcursorposfun callback);
	void setScrollCallback(GLFWscrollfun callback);
	//vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
	//void createSurface()
	//{
	//	VkSurfaceKHR _surface;
	//	if (glfwCreateWindowSurface(*instance, m_window, nullptr, &_surface) != 0)
	//	{
	//		throw std::runtime_error("failed to create window surface!");
	//	}
	//	surface = vk::raii::SurfaceKHR(instance, _surface);
	//}

private:
	GLFWwindow* m_window = nullptr;
	bool framebufferResized = false;
};