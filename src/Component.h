#pragma once


class Entity;

class ComponentTypeIDSystem
{
private:
	inline static std::size_t nextTypeID = 0;

public:
	template<typename T>
	static size_t getTypeID()
	{
		static size_t typeID = nextTypeID++;
		return typeID;	

	}
};

class Component
{
protected:
	Entity* owner = nullptr;
public:

	enum class State
	{
		Uninitialized,
		Initializing,
		Active,
		Destroying,
		Destroyed
	};
private:
	State state = State::Uninitialized;
public:

	virtual ~Component()
	{
		if (state != State::Destroyed)
		{
			OnDestroy();
			state = State::Destroyed;
		}	
	}

	template<typename T>
	static size_t GetTypeID()
	{
		return ComponentTypeIDSystem::getTypeID<T>();
	}

	void Initialize()
	{
		if (state == State::Uninitialized)
		{
			state = State::Initializing;
			OnInitialize();
			state = State::Active;
		}
	}

	void Destroy()
	{
		if (state == State::Active)
		{
			state = State::Destroying;
			OnDestroy();
			state = State::Destroyed;
		}
	}

	bool isActive() const { return state == State::Active; }

	void SetOwner(Entity* entity) { owner = entity; }
	Entity* GetOwner() const { return owner; }

protected:
	virtual void OnInitialize() {}
	virtual void OnDestroy() {}
	virtual void Update(float deltatime) {}
	virtual void Render() {}

	friend class Entity; //This grants Entity access to the protected members of Component, 
	//allowing it to call Update and Render during the game loop, and manage the component's lifecycle through Initialize and Destroy.
};
