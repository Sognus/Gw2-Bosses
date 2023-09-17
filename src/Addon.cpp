#include "Globals.h"
#include "utils/Render.h"
#include "utils/Map.h"

// DEBUG
int x = 0;
float textureScale = 1.0;
float piePercentage = 10;

Addon::Addon() {
	// Constructor
}

Addon::~Addon() {
	// Destructor implementation
}


void Addon::Render() {
	if (!MumbleLink->Context.IsMapOpen) return;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);

	// Calculate current bounding box 
	float mapCenterX = MumbleLink->Context.Compass.Center.X;
	float mapCenterY = MumbleLink->Context.Compass.Center.Y;
	float mapScale = MumbleLink->Context.Compass.Scale;

	float left = mapCenterX - ((io.DisplaySize.x / 2) * mapScale);
	float right = mapCenterX + ((io.DisplaySize.x / 2) * mapScale);
	float top = mapCenterY - ((io.DisplaySize.y / 2) * mapScale);
	float bottom = mapCenterY + ((io.DisplaySize.y / 2) * mapScale);

	ImVec2 topLeft = ImVec2(left, top);
	ImVec2 bottomRight = ImVec2(right, bottom);

	// Calculate scaling factors for X and Y axes
	ImVec2 mapScaleX = map_get_scale();

	if (ImGui::Begin("PLENGA", (bool*)0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing)) {

		render_debug_crosshair();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImU32 color = IM_COL32(255, 0, 0, 255);

		// Try to draw position
		float mapX = 46545.0;
		float mapY = 21450.0;

		float pixelX = (mapX - left) * mapScaleX.x;
		float pixelY = (mapY - top) * mapScaleX.y;

		ImVec2 pixelCenter = ImVec2(pixelX, pixelY);


		float min_zoom = 0.8533;
		float max_zoom = 17.066;
		float max_scale = 1.0;
		float min_scale = 0.4;
		float zoom_level = MumbleLink->Context.Compass.Scale;
		float scale = (zoom_level - min_zoom) / (max_zoom - min_zoom);  // Calculate the relative position
		textureScale = max_scale - scale * (max_scale - min_scale); // Linear interpolation - zoom in = larger scale


		ImGui::SetCursorPos(pixelCenter);

		float radius = 30.0f * textureScale;
		float totalAngle = (piePercentage / 100.0f) * (2 * M_PI);

		int segments = 64; // Number of segments
		drawList->AddCircleFilled(pixelCenter, radius, IM_COL32(0, 255, 0, 255), segments);

		// Shift the starting angle to 12 o'clock
		float angleStep = totalAngle / segments;
		float startingAngle = -1.57079633; // Start at 12 o'clock

		for (int i = 0; i < segments; ++i) {
			float startAngle = startingAngle + angleStep * i;
			float endAngle = startingAngle + angleStep * (i + 1);
			drawList->PathLineTo(pixelCenter);
			drawList->PathArcTo(pixelCenter, radius, startAngle, endAngle);
			drawList->PathFillConvex(color);
		}


		// Text
		ImVec2 textSize = ImGui::CalcTextSize("Boss");
		ImVec2 textPos = ImVec2(pixelCenter.x - (textSize.x / 2), pixelCenter.y + radius + 5);
		ImGui::SetCursorPos(textPos);
		ImGui::Text("Boss", piePercentage);

		// Tooltip
		ImVec2 mousePos = io.MousePos;
		float distance = sqrt((mousePos.x - pixelCenter.x) * (mousePos.x - pixelCenter.x) + (mousePos.y - pixelCenter.y) * (mousePos.y - pixelCenter.y));
		if (distance <= radius) {
			ImGui::SetTooltip("%s\n%.2f", "Examble boss tooltip\nWith multiple lines\nxddTree", textureScale);
		}

		piePercentage = piePercentage + 0.1f;
		if (piePercentage > 100.0f) {
			piePercentage = 0.0f;
		}


	}
	ImGui::End();

}