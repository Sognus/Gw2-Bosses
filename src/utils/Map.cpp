#include "../Globals.h"
#include "BoundingBox.h"

#include "../tinyxml2/tinyxml2.h"

int dpi_scale_cache_last_tick = 0;
bool dpi_scale_cache = false;


float map_get_min_zoom(Mumble::EUIScale uiSize) {
	switch (uiSize) {
	case Mumble::EUIScale::Small:
		return MAP_MIN_ZOOM_UISIZE_0;
	case Mumble::EUIScale::Large:
		return MAP_MIN_ZOOM_UISIZE_2;
	case Mumble::EUIScale::Larger:
		return MAP_MIN_ZOOM_UISIZE_3;
	case Mumble::EUIScale::Normal:
	default:
		return MAP_MIN_ZOOM_UISIZE_1;
	}
}

float map_get_max_zoom(Mumble::EUIScale uiSize) {
	switch (uiSize) {
	case Mumble::EUIScale::Small:
		return MAP_MAX_ZOOM_UISIZE_0;
	case Mumble::EUIScale::Large:
		return MAP_MAX_ZOOM_UISIZE_2;
	case Mumble::EUIScale::Larger:
		return MAP_MAX_ZOOM_UISIZE_3;
	case Mumble::EUIScale::Normal:
	default:
		return MAP_MAX_ZOOM_UISIZE_1;
	}
}

Mumble::EUIScale get_ui_scale() {
	if (!MumbleIdentity) {
		APIDefs->Log(ELogLevel::ELogLevel_WARNING, "get_ui_scale: Mumble Identity not initialized");
		return Mumble::EUIScale::Normal;
	}
	return MumbleIdentity->UISize;
}


float get_system_dpi_scale_percentage() {
	return GetDpiForSystem() / 96.0f;
}

bool is_use_dpi_scale_checked() {
	// Check current path of XML settings
	char appDataLocationCstr[1000];
	int s = GetEnvironmentVariableA("APPDATA", appDataLocationCstr, 1000);

	if (s == 0) {
		APIDefs->Log(ELogLevel_WARNING, "DPI Scale: Could not read GW2 settings location.");
		return false;
	}

	std::string documentPath = std::string(appDataLocationCstr) + "\\Guild Wars 2\\GFXSettings.Gw2-64.exe.xml";

	tinyxml2::XMLDocument document;
	if (document.LoadFile(documentPath.c_str()) != tinyxml2::XML_SUCCESS) {
		std::string message = std::string("DPI scaling: Could not load settings file: ") + documentPath;
		APIDefs->Log(ELogLevel_WARNING, message.c_str());
		return false;
	}

	tinyxml2::XMLElement* root = document.FirstChildElement("GSA_SDK");
	if (!root) {
		std::string message = std::string("DPI scaling: Could not parse settings: GSA_SDK does not exist");
		APIDefs->Log(ELogLevel_WARNING, message.c_str());
		return false;
	}

	tinyxml2::XMLElement* gameSettings = root->FirstChildElement("GAMESETTINGS");
	if (!gameSettings) {
		std::string message = std::string("DPI scaling: Could not parse settings: GSA_SDK->GAMESETTINGS does not exist");
		APIDefs->Log(ELogLevel_WARNING, message.c_str());
		return false;
	}

	tinyxml2::XMLElement* option = gameSettings->FirstChildElement("OPTION");
	while (option) {
		const char* name = option->Attribute("Name");
		if (name && std::string(name) == "dpiScaling") {
			const char* value = option->Attribute("Value");
			if (value) {
				// Check if value is true
				std::string valueStr = value;
				return valueStr.compare("true") == 0;
			}
		}
		option = option->NextSiblingElement("OPTION");
	}

	std::string message = std::string("DPI scaling: Could not parse settings: GSA_SDK->GAMESETTINGS->dpiScaling does not exist");
	APIDefs->Log(ELogLevel_WARNING, message.c_str());
	return false;
}

bool is_use_dpi_scale_checked_cached() {
	// Wait 60 frames before querying dpi_scale again
	if (MumbleLink->UITick - dpi_scale_cache_last_tick >= 30) {
		dpi_scale_cache_last_tick = MumbleLink->UITick;
		dpi_scale_cache = is_use_dpi_scale_checked();
	}
	return dpi_scale_cache;
}



float get_current_dpi_scale() {
	if (!is_use_dpi_scale_checked_cached()) {
		return 1.0f;
	}
	return get_system_dpi_scale_percentage();
}


float get_map_scale_normal() {
	Mumble::EUIScale UI_SCALE = get_ui_scale();
	
	// Get compass scale from Mumble
	float mapScale = MumbleLink->Context.Compass.Scale;
	mapScale = round(mapScale * 1000) / 1000;
	
	// Check if DPI scale is used
	bool useDPI = is_use_dpi_scale_checked_cached();

	// Calculate DPI scale
	if (!useDPI && !addon->enableDPIScaleOverride) {

		const float NORMAL_MAP_MIN_ZOOM = map_get_min_zoom(Mumble::EUIScale::Normal);
		const float NORMAL_MAP_MAX_ZOOM = map_get_max_zoom(Mumble::EUIScale::Normal);
		const float MAP_MIN_ZOOM = map_get_min_zoom(UI_SCALE);
		const float MAP_MAX_ZOOM = map_get_max_zoom(UI_SCALE);

		// Calculate the percentage between MAP_MIN_ZOOM and MAP_MAX_ZOOM
		float percentage = (mapScale - MAP_MIN_ZOOM) / (MAP_MAX_ZOOM - MAP_MIN_ZOOM);
		percentage = std::clamp(percentage, 0.0f, 1.0f);

		// Map the percentage to the range NORMAL_MAP_MIN_ZOOM to NORMAL_MAP_MAX_ZOOM
		float tempMapScale = percentage * (NORMAL_MAP_MAX_ZOOM - NORMAL_MAP_MIN_ZOOM) + NORMAL_MAP_MIN_ZOOM;
		mapScale = std::clamp(tempMapScale, NORMAL_MAP_MIN_ZOOM, NORMAL_MAP_MAX_ZOOM);
	}
	else {

		float dpiScale = get_current_dpi_scale();

		if (addon->enableDPIScaleOverride) {
			dpiScale = addon->DPIScaleOverride;
		}

		const float NORMAL_MAP_MIN_ZOOM = map_get_min_zoom(Mumble::EUIScale::Normal);
		const float NORMAL_MAP_MAX_ZOOM = map_get_max_zoom(Mumble::EUIScale::Normal);
		const float MAP_MIN_ZOOM = map_get_min_zoom(UI_SCALE) * dpiScale;
		const float MAP_MAX_ZOOM = map_get_max_zoom(UI_SCALE) * dpiScale;

		// Calculate the percentage between MAP_MIN_ZOOM and MAP_MAX_ZOOM
		float percentage = (mapScale - MAP_MIN_ZOOM) / (MAP_MAX_ZOOM - MAP_MIN_ZOOM);
		percentage = std::clamp(percentage, 0.0f, 1.0f);

		// Map the percentage to the range NORMAL_MAP_MIN_ZOOM to NORMAL_MAP_MAX_ZOOM
		float tempMapScale = percentage * (NORMAL_MAP_MAX_ZOOM - NORMAL_MAP_MIN_ZOOM) + NORMAL_MAP_MIN_ZOOM;
		mapScale = std::clamp(tempMapScale, NORMAL_MAP_MIN_ZOOM, NORMAL_MAP_MAX_ZOOM);
	}

	return mapScale;
}



float map_object_scale() {
	Mumble::EUIScale UI_SCALE = get_ui_scale();
	const float MAP_MIN_ZOOM = map_get_min_zoom(UI_SCALE);
	const float MAP_MAX_ZOOM = map_get_max_zoom(UI_SCALE);
	
	//float zoom_level = MumbleLink->Context.Compass.Scale;
	float zoom_level = get_map_scale_normal();
	
	float scale = (zoom_level - MAP_MIN_ZOOM) / (MAP_MAX_ZOOM - MAP_MIN_ZOOM);
	return MAX_SCALE - scale * (MAX_SCALE - MIN_SCALE);
}

float map_zoom_scale() {
	Mumble::EUIScale UI_SCALE = get_ui_scale();
	const float MAP_MIN_ZOOM = map_get_min_zoom(UI_SCALE);
	const float MAP_MAX_ZOOM = map_get_max_zoom(UI_SCALE);


	//float zoom_level = MumbleLink->Context.Compass.Scale;
	float zoom_level = get_map_scale_normal();

	float scale = (zoom_level - MAP_MIN_ZOOM) / (MAP_MAX_ZOOM - MAP_MIN_ZOOM);
	float scaled_value = ZOOM_OUT + std::pow(2.0f, (-CURVE_CONVEXITY * scale)) * (ZOOM_IN - ZOOM_OUT);
	return scaled_value;
}

float map_font_scale() {
	Mumble::EUIScale UI_SCALE = get_ui_scale();
	const float MAP_MIN_ZOOM = map_get_min_zoom(UI_SCALE);
	const float MAP_MAX_ZOOM = map_get_max_zoom(UI_SCALE);

	//float zoom_level = MumbleLink->Context.Compass.Scale;
	float zoom_level = get_map_scale_normal();

	float scale = (zoom_level - MAP_MIN_ZOOM) / (MAP_MAX_ZOOM - MAP_MIN_ZOOM);
	float scaled_value = FONT_ZOOM_OUT + std::pow(2.0f, (-CURVE_CONVEXITY * scale)) * (FONT_ZOOM_IN - FONT_ZOOM_OUT);
	return scaled_value;
}


BoundingBox map_get_bounding_box() {
	ImGuiIO& io = ImGui::GetIO();

	float mapCenterX = MumbleLink->Context.Compass.Center.X;
	float mapCenterY = MumbleLink->Context.Compass.Center.Y;
	
	
	//float mapScale = MumbleLink->Context.Compass.Scale;
	float mapScale = get_map_scale_normal();

	float left = mapCenterX - ((io.DisplaySize.x / 2.0f) * mapScale);
	float right = mapCenterX + ((io.DisplaySize.x / 2.0f) * mapScale);
	float top = mapCenterY - ((io.DisplaySize.y / 2.0f) * mapScale);
	float bottom = mapCenterY + ((io.DisplaySize.y / 2.0f) * mapScale);

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