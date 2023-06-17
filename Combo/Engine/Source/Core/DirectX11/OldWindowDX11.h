#ifdef 0
#pragma once
#include <Core/Window.h>

class WindowDX11 : public Window{
public:
	HWND hWnd;
	bool MouseLocked;
	MSG Message;
	virtual void Init();
	virtual void Update();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual const void* GetPlainWindow();
	virtual void LockCursor(bool lock);
	virtual std::string GetApiName();
};
#endif