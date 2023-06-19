#include "pch.h"
#include "Render2D.h"
#include "Core.h"

std::vector<DrawTarget> Render2D::DrawTargets;

void Render2D::RenderImGui()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(Core::s_Window.Get()->GetWidth(), Core::s_Window.Get()->GetHeight()));
	ImGui::Begin("##Render2DSurface",nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);
	for (DrawTarget target : DrawTargets) {
		if (target.Type == RECTANGLE) {
			int x = std::any_cast<int>(target.Data["x"]);
			int y = std::any_cast<int>(target.Data["y"]);
			int width = std::any_cast<int>(target.Data["width"]);
			int height = std::any_cast<int>(target.Data["height"]);
			glm::vec4 color = std::any_cast<glm::vec4>(target.Data["color"]);

			ImGui::GetWindowDrawList()->AddRectFilled( ImVec2(x, y), ImVec2(x + width, y + height), IM_COL32(color.x,color.y,color.z,color.w));
		}
		if (target.Type == TEXTUREDRECTANGLE) {
			int x = std::any_cast<int>(target.Data["x"]);
			int y = std::any_cast<int>(target.Data["y"]);
			int width = std::any_cast<int>(target.Data["width"]);
			int height = std::any_cast<int>(target.Data["height"]);
			void* texture = std::any_cast<void*>(target.Data["texture"]);

			ImGui::GetWindowDrawList()->AddImage(texture, ImVec2(x, y), ImVec2(x + width, y + height));
		}
		if (target.Type == TEXT) {
			int x = std::any_cast<int>(target.Data["x"]);
			int y = std::any_cast<int>(target.Data["y"]);
			std::string text = std::any_cast<std::string>(target.Data["text"]);
			glm::vec4 color = std::any_cast<glm::vec4>(target.Data["color"]);

			ImGui::SetCursorPos(ImVec2(x, y));
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(color.x, color.y, color.z, color.w));
			ImGui::Text(text.c_str());
			ImGui::PopStyleColor();
		}
	}
	ImGui::End();
	DrawTargets.clear();
}

void Render2D::RenderRect(int x, int y, int width, int height,glm::vec4 color)
{
	DrawTarget target;
	target.Type = RECTANGLE;
	target.Data["x"] = x;
	target.Data["y"] = y;
	target.Data["width"] = width;
	target.Data["height"] = height;
	target.Data["color"] = color;
	DrawTargets.push_back(target);
}

void Render2D::RenderText(std::string text, int x, int y, glm::vec4 color) {
	DrawTarget target;
	target.Type = TEXT;
	target.Data["x"] = x;
	target.Data["y"] = y;
	target.Data["text"] = text;
	target.Data["color"] = color;
	DrawTargets.push_back(target);
}

void Render2D::RenderTexturedRect(int x, int y, int width, int height, void* texture)
{
	DrawTarget target;
	target.Type = TEXTUREDRECTANGLE;
	target.Data["x"] = x;
	target.Data["y"] = y;
	target.Data["width"] = width;
	target.Data["height"] = height;
	target.Data["texture"] = texture;
	DrawTargets.push_back(target);
}
