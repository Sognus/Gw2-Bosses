#include "../Globals.h"

void render_debug_crosshair() {

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 center = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec2 horizontalStart(center.x - DEBUG_LINE_LENGTH, center.y);
	ImVec2 horizontalEnd(center.x + DEBUG_LINE_LENGTH, center.y);
	drawList->AddLine(horizontalStart, horizontalEnd, RED, DEBUG_LINE_THICKNESS);

	ImVec2 verticalStart(center.x, center.y - DEBUG_LINE_LENGTH);
	ImVec2 verticalEnd(center.x, center.y + DEBUG_LINE_LENGTH);
	drawList->AddLine(verticalStart, verticalEnd, RED, DEBUG_LINE_THICKNESS);
}