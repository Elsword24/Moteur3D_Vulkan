#include "Engine.hpp"

#include <chrono>

#include "EventBus.h"


void EngineQVY::SettingVulkan(const char* Title)
{
	m_Vulkan->createInstance(Title);
	m_Vulkan->setupDebugMessenger();
	m_Vulkan->createSurface(m_Window.get()->getGLFWWindow());
	m_Vulkan->pickPhysicalDevice();
	m_Vulkan->createLogicalDevice();
	m_Vulkan->createSwapChain();
	m_Vulkan->createImageViews();
	m_Vulkan->createCommandPool();
}

EngineQVY::EngineQVY(const char* Title, uint32_t Width, uint32_t Height)
{
	m_EntityEventSystem = std::make_unique<EntityEventSystem>();

	m_Window = std::make_unique<Window>(Title, Width, Height);
	m_Vulkan = std::make_unique<VulkanRAII>(m_Window.get()->getGLFWWindow());
	SettingVulkan(Title);

	m_Physics = std::make_unique<Physics::PhysicsSystem>();
	m_SceneManager = std::make_unique<SceneManager>(m_Physics.get());

	// Set event bus to queue mode for better performance and to avoid issues with events being processed while entities are being destroyed
	EventBus::Get().SetImmediateMode(true);


	m_Renderer = std::make_unique<Renderer>(m_Vulkan.get());

	glfwSetInputMode(m_Window.get()->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	InputMapper::GetInstance().Init(m_Window.get()->getGLFWWindow());
	m_RailShooter = std::make_unique<RailShooter>(this);
	m_RailShooter->SettingWorld(m_SceneManager.get());
}

void EngineQVY::RunGameLoop()
{
	static auto previous = std::chrono::high_resolution_clock::now();
	while (!m_Window->WindowClosed())
	{
		auto current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration<float>(current - previous).count();
		previous = current;

		m_Window->PollEvent();
		InputMapper::GetInstance().Update();

		m_RailShooter->Update(elapsed);
		m_Renderer->drawFrame();
		m_RailShooter->Cleanup();
	}
	m_Vulkan->GetDevice().waitIdle();
	m_Window->cleanup();
}

const std::unique_ptr<Physics::PhysicsSystem>& EngineQVY::GetPhysicSystem() const
{
	return m_Physics;
}

const std::unique_ptr<Window>& EngineQVY::GetWindow() const
{
	return m_Window;
}

const std::unique_ptr<VulkanRAII>& EngineQVY::GetVulkan() const
{
	return m_Vulkan;
}