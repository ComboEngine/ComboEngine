#include <Engine/Engine.h>


//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
int main(int argc, char* argv[]) {
	bool windows = true;
#ifndef _WIN32
	windows = false;
#endif
	if (!windows) {
		cout << "Non-Windows platforms are not currently supported" << endl;
		return -1;
	}
	sakura_array<string> arguments;
	for (int i = 0; i < argc; i++) {
		arguments.push_back(argv[i]);
	}
	return Engine::Main(arguments);
}