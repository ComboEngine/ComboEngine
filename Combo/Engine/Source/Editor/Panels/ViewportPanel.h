#pragma once
#include "../Panel.h"

enum EditorViewMode {
	Position,
	Normal,
	Diffuse,
	FinalBuffer
};

class ViewportPanel : public Panel {
public:
	int CurrentActorTypeCreateActorPopup = 0;
	EditorViewMode ViewMode = EditorViewMode::FinalBuffer;
	bool MouseHooked;
	virtual std::string GetName();
	virtual void Draw();
};