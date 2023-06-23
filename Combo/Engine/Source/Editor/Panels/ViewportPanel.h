#pragma once
#include "../Panel.h"

enum EditorViewMode {
	Position,
	Normal,
	Diffuse,
	FinalBuffer
};

enum GizmoType {
	POSITION,
	ROTATION,
	SCALE,
	ALL
};

class ViewportPanel : public Panel {
public:
	GizmoType GizmoType;
	EditorViewMode ViewMode = EditorViewMode::FinalBuffer;
	bool MouseHooked;
	virtual std::string GetName();
	virtual void Draw();
};