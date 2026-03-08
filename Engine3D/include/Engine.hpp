#ifndef ENGINE_HPP
#define ENGINE_HPP
#pragma once

#include <memory>
#include <chrono>

#include "window.h"
#include "API_Vulkan.h"
#include "Renderer.h"
#include "Physics.h"
#include "SceneManager.h"
#include "EntityEventSystem.h"
#include "EventBus.h"
#include "InputMapper.h"


class EngineQVY
{
private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<VulkanRAII> m_Vulkan;
	std::unique_ptr<Renderer> m_Renderer;
	std::unique_ptr<Physics::PhysicsSystem> m_Physics;
	std::unique_ptr<SceneManager> m_SceneManager;
	std::unique_ptr<EntityEventSystem> m_EntityEventSystem;
	InputMapper m_inputManager;

	

private:
	void SettingVulkan(const char* Title)
	{
		m_Vulkan->createInstance(Title);
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
		m_EntityEventSystem = std::make_unique<EntityEventSystem>();

		m_Window = std::make_unique<Window>(Title, Width, Height);
		m_Vulkan = std::make_unique<VulkanRAII>(m_Window.get()->getGLFWWindow());
		SettingVulkan(Title);

		m_Physics = std::make_unique<Physics::PhysicsSystem>();
		m_SceneManager = std::make_unique<SceneManager>(m_Physics.get());

		// Set event bus to queue mode for better performance and to avoid issues with events being processed while entities are being destroyed
		EventBus::Get().SetImmediateMode(false);


		m_Renderer = std::make_unique<Renderer>(m_Vulkan.get());

		glfwSetInputMode(m_Window.get()->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		m_inputManager = InputMapper::GetInstance();
		m_inputManager.Init(m_Window.get()->getGLFWWindow());
	}
	~EngineQVY() = default;

	void RunGameLoop()
	{
		static auto previous = std::chrono::high_resolution_clock::now();
		while (!m_Window->WindowClosed())
		{
			auto current = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration<float, std::milli>(current - previous).count();
			previous = current;
			m_Window->PollEvent();
			m_inputManager.Update();
			m_Renderer->drawFrame();
		}
		m_Vulkan->GetDevice().waitIdle();
		m_Window->cleanup();
	}

};

#endif
