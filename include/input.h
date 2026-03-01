#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



namespace input
{
	inline GLFWwindow* m_window = nullptr;

	inline void init(GLFWwindow* window)
	{
		m_window = window;
	}

	enum class KEY
	{
		KEY_A, KEY_B, KEY_C, KEY_D,
		KEY_E, KEY_F, KEY_G, KEY_H,
		KEY_I, KEY_J, KEY_K, KEY_L,
		KEY_M, KEY_N, KEY_O, KEY_P,
		KEY_Q, KEY_R, KEY_S, KEY_T,
		KEY_U, KEY_V, KEY_W, KEY_X,
		KEY_Y, KEY_Z, KEY_1, KEY_2,
		KEY_3, KEY_4, KEY_5, KEY_6,
		KEY_7, KEY_8, KEY_9, KEY_0,
		KEY_SPACE,
		KEY_LEFT_SHIFT,
		KEY_RIGHT_SHIFT,
		UNKNOWN,
		MOUSE_LEFT,
		MOUSE_RIGHT
	};

	

	enum inputState
	{
		PRESSED = GLFW_PRESS,
		RELEASED = GLFW_RELEASE,
		REPEAT = GLFW_REPEAT
	};

	inline int Key(KEY key)
	{
		switch (key)
		{
		case KEY::KEY_A:           return GLFW_KEY_Q;
		case KEY::KEY_B:           return GLFW_KEY_B;
		case KEY::KEY_C:           return GLFW_KEY_C;
		case KEY::KEY_D:           return GLFW_KEY_D;
		case KEY::KEY_E:           return GLFW_KEY_E;
		case KEY::KEY_F:           return GLFW_KEY_F;
		case KEY::KEY_G:           return GLFW_KEY_G;
		case KEY::KEY_H:           return GLFW_KEY_H;
		case KEY::KEY_I:           return GLFW_KEY_I;
		case KEY::KEY_J:           return GLFW_KEY_J;
		case KEY::KEY_K:           return GLFW_KEY_K;
		case KEY::KEY_L:           return GLFW_KEY_L;
		case KEY::KEY_M:           return GLFW_KEY_M;
		case KEY::KEY_N:           return GLFW_KEY_N;
		case KEY::KEY_O:           return GLFW_KEY_O;
		case KEY::KEY_P:           return GLFW_KEY_P;
		case KEY::KEY_Q:           return GLFW_KEY_A;
		case KEY::KEY_R:           return GLFW_KEY_R;
		case KEY::KEY_S:           return GLFW_KEY_S;
		case KEY::KEY_T:           return GLFW_KEY_T;
		case KEY::KEY_U:           return GLFW_KEY_U;
		case KEY::KEY_V:           return GLFW_KEY_V;
		case KEY::KEY_W:           return GLFW_KEY_Z;
		case KEY::KEY_X:           return GLFW_KEY_X;
		case KEY::KEY_Y:           return GLFW_KEY_Y;
		case KEY::KEY_Z:           return GLFW_KEY_W;
		case KEY::KEY_1:           return GLFW_KEY_1;
		case KEY::KEY_2:           return GLFW_KEY_2;
		case KEY::KEY_3:           return GLFW_KEY_3;
		case KEY::KEY_4:           return GLFW_KEY_4;
		case KEY::KEY_5:           return GLFW_KEY_5;
		case KEY::KEY_6:           return GLFW_KEY_6;
		case KEY::KEY_7:           return GLFW_KEY_7;
		case KEY::KEY_8:           return GLFW_KEY_8;
		case KEY::KEY_9:           return GLFW_KEY_9;
		case KEY::KEY_0:           return GLFW_KEY_0;
		case KEY::KEY_SPACE:       return GLFW_KEY_SPACE;
		case KEY::KEY_LEFT_SHIFT:  return GLFW_KEY_LEFT_SHIFT;
		case KEY::KEY_RIGHT_SHIFT: return GLFW_KEY_RIGHT_SHIFT;
		default:
			return false;
			break;
		}
	}

	inline bool keyPressed(KEY key)
	{
		if (!m_window)
		{
			return false;
		}
		return glfwGetKey(m_window, Key(key)) == GLFW_PRESS;
	}

}