#include "../Globals.h"
#include "BoundingBox.h"

#ifndef MAP_H
#define MAP_H

float map_object_scale();

float map_zoom_scale();

float map_font_scale();

BoundingBox map_get_bounding_box();

ImVec2 map_get_scale();


ImVec2 map_coords_to_pixels(ImVec2 mapCoords, BoundingBox viewport, ImVec2 mapScale);

#endif //MAP_H