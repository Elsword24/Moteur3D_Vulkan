#pragma once

#include "EventBus.h"
#include "EventListener.h"
#include "Event.h"

class EntityEventSystem : public EventListener
{
public:
	EntityEventSystem()
	{
		EventBus::Get().AddListener(this, EventCategory::Application, 0);
	}
	~EntityEventSystem() override
	{
		EventBus::Get().RemoveListener(this);
	}
	void OnEvent(const Event& event) override
	{
		std::printf("[EntityEventSystem::OnEvent] received %s\n", event.GetType());

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<HitEvent>([&](const HitEvent& e)
			{
				std::printf("[EntityEventSystem] Dispatch HitEvent\n");
				HandlerHitEvent(e);
			});

		dispatcher.Dispatch<CollisionEvent>([&](const CollisionEvent& e)
			{
				std::printf("[EntityEventSystem] Dispatch CollisionEvent\n");
				HandleCollisionEvent(e);
			});
	}

private:
	void HandlerHitEvent(const HitEvent& event)
	{
		Entity* target = event.GetTarget();
		if (!target) return;
		DestroyEntityEvent destroyEvent(target);
		EventBus::Get().PublishEvent(destroyEvent);
	}

	void HandleCollisionEvent(const CollisionEvent& event)
	{
		Entity* a = event.GetEntityA();
		Entity* b = event.GetEntityB();

		if (a)
		{
			DestroyEntityEvent destroyA(a);
			EventBus::Get().PublishEvent(destroyA);
		}

		if (b)
		{
			DestroyEntityEvent destroyB(b);
			EventBus::Get().PublishEvent(destroyB);
		}
	}
};