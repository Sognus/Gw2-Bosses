#include "../Globals.h"
#include "BoundingBox.h"


BoundingBox map_get_bounding_box() {
	ImGuiIO& io = ImGui::GetIO();

	float mapCenterX = MumbleLink->Context.Compass.Center.X;
	float mapCenterY = MumbleLink->Context.Compass.Center.Y;
	float mapScale = MumbleLink->Context.Compass.Scale;

	float left = mapCenterX - ((io.DisplaySize.x / 2) * mapScale);
	float right = mapCenterX + ((io.DisplaySize.x / 2) * mapScale);
	float top = mapCenterY - ((io.DisplaySize.y / 2) * mapScale);
	float bottom = mapCenterY + ((io.DisplaySize.y / 2) * mapScale);

	return BoundingBox(top, left, bottom, right);
}

ImVec2 map_get_scale() {
	ImGuiIO& io = ImGui::GetIO();

	BoundingBox viewport = map_get_bounding_box();

	float scaleX = io.DisplaySize.x / viewport.GetSizeX();
	float scaleY = io.DisplaySize.y / viewport.GetSizeY();

	return ImVec2(scaleX, scaleY);
}