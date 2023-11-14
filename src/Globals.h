#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

#include "nexus/Nexus.h"
#include "imgui\imgui.h"
#include "imgui\imgui_extensions.h"
#include "resource.h"
#include "mumble/Mumble.h"

#include "Constants.h"
#include "Addon.h"

extern AddonAPI* APIDefs;
extern AddonDefinition* AddonDef;
extern Mumble::Data* MumbleLink;
extern NexusLinkData* NexusLink;

extern Addon* addon;

#endif // GLOBALS_H