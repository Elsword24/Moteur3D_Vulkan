#ifndef ENGINE_HPP
#define ENGINE_HPP
#pragma once

#include <memory>

#include "window.h"
#include "vulkan.h"
#include "Renderer.h"


class EngineQVY
{
private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<VulkanRAII> m_Vulkan;
	std::unique_ptr<Renderer> m_Renderer;

private:
	void SettingVulkan()
	{
		m_Vulkan->createInstance();
		m_Vulkan->setupDebugMessenger();
		m_Vulkan->createSurface(m_Window.get()->getGLFWWindow());
		m_Vulkan->pickPhysicalDevice();
		m_Vulkan->createLogicalDevice();
		m_Vulkan->createSwapChain();
		m_Vulkan->createImageViews();
		m_Vulkan->createCommandPool();

		//TODO: A ranger dans Renderer
		/*m_Vulkan->createDescriptorSetLayout();
		m_Vulkan->createGraphicsPipeline();*/

		//TODO: A Ranger dans MeshComponent
		/*m_Vulkan->createVertexBuffer();
		m_Vulkan->createIndexBuffer();*/

	}

	
public:
	EngineQVY(const char* Title, uint32_t Width, uint32_t Height)
	{
		m_Window = std::make_unique<Window>(Title, Width, Height);
		m_Vulkan = std::make_unique<VulkanRAII>(m_Window.get()->getGLFWWindow());
		SettingVulkan();
		m_Renderer = std::make_unique<Renderer>(m_Vulkan.get());
	}
	~EngineQVY() = default;

	void RunGameLoop()
	{
		while (!m_Window->WindowClosed())
		{
			m_Window->PollEvent();
		}
	}

};

#endif
