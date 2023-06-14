#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/Window.h>

class WindowDX11 : public Window{
public:
	HWND hWnd;
	MSG Message;
	virtual void Init();
	virtual void Update();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual const void* GetPlainWindow();
	virtual std::string GetApiName();
};
#endif