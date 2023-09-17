#ifndef GLOBALS_H
#define GLOBALS_H

#include "nexus/Nexus.h"
#include "imgui\imgui.h"
#include "imgui\imgui_extensions.h"
#include "resource.h"
#include "mumble/Mumble.h"

#include "Addon.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

extern AddonAPI APIDefs;
extern AddonDefinition* AddonDef;
extern Mumble::Data* MumbleLink;
extern NexusLinkData* NexusLink;

extern Addon* addon;

#endif // GLOBALS_H