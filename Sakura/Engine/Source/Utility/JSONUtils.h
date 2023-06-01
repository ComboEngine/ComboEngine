#pragma once

#include <Math/Color.h>
#include <Utility/JSON.h>

class JSONUtils {
public:
	static Color32 ReadColorFromJson(nlohmann::json j);
	static nlohmann::json WriteJsonFromColor(Color32 color);
};