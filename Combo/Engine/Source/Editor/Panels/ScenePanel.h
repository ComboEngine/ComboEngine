#pragma once
#include "../Panel.h"

class ScenePanel : public Panel {
public:
	int CurrentActorTypeCreateActorPopup = 0;
	virtual std::string GetName();
	virtual void Draw();
};