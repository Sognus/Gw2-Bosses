#include "imgui\imgui.h"

// Constants 
#ifndef M_PI
#define M_PI (3.14159265358979323846f)
#endif

// DEBUG
const float DEBUG_LINE_THICKNESS = 2.0f;
const float DEBUG_LINE_LENGTH = 20.0f;

// STANDARD

const float MAP_MIN_ZOOM = 0.8533f;
const float MAP_MAX_ZOOM = 17.066f;

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
const float ENTRY_SEGMENTS = 256.0f;
const float ENTRY_NAME_OFFSET = 5.0f;

const float ENTRY_MAX_ZOOM_TEXT_VISIBILITY = 6.4f;

// Nexus constants
const std::string NLINK_NAME = "DL_NEXUS_LINK_";
const std::string IDENTITY_EVENT = "EV_MUMBLE_IDENTITY_UPDATED";
const std::string MUMBLE_LINK_RESOURCE = "DL_MUMBLE_LINK";