#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



namespace input
{
	enum class Key
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

	};

	enum inputState
	{
		PRESSED = GLFW_PRESS,
		RELEASED = GLFW_RELEASE,
		REPEAT = GLFW_REPEAT
	};

	inline int keyPressed(Key key)
	{
		switch (key)
		{
		case Key::KEY_A:           return GLFW_KEY_Q;
		case Key::KEY_B:           return GLFW_KEY_B;
		case Key::KEY_C:           return GLFW_KEY_C;
		case Key::KEY_D:           return GLFW_KEY_D;
		case Key::KEY_E:           return GLFW_KEY_E;
		case Key::KEY_F:           return GLFW_KEY_F;
		case Key::KEY_G:           return GLFW_KEY_G;
		case Key::KEY_H:           return GLFW_KEY_H;
		case Key::KEY_I:           return GLFW_KEY_I;
		case Key::KEY_J:           return GLFW_KEY_J;
		case Key::KEY_K:           return GLFW_KEY_K;
		case Key::KEY_L:           return GLFW_KEY_L;
		case Key::KEY_M:           return GLFW_KEY_M;
		case Key::KEY_N:           return GLFW_KEY_N;
		case Key::KEY_O:           return GLFW_KEY_O;
		case Key::KEY_P:           return GLFW_KEY_P;
		case Key::KEY_Q:           return GLFW_KEY_A;
		case Key::KEY_R:           return GLFW_KEY_R;
		case Key::KEY_S:           return GLFW_KEY_S;
		case Key::KEY_T:           return GLFW_KEY_T;
		case Key::KEY_U:           return GLFW_KEY_U;
		case Key::KEY_V:           return GLFW_KEY_V;
		case Key::KEY_W:           return GLFW_KEY_Z;
		case Key::KEY_X:           return GLFW_KEY_X;
		case Key::KEY_Y:           return GLFW_KEY_Y;
		case Key::KEY_Z:           return GLFW_KEY_W;
		case Key::KEY_1:           return GLFW_KEY_1;
		case Key::KEY_2:           return GLFW_KEY_2;
		case Key::KEY_3:           return GLFW_KEY_3;
		case Key::KEY_4:           return GLFW_KEY_4;
		case Key::KEY_5:           return GLFW_KEY_5;
		case Key::KEY_6:           return GLFW_KEY_6;
		case Key::KEY_7:           return GLFW_KEY_7;
		case Key::KEY_8:           return GLFW_KEY_8;
		case Key::KEY_9:           return GLFW_KEY_9;
		case Key::KEY_0:           return GLFW_KEY_0;
		case Key::KEY_SPACE:       return GLFW_KEY_SPACE;
		case Key::KEY_LEFT_SHIFT:  return GLFW_KEY_LEFT_SHIFT;
		case Key::KEY_RIGHT_SHIFT: return GLFW_KEY_RIGHT_SHIFT;
		default:
			return false;
			break;
		}
	}

}