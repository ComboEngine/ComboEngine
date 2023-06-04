#include "pch.h"
#include <Engine/Engine.h>


//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
int main(int argc, char* argv[]) {
	bool windows = true;
#ifndef _WIN32
	windows = false;
#endif
	if (!windows) {
		std::cout << "Non-Windows platforms are not currently supported" << std::endl;
		return -1;
	}
	std::vector<std::string> arguments;
	for (int i = 0; i < argc; i++) {
		arguments.push_back(argv[i]);
	}
	Platform::PreInit(GetModuleHandle(NULL));
	return Engine::Main(arguments);
}