#pragma once
#include "../Panel.h"

class ContentPanel : public Panel {
public:
	int CurrentAssetTypeCreateAssetPopup = 0;
	std::string PathBuffer = "l";
	virtual std::string GetName();
	virtual void Draw();
};