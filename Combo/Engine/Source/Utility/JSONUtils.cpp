#include "pch.h"
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

nlohmann::json JSONUtils::WriteJsonFromColor(Color32 color)
{
    nlohmann::json j;
    j["r"] = color.R;
    j["g"] = color.G;
    j["b"] = color.B;
    j["a"] = color.A;
    return j;
}
