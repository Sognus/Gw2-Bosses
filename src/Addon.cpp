#include "Globals.h"
#include "utils/Render.h"
#include "utils/Map.h"
#include "utils/BoundingBox.h"
#include "Event.h"

Event exampleEvent = Event("Example Event", 46545.0f, 21450.0f);

Addon::Addon() {
	// Constructor
}

Addon::~Addon() {
	// Destructor implementation
}


void Addon::Render() {
	render_debug_crosshair();
	this->RenderEvents();
	this->Update();

}

void Addon::RenderEvents() {
	if (!MumbleLink->Context.IsMapOpen) return;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);

	BoundingBox viewport = map_get_bounding_box();

	// Calculate scaling factors for X and Y axes;
	ImVec2 mapScaleX = map_get_scale();


	if (ImGui::Begin("PLENGA", (bool*)0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing)) {

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 pixelCenter = map_coords_to_pixels(exampleEvent.GetLocation(), viewport, mapScaleX);

		// Update scale for render
		exampleEvent.SetScale(map_object_scale());

		ImGui::SetCursorPos(pixelCenter);

		float radius = ENTRY_RADIUS * exampleEvent.GetScale();
		float totalAngle = (exampleEvent.GetPercentage() / 100.0f) * (2 * M_PI);

		int segments = 64; // Number of segments
		drawList->AddCircleFilled(pixelCenter, radius, GREEN, segments);

		// Shift the starting angle to 12 o'clock
		float angleStep = totalAngle / ENTRY_SEGMENTS;
		float startingAngle = ENTRY_ARC_OFFSET; // Start at 12 o'clock

		for (int i = 0; i < segments; ++i) {
			float startAngle = startingAngle + angleStep * i;
			float endAngle = startingAngle + angleStep * (i + 1);
			drawList->PathLineTo(pixelCenter);
			drawList->PathArcTo(pixelCenter, radius, startAngle, endAngle);
			drawList->PathFillConvex(RED);
		}

		ImVec2 textSize = ImGui::CalcTextSize(exampleEvent.GetName().c_str());
		ImVec2 textPos = ImVec2(pixelCenter.x - (textSize.x / 2), pixelCenter.y + radius + 5);
		ImGui::SetCursorPos(textPos);
		ImGui::Text(exampleEvent.GetName().c_str());

		// Tooltip
		ImVec2 mousePos = io.MousePos;
		float distance = sqrt((mousePos.x - pixelCenter.x) * (mousePos.x - pixelCenter.x) + (mousePos.y - pixelCenter.y) * (mousePos.y - pixelCenter.y));
		if (distance <= radius) {
			ImGui::SetTooltip("Event %s\nPercentage: %.2f\nScale: %.2f", exampleEvent.GetName().c_str(), exampleEvent.GetPercentage(), exampleEvent.GetScale());
		}

	}
	ImGui::End();
}

void Addon::Update() {
	float piePercentage = exampleEvent.GetPercentage();
	piePercentage = piePercentage + 0.1f;
	if (piePercentage > 100.0f) {
		piePercentage = 0.0f;
	}
	exampleEvent.SetPercentage(piePercentage);
}