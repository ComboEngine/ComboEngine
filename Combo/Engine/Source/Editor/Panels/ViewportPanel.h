#pragma once
#include "../Panel.h"

class ViewportPanel {
public:
	virtual std::string GetName();
	virtual void Draw();
};