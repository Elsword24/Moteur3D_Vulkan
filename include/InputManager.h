#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <GLFW/glfw3.h>
#include "input.h"

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
			{"Vertical",   {input::KEY::KEY_W, input::KEY::KEY_S}},
			{"MouseX",     {input::KEY::UNKNOWN, input::KEY::UNKNOWN}},
			{"MouseY",     {input::KEY::UNKNOWN, input::KEY::UNKNOWN}}
		},
		{
			{"Jump",  input::KEY::KEY_SPACE},
			{"Sprint",input::KEY::KEY_LEFT_SHIFT},
			{"Shoot", input::KEY::MOUSE_LEFT},
			{"Aim",   input::KEY::MOUSE_RIGHT}
		}
	};

public:
	InputMapper()
	{
		currentState = mapper;
		previousState = mapper;
	}

	// Constructeur pour initialiser/populer le mapper si nécessaire
	void Update()
	{
		previousState = currentState;

		double mouseX = 0.0, mouseY = 0.0;
		GLFWwindow* ctx = glfwGetCurrentContext();
		if (ctx)
		{
			glfwGetCursorPos(ctx, &mouseX, &mouseY);

			int windowWidth = 1, windowHeight = 1;
			glfwGetWindowSize(ctx, &windowWidth, &windowHeight);
			float normalizedMouseX = 2.0f * static_cast<float>(mouseX) / static_cast<float>(windowWidth) - 1.0f;
			float normalizedMouseY = 2.0f * static_cast<float>(mouseY) / static_cast<float>(windowHeight) - 1.0f;

			glfwPollEvents();

			for (auto& axis : currentState.axis)
			{
				if (axis.name == "MouseX")
				{
					axis.value = normalizedMouseX;
				}
				else if (axis.name == "MouseY")
				{
					axis.value = normalizedMouseY;
				}
			}
		}
		else
		{
			glfwPollEvents();
		}

		// reset boutons (flags) dans l'état courant
		for (auto& btn : currentState.btn)
		{
			btn.pressed = false;
			btn.released = false;
		}

		// updates axis (clavier)
		for (auto& axis : currentState.axis)
		{
			if (axis.name == "MouseX" || axis.name == "MouseY")
				continue;

			float newValue = 0.0f;

			if (input::keyPressed(axis.btn[0]) == input::PRESSED) {
				newValue -= 1.0f;
			}
			if (input::keyPressed(axis.btn[1]) == input::PRESSED)
			{
				newValue += 1.0f;
			}
			const float smoothFactor /* smooth to make better transition*/ = 0.2f;
			axis.value = (axis.value * (1.0f - smoothFactor)) + (newValue * smoothFactor);
		}

		// Update buttons (détection des transitions)
		for (auto& btn : currentState.btn)
		{
			auto state = input::keyPressed(btn.btn);
			if (state == input::PRESSED && !btn.pressed)
			{
				btn.pressed = true;
			}
			else if (state == input::RELEASED && btn.pressed)
			{
				btn.released = true;
				btn.pressed = false;
			}
		}
	}

	// Getter

	float GetAxis(const std::string& name) const
	{
		auto it = std::find_if(currentState.axis.begin(), currentState.axis.end(), [&name](const AxisDescription& a) { return a.name == name; });
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

};