#include "imgui\imgui.h"

// Constants 
#ifndef M_PI
#define M_PI (3.14159265358979323846f)
#endif

// Data version (for data.json)

// DEBUG
const float DEBUG_LINE_THICKNESS = 2.0f;
const float DEBUG_LINE_LENGTH = 20.0f;

// STANDARD

const float MAP_MIN_ZOOM_UISIZE_0 = 00.7681f;
const float MAP_MAX_ZOOM_UISIZE_0 = 15.3620f;

const float MAP_MIN_ZOOM_UISIZE_1 = 00.8533f;
const float MAP_MAX_ZOOM_UISIZE_1 = 17.0660f;

const float MAP_MIN_ZOOM_UISIZE_2 = 00.9481f;
const float MAP_MAX_ZOOM_UISIZE_2 = 18.9630f;

const float MAP_MIN_ZOOM_UISIZE_3 = 01.0000f;
const float MAP_MAX_ZOOM_UISIZE_3 = 20.0000f;

const float MAX_SCALE = 1.0f;
const float MIN_SCALE = 0.4f;

// Zoomed OUT
const float ZOOM_OUT = 0.60f;
// Zoomed IN
const float ZOOM_IN = 3.0f;
// Bigger values means that values stay at ZOOM_OUT value longer 
const float CURVE_CONVEXITY = 5;

// Font scaling
const float FONT_ZOOM_OUT = 0.2f;
const float FONT_ZOOM_IN = 0.9f;

const ImU32 RED = IM_COL32(255, 0, 0, 255);
const ImU32 GREEN = IM_COL32(0, 255, 0, 255);
const ImU32 BLUE = IM_COL32(0, 0, 255, 255);

const float ENTRY_RADIUS = 30.0f;
const float ENTRY_ARC_OFFSET = -(M_PI / 2);
const float ENTRY_SEGMENTS = 64.0f;
const float ENTRY_NAME_OFFSET = 5.0f;

const float ENTRY_MAX_ZOOM_TEXT_VISIBILITY = 6.4f;

// Addon constants
const std::string ADDON_NAME = "World bosses";
const std::string GW2_BOSSES_SHORTCUT = "QAS_BOSSES";

// Nexus constants
const std::string NLINK_NAME = "DL_NEXUS_LINK";
const std::string IDENTITY_EVENT = "EV_MUMBLE_IDENTITY_UPDATED";
const std::string MUMBLE_LINK_RESOURCE = "DL_MUMBLE_LINK";

// Keybinds
const std::string KEY_BOSSES_TOGGLE_RENDER = "KEY_BOSSES_TOGGLE_RENDER";
const std::string KEY_BOSSES_TOGGLE_NOTIFICATION = "KEY_BOSSES_TOGGLE_NOTIFICATION";

// Resources
const std::string GW2BOSSES_RESOURCE_COREWORLDBOSSES_IN_PROGRESS = "GW2BOSSES_RESOURCE_COREWORLDBOSSES_IN_PROGRESS";
const std::string GW2BOSSES_RESOURCE_COREWORLDBOSSES_UPCOMING = "GW2BOSSES_RESOURCE_COREWORLDBOSSES_UPCOMING";
const std::string GW2BOSSES_RESOURCE_COREWORLDBOSSES_GRAY = "GW2BOSSES_RESOURCE_COREWORLDBOSSES_GRAY";
const std::string GW2BOSSES_RESOURCE_COREWORLDBOSSES_IN_EDIT = "GW2BOSSES_RESOURCE_COREWORLDBOSSES_IN_EDIT";