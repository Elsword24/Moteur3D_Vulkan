#ifndef ENGINE_HPP
#define ENGINE_HPP
#pragma once

#include <memory>
#include "InputMapper.h"
#include "World.h"
#include "window.h"
#include "API_Vulkan.h"
#include "Renderer.h"
#include "Physics.h"
#include "SceneManager.h"
#include "EntityEventSystem.h"

//class Window;
//class VulkanRAII;
//class Renderer;
//class SceneManager;
//class EntityEventSystem;
//class RailShooter;

namespace Physics
{
	class PhysicsSystem;
}


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
	std::unique_ptr<RailShooter> m_RailShooter;

	

private:
	void SettingVulkan(const char* Title);

public:
	EngineQVY(const char* Title, uint32_t Width, uint32_t Height);
	~EngineQVY() = default;

	void RunGameLoop();

	const std::unique_ptr<Physics::PhysicsSystem>& GetPhysicSystem() const;

	const std::unique_ptr<Window>& GetWindow() const;
};

#endif
