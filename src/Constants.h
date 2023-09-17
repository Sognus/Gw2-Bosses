#include "imgui\imgui.h"

// DEBUG
const float DEBUG_LINE_THICKNESS = 2.0f;
const float DEBUG_LINE_LENGTH = 20.0f;


// STANDARD

const float MAP_MIN_ZOOM = 0.8533f;
const float MAP_MAX_ZOOM = 17.066f;

const float MAX_SCALE = 1.0f;
const float MIN_SCALE = 0.4f;

const ImU32 RED = IM_COL32(255, 0, 0, 255);
const ImU32 GREEN = IM_COL32(0, 255, 0, 255);
const ImU32 BLUE = IM_COL32(0, 0, 255, 255);

const float ENTRY_RADIUS = 30.0f;
const float ENTRY_ARC_OFFSET = -1.57079633f;
const float ENTRY_SEGMENTS = 64;
const float ENTRY_NAME_OFFSET = 5.0f;


#ifndef M_PI
#define M_PI (3.14159265358979323846f)
#endif