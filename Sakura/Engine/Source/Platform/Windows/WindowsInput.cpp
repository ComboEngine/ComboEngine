#include "pch.h"
#include "WindowsInput.h"
#include "WindowsPlatform.h"
IDirectInputDevice8* Input::Keyboard;
IDirectInputDevice8* Input::Mouse;
DIMOUSESTATE Input::MouseLastState;
LPDIRECTINPUT8 Input::DirectInput;
BYTE Input::KeyboardState[256];
DIMOUSESTATE Input::MouseState;

void Input::Init()
{
    DirectInput8Create(Platform::hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);
    DirectInput->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL);
    DirectInput->CreateDevice(GUID_SysMouse, &Mouse, NULL);
    Keyboard->SetDataFormat(&c_dfDIKeyboard);
    Keyboard->SetCooperativeLevel(Platform::window->Hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    Mouse->SetDataFormat(&c_dfDIMouse);
   // Mouse->SetCooperativeLevel(Platform::window->Hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
}

void Input::Update()
{
    Keyboard->Acquire();
    Mouse->Acquire();
    Mouse->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState);
    Keyboard->GetDeviceState(sizeof(KeyboardState), (LPVOID)&KeyboardState);
}

void Input::End()
{
    Input::MouseLastState = Input::MouseState;
}

bool Input::IsKeyDown(int key)
{
    return (KeyboardState[key] & 0x80) != 0;
}

Vector2 Input::GetMouseDelta()
{
    return Vector2(Input::MouseState.lX - Input::MouseLastState.lX, Input::MouseState.lY - Input::MouseLastState.lY);
}
