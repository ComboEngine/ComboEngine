#include "Engine.h"

Logger Engine::logger = Logger();
Version Engine::version = Version(1, 0, 0);
bool Engine::shouldExit = false;
sakura_array<sakura_ptr<Material>> Engine::Materials;
sakura_ptr<GPUFramebuffer> Engine::Color = nullptr;
sakura_ptr<GPUFramebuffer> Engine::Depth = nullptr;