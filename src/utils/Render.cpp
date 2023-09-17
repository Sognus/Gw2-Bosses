#include "../Globals.h"

void render_debug_crosshair() {
	float lineThickness = 2.0f;
	float lineLength = 20.0f;
	ImU32 color = IM_COL32(255, 0, 0, 255);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 center = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec2 horizontalStart(center.x - lineLength, center.y);
	ImVec2 horizontalEnd(center.x + lineLength, center.y);
	drawList->AddLine(horizontalStart, horizontalEnd, color, lineThickness);

	ImVec2 verticalStart(center.x, center.y - lineLength);
	ImVec2 verticalEnd(center.x, center.y + lineLength);
	drawList->AddLine(verticalStart, verticalEnd, color, lineThickness);
}