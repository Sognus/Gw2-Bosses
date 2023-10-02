#include "../Globals.h"
#include "BoundingBox.h"


float map_object_scale() {
	float zoom_level = MumbleLink->Context.Compass.Scale;
	float scale = (zoom_level - MAP_MIN_ZOOM) / (MAP_MAX_ZOOM - MAP_MIN_ZOOM);  
	return MAX_SCALE - scale * (MAX_SCALE - MIN_SCALE); 
}

float map_zoom_scale() {
	float zoom_level = MumbleLink->Context.Compass.Scale;
	float scale = (zoom_level - MAP_MIN_ZOOM) / (MAP_MAX_ZOOM - MAP_MIN_ZOOM);
	float scaled_value = ZOOM_OUT + std::pow(2, (-CURVE_CONVEXITY * scale)) * (ZOOM_IN - ZOOM_OUT);
	return scaled_value;
}

float map_font_scale() {
	float zoom_level = MumbleLink->Context.Compass.Scale;
	float scale = (zoom_level - MAP_MIN_ZOOM) / (MAP_MAX_ZOOM - MAP_MIN_ZOOM);
	float scaled_value = FONT_ZOOM_OUT + std::pow(2, (-CURVE_CONVEXITY * scale)) * (FONT_ZOOM_IN - FONT_ZOOM_OUT);
	return scaled_value;
}


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

ImVec2 map_coords_to_pixels(ImVec2 mapCoords, BoundingBox viewport, ImVec2 mapScale) {
	float pixelX = (mapCoords.x - viewport.GetLeft()) * mapScale.x;
	float pixelY = (mapCoords.y - viewport.GetTop()) * mapScale.y;
	return ImVec2(pixelX, pixelY);
}