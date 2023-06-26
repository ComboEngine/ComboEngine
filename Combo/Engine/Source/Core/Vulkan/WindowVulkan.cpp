#include "pch.h"
#ifdef COMBO_VULKAN
#include <Core/Window.h>
#include "WindowVulkan.h"
#include "PlatformVulkan.h"
#include <Core/Core.h>
#include "ContextVulkan.h"
#include "../Input.h"
#include <Editor/Editor.h>


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input::Keys[key] = action == GLFW_PRESS || action == GLFW_REPEAT;
}

void WindowVulkan::Init()
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

void WindowVulkan::Update() {
	double x, y;
	glfwGetCursorPos(glfwWindow, &x, &y);
	Input::CurrentMousePosition = glm::vec2(x, y);

	if (glfwWindowShouldClose(glfwWindow)) {
		Core::RequestExit();
	}

	glfwPollEvents();
}

bool WindowVulkan::IsWindowFocused()
{
	return glfwGetWindowAttrib(this->glfwWindow, 0x00020001);
}

void WindowVulkan::SetTitle(std::string title)
{
	glfwSetWindowTitle(glfwWindow, title.c_str());
}

int WindowVulkan::GetWidth()
{
	int width;
	glfwGetWindowSize(glfwWindow, &width, nullptr);
	return width;
}

int WindowVulkan::GetHeight()
{
	int height;
	glfwGetWindowSize(glfwWindow, nullptr,&height);
	return height;
}

const void* WindowVulkan::GetPlainWindow()
{
	return glfwGetWin32Window(glfwWindow);
}

void WindowVulkan::LockCursor(bool lock)
{
	if (lock) {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

std::string WindowVulkan::GetApiName()
{
	return "Vulkan";
}


#endif