#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include <utility>

#include "Component.h"

class Entity
{
private:
    std::vector<std::unique_ptr<Component>> components;
    std::unordered_map<std::size_t, Component*> componentMap;
    std::string name;
    bool active = true;

public:
    explicit Entity(std::string entityName) : name(std::move(entityName)) {}

    const std::string& GetName() const { return name; }
    bool isActive() const { return active; }
    void setActive(bool isActive_) { active = isActive_; }

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        std::size_t typeID = Component::GetTypeID<T>();

        auto it = componentMap.find(typeID);
        if (it != componentMap.end())
            return static_cast<T*>(it->second);

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* componentPtr = component.get();

        componentPtr->SetOwner(this);

        componentMap[typeID] = componentPtr;
        components.push_back(std::move(component));
        return componentPtr;
    }

    template<typename T>
    T* GetComponent()
    {
        std::size_t typeID = Component::GetTypeID<T>(); 
        auto it = componentMap.find(typeID);
        if (it != componentMap.end())
            return static_cast<T*>(it->second);
        return nullptr;
    }

    template <typename T>
    bool RemoveComponent()
    {
        std::size_t typeID = Component::GetTypeID<T>();
        auto it = componentMap.find(typeID);
        if (it == componentMap.end())
            return false;

        Component* componentPtr = it->second;
        componentMap.erase(it);

        for (auto compIt = components.begin(); compIt != components.end(); ++compIt)
        {
            if (compIt->get() == componentPtr)
            {
                components.erase(compIt);
                return true;
            }
        }
        return false;
    }
};
