#pragma once
#include "Entity.h"

enum class EventCategory
	: uint16_t
{
	None = 0,
	Application = 1 << 0,
	Input = 1 << 1,
	Keyboard = 1 << 2,
	Mouse = 1 << 3,
	MouseButton = 1 << 4,
	Window = 1 << 5,
	UI = 1 << 6,
	All = 0xffff
};

inline EventCategory operator|(EventCategory a, EventCategory b)
{
	return static_cast<EventCategory>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline EventCategory operator&(EventCategory a, EventCategory b)
{
	return static_cast<EventCategory>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

class Event
{
public:
	virtual ~Event() = default;

	//Get type of Event
	virtual const char* GetType() const = 0;

	//For queued event clone the event
	virtual Event* Clone() const = 0;

	//Get the category of event
	virtual EventCategory GetCategoryFlag() const = 0;

	//Check if event is in category
	bool IsInCategory(EventCategory category) const
	{
		return GetCategoryFlag() & static_cast<int>(category);
	}
};

// Macro to help define event types with amelioration for event
#define DEFINE_EVENT_TYPE(type, categoryFlag) \
    static const char* GetStaticType() { return #type; } \
    virtual const char* GetType() const override { return GetStaticType(); } \
    virtual Event* Clone() const override { return new type(*this); } \
	virtual EventCategory GetCategoryFlag() const override {return categoryFlag; }


class WindowResizeEvent : public Event
{
private: 
	int width;
	int height;

public:
	WindowResizeEvent(int w, int h) : width(w), height(h) {}

	int GetWidth() const { return width; }
	int GetHeight() const { return  height; }

	DEFINE_EVENT_TYPE(WindowResizeEvent, EventCategory::Window);
};

class KeyPressedEvent : public Event
{
private:
	int keyCode;
	bool repeat;

public:
	KeyPressedEvent(int key, bool isRepeat) : keyCode(key), repeat(isRepeat) {}
	
	int GetKeyCode() const { return  keyCode; }
	bool isRepeat() const { return  repeat; }
	

	DEFINE_EVENT_TYPE(KeyPressedEvent, EventCategory::Input | EventCategory::Keyboard);
};

class KeyReleasedEvent : public Event
{
	int keycode;
	bool repeat;
public:
	KeyReleasedEvent(int key, bool isRepeat): keycode(key), repeat(isRepeat) {} 

	int GetKeyCode() const { return keycode; }
	bool isRepeat() const { return repeat; }

	DEFINE_EVENT_TYPE(KeyReleasedEvent, EventCategory::Input | EventCategory::Keyboard);
};


class CollisionEvent : public Event
{
private:
	Entity* entityA;
	Entity* entityB;

public:
	CollisionEvent(Entity* a, Entity* b) : entityA(a), entityB(b) {}

	Entity* GetEntityA() const { return entityA; }
	Entity* GetEntityB() const { return entityB; }

};