#include "pch.h"
#include "Engine.h"


Logger Engine::logger = Logger();
Version Engine::version = Version(1, 0, 0);
bool Engine::shouldExit = false;
std::vector<std::shared_ptr<Material>> Engine::Materials;
std::shared_ptr<GPUFramebuffer> Engine::Color = nullptr;
std::shared_ptr<GPUFramebuffer> Engine::Depth = nullptr;