#include "pch.h"
#ifdef COMBO_DIRECTX11
#include <Core/Window.h>
#include "WindowDX11.h"
#include "PlatformDX11.h"
#include <Core/Core.h>
#include "ContextDX11.h"
#include "../Input.h"
#include <Editor/Editor.h>


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input::Keys[key] = action == GLFW_PRESS || action == GLFW_REPEAT;
}

void WindowDX11::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindow = glfwCreateWindow(Specification.Width, Specification.Height, Specification.Title.c_str(), NULL, NULL);
	glfwSetKeyCallback(glfwWindow, KeyCallback);
	glfwSetDropCallback(glfwWindow, [](GLFWwindow* window, int count, const char** paths) {
#ifdef COMBO_EDITOR
		std::vector<std::string> pathArray;
		for (int i = 0; i < count; i++) {
			pathArray.push_back(paths[i]);
		}
		Editor::OnDrop(pathArray);
#endif
	});

	Core::ExitEvent.Hook([&] {
		glfwDestroyWindow(glfwWindow);
		glfwTerminate();
	});
}

void WindowDX11::Update() {
	double x, y;
	glfwGetCursorPos(glfwWindow, &x, &y);
	Input::CurrentMousePosition = glm::vec2(x, y);

	if (glfwWindowShouldClose(glfwWindow)) {
		Core::RequestExit();
	}

	glfwPollEvents();
}

void WindowDX11::SetTitle(std::string title)
{
	glfwSetWindowTitle(glfwWindow, title.c_str());
}

int WindowDX11::GetWidth()
{
	int width;
	glfwGetWindowSize(glfwWindow, &width, nullptr);
	return width;
}

int WindowDX11::GetHeight()
{
	int height;
	glfwGetWindowSize(glfwWindow, nullptr,&height);
	return height;
}

const void* WindowDX11::GetPlainWindow()
{
	return glfwGetWin32Window(glfwWindow);
}

void WindowDX11::LockCursor(bool lock)
{
	if (lock) {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

std::string WindowDX11::GetApiName()
{
	return "DX11";
}


#endif