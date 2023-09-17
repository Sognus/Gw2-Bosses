#include "../Globals.h"
#include "../Event.h"
#include "BoundingBox.h"
#include "Map.h"

void render_debug_crosshair() {
	if (!MumbleLink->Context.IsMapOpen) return;

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


void render_event_circle(ImVec2 location, float scale) {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	float radius = ENTRY_RADIUS * scale;
	drawList->AddCircleFilled(location, radius, GREEN, ENTRY_SEGMENTS);
}

void render_event_progress(ImVec2 location, float percentage, float scale) {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	float radius = ENTRY_RADIUS * scale;
	float totalAngle = (percentage / 100.0f) * (2.0f * M_PI);

	float angleStep = totalAngle / ENTRY_SEGMENTS;
	float startingAngle = ENTRY_ARC_OFFSET; 

	for (int i = 0; i < ENTRY_SEGMENTS; ++i) {
		float startAngle = startingAngle + angleStep * i;
		float endAngle = startingAngle + angleStep * (i + 1);
		drawList->PathLineTo(location);
		drawList->PathArcTo(location, radius, startAngle, endAngle);
		drawList->PathFillConvex(RED);
	}

}

void render_event_name(std::string name, ImVec2 location, float scale) {
	float radius = ENTRY_RADIUS * scale;
	ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
	ImVec2 textPos = ImVec2(location.x - (textSize.x / 2), location.y + radius + (ENTRY_NAME_OFFSET * scale));
	ImGui::SetCursorPos(textPos);
	ImGui::Text(name.c_str());
}

void render_tooltip(Event event, ImVec2 location) {
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mousePos = io.MousePos;
	float radius = ENTRY_RADIUS * event.GetScale();

	float distance = sqrt((mousePos.x - location.x) * (mousePos.x - location.x) + (mousePos.y - location.y) * (mousePos.y - location.y));
	if (distance <= radius) {
		ImGui::SetTooltip("Event %s\nPercentage: %.2f\nScale: %.2f", event.GetName().c_str(), event.GetPercentage(), event.GetScale());
	}
}

void render_event(Event event) {
	BoundingBox viewport = map_get_bounding_box();

	// Calculate scaling factors for X and Y axes;
	ImVec2 mapScaleX = map_get_scale();

	// Update necessary data for render
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 location = map_coords_to_pixels(event.GetLocation(), viewport, mapScaleX);
	event.SetScale(map_object_scale());

	// Render circle and progress arc
	ImGui::SetCursorPos(location);
	render_event_circle(location, event.GetScale());
	render_event_progress(location, event.GetPercentage(), event.GetScale());

	// Render text
	render_event_name(event.GetName(), location, event.GetScale());

	// Render tooltip
	render_tooltip(event, location);
}