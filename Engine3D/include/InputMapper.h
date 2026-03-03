#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <GLFW/glfw3.h>
#include "input.h"
#include <array>
#include <cstdint>

class InputMapper
{
protected:
	struct AxisDescription
	{
		std::string name;
		std::array<input::KEY, 2> btn;
		float value = 0.0f;
	};

	struct BtsDescription
	{
		std::string name;
		input::KEY btn;
		bool pressed = false;
		bool released = false;
	};

	struct InputMapDesc
	{
		std::vector<AxisDescription> axis;
		std::vector<BtsDescription> btn;
	};

	InputMapDesc currentState;
	InputMapDesc previousState;

	// Init (mapping par défaut)
	InputMapDesc mapper{
		{
			{"Horizontal", {input::KEY::KEY_Q, input::KEY::KEY_D}},
			{"Vertical",   {input::KEY::KEY_SPACE, input::KEY::KEY_CTRL}},
			{"Forward",    {input::KEY::KEY_S, input::KEY::KEY_Z}},
			{"MouseX",     {input::KEY::UNKNOWN, input::KEY::UNKNOWN}},
			{"MouseY",     {input::KEY::UNKNOWN, input::KEY::UNKNOWN}}
		},
		{
			{"Sprint",input::KEY::KEY_LEFT_SHIFT},
			{"Shoot", input::KEY::MOUSE_LEFT},
			{"Aim",   input::KEY::MOUSE_RIGHT}
		}
	};

	std::array<uint8_t, GLFW_KEY_LAST + 1> m_keysDown{};     // 0/1
	std::array<uint8_t, GLFW_KEY_LAST + 1> m_keysPrev{};     // 0/1
	std::array<uint8_t, GLFW_MOUSE_BUTTON_LAST + 1> m_mouseDown{};
	std::array<uint8_t, GLFW_MOUSE_BUTTON_LAST + 1> m_mousePrev{};

	double m_mouseX = 0.0, m_mouseY = 0.0;
	double m_prevMouseX = 0.0, m_prevMouseY = 0.0;
	float  m_mouseDeltaX = 0.0f, m_mouseDeltaY = 0.0f;

	GLFWwindow* m_window = nullptr;

private:

	bool IsHeld(input::KEY k) const
	{
		int code = input::Key(k);
		if (code == GLFW_KEY_UNKNOWN) return false;

		if (input::isMouseKey(k)) {
			if (code < 0 || code > GLFW_MOUSE_BUTTON_LAST) return false;
			return m_mouseDown[(size_t)code] != 0;
		}
		else {
			if (code < 0 || code > GLFW_KEY_LAST) return false;
			return m_keysDown[(size_t)code] != 0;
		}
	}

	bool WasHeld(input::KEY k) const
	{
		int code = input::Key(k);
		if (code == GLFW_KEY_UNKNOWN) return false;

		if (input::isMouseKey(k)) {
			if (code < 0 || code > GLFW_MOUSE_BUTTON_LAST) return false;
			return m_mousePrev[(size_t)code] != 0;
		}
		else {
			if (code < 0 || code > GLFW_KEY_LAST) return false;
			return m_keysPrev[(size_t)code] != 0;
		}
	}

	

public:

	static InputMapper& GetInstance()
	{
		static InputMapper instance;
		return instance;
	}

	InputMapper()
	{
		currentState = mapper;
		previousState = mapper;
	}

	void Init(GLFWwindow* window)
	{
		m_window = window;
		input::init(window);

		m_keysDown.fill(0);
		m_keysPrev.fill(0);
		m_mouseDown.fill(0);
		m_mousePrev.fill(0);

		m_mouseX = m_mouseY = 0.0;
		m_prevMouseX = m_prevMouseY = 0.0;
		m_mouseDeltaX = m_mouseDeltaY = 0.0f;

		glfwSetKeyCallback(window, [](GLFWwindow*, int key, int scancode, int action, int mods) {
			auto& im = InputMapper::GetInstance();
			if (key < 0 || key > GLFW_KEY_LAST) return;

			if (action == GLFW_PRESS || action == GLFW_REPEAT) im.m_keysDown[(size_t)key] = 1;
			else if (action == GLFW_RELEASE)                   im.m_keysDown[(size_t)key] = 0;
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int) {
			auto& im = InputMapper::GetInstance();
			if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return;

			if (action == GLFW_PRESS)   im.m_mouseDown[(size_t)button] = 1;
			else if (action == GLFW_RELEASE) im.m_mouseDown[(size_t)button] = 0;
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow*, double x, double y) {
			auto& im = InputMapper::GetInstance();
			im.m_mouseX = x;
			im.m_mouseY = y;
			});
	}

	void Update()
	{
		// mouse delta
		m_mouseDeltaX = (float)(m_mouseX - m_prevMouseX);
		m_mouseDeltaY = (float)(m_mouseY - m_prevMouseY);
		m_prevMouseX = m_mouseX;
		m_prevMouseY = m_mouseY;

		// update axes
		for (auto& axis : currentState.axis)
		{
			if (axis.name == "MouseX") { axis.value = m_mouseDeltaX; continue; }
			if (axis.name == "MouseY") { axis.value = m_mouseDeltaY; continue; }

			float newValue = 0.0f;
			if (IsHeld(axis.btn[0])) newValue -= 1.0f;
			if (IsHeld(axis.btn[1])) newValue += 1.0f;

			const float smoothFactor = 0.2f;
			axis.value = (axis.value * (1.0f - smoothFactor)) + (newValue * smoothFactor);
		}

		// update buttons transitions
		for (auto& btn : currentState.btn)
		{
			bool now = IsHeld(btn.btn);
			bool prev = WasHeld(btn.btn);

			btn.pressed = now && !prev;
			btn.released = !now && prev;
		}

		// update previous raw states
		m_keysPrev = m_keysDown;
		m_mousePrev = m_mouseDown;
	}

	// Getter

	float GetAxis(const std::string& name) const
	{
		auto it = std::find_if(currentState.axis.begin(), currentState.axis.end(),
			[&name](const AxisDescription& a) { return a.name == name; });

		return (it != currentState.axis.end()) ? it->value : 0.0f;
	}

	bool isButtonPressed(const std::string& name) const
	{
		auto it = std::find_if(currentState.btn.begin(), currentState.btn.end(), [&name](const BtsDescription& b)
		{
			return b.name == name;
		});
		return (it != currentState.btn.end()) ? it->pressed : false;
	}

	bool isButtonReleased(const std::string& name) const
	{
		auto it = std::find_if(currentState.btn.begin(), currentState.btn.end(), [&name](const BtsDescription& b)
		{
			return b.name == name;
		});
		return (it != currentState.btn.end()) ? it->released : false;
	}

	// Allow dynamic remapping
	void RemapAxis(const std::string& axisName, input::KEY negativeKey, input::KEY positiveKey) {
		for (auto& axis : currentState.axis) {
			if (axis.name == axisName) {
				axis.btn[0] = negativeKey;
				axis.btn[1] = positiveKey;
				break;
			}
		}
	}

	void RemapButton(const std::string& buttonName, input::KEY newKey) {
		for (auto& button : currentState.btn) {
			if (button.name == buttonName) {
				button.btn = newKey;
				break;
			}
		}
	}

	bool IsButtonDown(const std::string& name) const
	{
		return isButtonPressed(name);
	}

	bool IsButtonUp(const std::string& name) const
	{
		return isButtonReleased(name);
	}

	bool IsButtonHeld(const std::string& name) const
	{
		auto it = std::find_if(currentState.btn.begin(), currentState.btn.end(),
			[&name](const BtsDescription& b) { return b.name == name; });

		if (it == currentState.btn.end()) return false;

		return IsHeld(it->btn);
	}

};