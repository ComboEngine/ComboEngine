#pragma once

#include <Math/Color.h>
#include "../GPURenderData.h"

class GPURenderPass {
public:
	Color32 Color = Color32(0,0,0,1);
	sakura_array<GPURenderData> RenderDataList;
	void Start();
	void End();
};