#pragma once

#include "../Project.h"
class ProjectSerializer {
public:
	static void LoadProject(std::string path);
	static void CreateProject(std::string path);
};