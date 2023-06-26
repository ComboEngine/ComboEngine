#ifdef COMBO_VULKAN
#pragma once
#include <Core/Window.h>

class WindowVulkan : public Window {
public:
	GLFWwindow* glfwWindow;
	virtual void Init();
	virtual void Update();
	virtual bool IsWindowFocused();
	virtual void SetTitle(std::string title);
	virtual int GetWidth();
	virtual int GetHeight();
	virtual const void* GetPlainWindow();
	virtual void LockCursor(bool lock);
	virtual std::string GetApiName();
};
#endif