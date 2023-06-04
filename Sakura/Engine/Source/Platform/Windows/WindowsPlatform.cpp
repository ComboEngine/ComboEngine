#include "pch.h"
#include "WindowsPlatform.h"
#include <Engine/Engine.h>


HINSTANCE Platform::hInstance = nullptr;
std::shared_ptr<Window> Platform::window = nullptr;
MSG Platform::msg;
void Platform::PreInit(HINSTANCE hInstance)
{
	Platform::hInstance = hInstance;
}

void Platform::Init()
{
	Platform::window = Window::Create("Sakura",1920,1080);
    ZeroMemory(&Platform::msg, sizeof(MSG));
}

void Platform::OnUpdate()
{
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            Engine::RequestExit();

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
