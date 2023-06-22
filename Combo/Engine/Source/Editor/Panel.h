#pragma once

#include "pch.h"

class Panel {
public:
	virtual std::string GetName() = 0;
	virtual void Draw() = 0;
};