#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/Window.h>

class WindowDX11 : public Window {
public:
	GLFWwindow* glfwWindow;
	virtual void Init();
	virtual void Update();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual const void* GetPlainWindow();
	virtual void LockCursor(bool lock);
	virtual std::string GetApiName();
};
#endif