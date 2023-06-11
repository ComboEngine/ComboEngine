#pragma once
#include "pch.h"

class Input {
public:
	static BYTE KeyboardState[256];
	static DIMOUSESTATE MouseState;
	static IDirectInputDevice8* Keyboard;
	static IDirectInputDevice8* Mouse;
	static DIMOUSESTATE MouseLastState;
	static LPDIRECTINPUT8 DirectInput;
	static void Init();
	static void Update();
	static void End();
	static bool IsKeyDown(int key);
	static glm::vec2 GetMouseDelta();
};