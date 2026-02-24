#pragma once
#include "input.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

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