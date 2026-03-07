#pragma once

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <cstring>

#include "Physics.h"
#include "Entity.h"
#include "EventBus.h"
#include "EventListener.h"
#include "BaseComponent.h"

class SceneManager : public EventListener
{
private:
    std::vector<std::unique_ptr<Entity>> entities;
    Physics::PhysicsSystem* physics = nullptr;

public:
    explicit SceneManager(Physics::PhysicsSystem* physicsSystem);
    ~SceneManager() override;

    Entity* CreateEntity(const std::string& name);
    void OnEvent(const Event& event) override;
    void CleanupDestroyedEntities();
    void DestroyEntity(Entity* entity);

private:
    void PreDestroyEntity(Entity* entity) const ;
};