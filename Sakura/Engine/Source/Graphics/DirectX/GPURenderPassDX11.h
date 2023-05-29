#pragma once

#include <Math/Color.h>

class GPURenderPass {
public:
	Color32 Color = Color32(0,0,0,1);
	void Start();
	void End();
};