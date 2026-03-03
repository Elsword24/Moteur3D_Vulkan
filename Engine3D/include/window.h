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

private:
	GLFWwindow* m_window = nullptr;
	bool framebufferResized = false;
};