#ifndef GLOBALS_H
#define GLOBALS_H

#ifndef NOMINMAX
	#define NOMINMAX
#endif // !NOMINMAX

#ifndef UNICODE
	#define UNICODE
#endif // !UNICODE

#ifndef STRICT
	#define STRICT
#endif // !STRICT

#include <string>

#include "nexus/Nexus.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "mumble/Mumble.h"

#include "Constants.h"
#include "Addon.h"
#include "../resource.h"

#include "GW2BossesVersion.h"

extern AddonAPI* APIDefs;
extern AddonDefinition* AddonDef;
extern Mumble::Data* MumbleLink;
extern NexusLinkData* NexusLink;
extern Mumble::Identity* MumbleIdentity;

extern Addon* addon;
extern std::unordered_map<std::string, Texture*> resource_textures;

extern bool isLeftMouseClicked;
extern bool isControlPressed;


#endif // GLOBALS_H