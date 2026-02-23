#pragma once
#include "BaseComponent.h"
#include "Component.h"
#include "EventBus.h"
#include "EventListener.h"

class CameraController: public Component, public EventListener
{
private: 
	CameraComponent* camera;
	float moveSpeed = 5.0f;
	float rotateSpeed = 0.1f;

	bool moveForward = false;
	bool moveBackward = false;
	bool moveLeft = false;
	bool moveRight = false;

public:
	void Initialize() override
	{
		camera = GetOwner()->GetComponent<CameraComponent>();

		//Register as Event Listener

		GetEventBus().AddListener(this);
	}

	void Update(float deltatime) override
	{
		if (!camera) return;

		glm::vec3 movement(0.0f);

		if (moveForward) movement.z -= 1.0f;
		if (moveBackward) movement.z += 1.0f;
		if (moveLeft) movement.x -= 1.0f;
		if (moveRight) movement.x += 1.0f;

		if (glm::length(movement) > 0.0f)
		{
			movement = glm::normalize(movement) * moveSpeed * deltatime;

			auto transform = GetOwner()->GetComponent<TransformComponent>();
			if (transform)
			{
				glm::vec3 position = transform->GetPosition();
				position += movement;
				transform->SetPosition(position);
			}
		}
	}

	void OnEvent(const Event& event) override
	{
		EventDispatcher dispatcher(event);

		// Handler Key pressed Event

		dispatcher.Dispatch<KeyPressedEvent>([this](const  KeyPressedEvent& e)
			{
				switch (e.GetKeyCode())
				{
				case KEY_W: moveForward = true; break;
				case KEY_S: moveBackward = true; break;
				case KEY_A: moveLeft = true; break;
				case KEY_D: moveRight = true; break;
				}
				return false;
			});

		//handle Key Released Event
		dispatcher.Dispatch<KeyReleasedEvent>([this](const KeyReleasedEvent& e)
			{
				switch (e.GetKeyCode())
				{
				case KEY_W: moveForward = false; break;
				case KEY_S: moveBackward = false; break;
				case KEY_A: moveLeft = false; break;
				case KEY_D: moveRight = false; break;
				}
				return false;
			});

		//handle windows Resize Event
		dispatcher.Dispatch<WindowResizeEvent>([this](const WindowResizeEvent& e)
		{
			if (camera)
			{
				float aspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
				camera->SetAspectRatio(aspectRatio);
			}
			return false;
		});
	}

	~CameraController() override
	{
		// UnRegister as event Listener
		GetEventBus().RemoveListener(this);
	}

private:
	EventBus& GetEventBus()
	{
		static EventBus eventBus;
		return  eventBus;
	}
};

class InputSystem
{
private:
	EventBus& eventbus;

	//Key states
	std::unordered_map<int, bool> keyStates;

public:
	explicit InputSystem(EventBus& bus) : eventbus(bus) {}

	void Update()
	{
		// Poll input events from the platform
		// ...

		// Example: Process a key press
		ProcessKeyPress(KEY_W, false);
	}

	void ProcessKeyPress(int keyCode, bool repeat) {
		bool& keyState = keyStates[keyCode];

		if (!keyState || repeat) {
			// Key was not pressed before or this is a repeat
			KeyPressedEvent event(keyCode, repeat);
			eventbus.PublishEvent(event);
		}

		keyState = true;
	}

	void ProcessKeyRelease(int keyCode) {
		bool& keyState = keyStates[keyCode];

		if (keyState) {
			// Key was pressed before
			KeyReleasedEvent event(keyCode);
			eventbus.PublishEvent(event);
		}

		keyState = false;
	}
};