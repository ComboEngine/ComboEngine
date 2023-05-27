#pragma once

#include <iostream>
#include <vector>
#include <string>

//Include windows headers
#ifdef _WIN32
#include <Platform/Windows/PlatformHeaders.h>
#endif

//Using std
using namespace std;

//Sakura types
#define sakura_array std::vector
#define sakura_string std::string
#define sakura_ptr std::shared_ptr