#pragma once

#include "pch.h"

class Panel {
public:
	bool Show = true;
	virtual std::string GetName() = 0;
	virtual void Draw() = 0;
};