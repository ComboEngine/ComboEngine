#include "JSONUtils.h"

Color32 JSONUtils::ReadColorFromJson(nlohmann::json j)
{
    Color32 color = Color32(0, 0, 0, 0);
    color.R = j["r"];
    color.G = j["g"];
    color.B = j["b"];
    color.A = j["a"];
    return color;
}
