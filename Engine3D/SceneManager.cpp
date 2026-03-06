#include "SceneManager.h"

SceneManager::SceneManager(Physics::PhysicsSystem* physicsSystem) : physics(physicsSystem)
{
	EventBus::Get().AddListener(this, EventCategory::Application, 0);
}

SceneManager::~SceneManager()
{
	EventBus::Get().RemoveListener(this);
}

Entity*	SceneManager::CreateEntity(const std::string& name)
{
	auto entity = std::make_unique<Entity>(name);
	Entity* ptr = entity.get();
	entities.push_back(std::move(entity));
	return ptr;
}

void SceneManager::OnEvent(const Event& event)
{
	EventDispatcher dispatcher(event);

	dispatcher.Dispatch<DestroyEntityEvent>([this](const DestroyEntityEvent& e)
		{
			DestroyEntity(e.GetTarget());
		}
	);
}

void SceneManager::CleanupDestroyedEntities()
{
	for (auto& entity : entities)
	{
		if (!entity || !entity->isPendingDestroy())
			continue;

		PreDestroyEntity(entity.get());
	}

	entities.erase(std::remove_if(entities.begin(), entities.end(),
		[](const std::unique_ptr<Entity>& entity)
		{
			return !entity || entity->isPendingDestroy();
		}), entities.end());
}

void SceneManager::DestroyEntity(Entity* entity)
{
	if (!entity) return;
	if (entity->isPendingDestroy()) return;
	std::printf("[SceneManager] MarkForDestroy\n");
	entity->MarkForDestroy();
}

void SceneManager::PreDestroyEntity(Entity* entity) const
{
	if (!entity) return;

	if (auto rbComp = entity->GetComponent<RigidBodyComponent>())
	{
		if (auto rb = rbComp->GetRigidBody())
		{
			if (physics)
			{
				physics->DestroyRigidBody(rb);
			}
		}
	}
}