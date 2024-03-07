#include <Windows.h>
#include <string>
#include <cmath>
#include <unordered_map> // HashMap

#include "Globals.h"

// MODULE
HMODULE hSelf;

// Nexus
AddonAPI* APIDefs = nullptr;
AddonDefinition* AddonDef = nullptr;
Mumble::Data* MumbleLink = nullptr;
NexusLinkData* NexusLink = nullptr;
Mumble::Identity* MumbleIdentity = nullptr;

std::unordered_map<std::string, Texture*> resource_textures;


// Addon
Addon* addon;
AddonVersion Version;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: hSelf = hModule; break;
	case DLL_PROCESS_DETACH: break;
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	}
	return TRUE;
}

void ProcessKeybind(const char* aIdentifier) {
	std::string keybind = aIdentifier;

	if (addon) {

		if (keybind == KEY_BOSSES_TOGGLE_RENDER) {
			addon->render = !addon->render;
		}

		if (keybind == KEY_BOSSES_TOGGLE_NOTIFICATION) {
			addon->showNotifications = !addon->showNotifications;
		}

	}
}

void ReceiveTexture(const char* aIdentifier, Texture* aTexture)
{
	std::string stdIdentifier = aIdentifier;
	if (stdIdentifier.empty() || aTexture == nullptr) {
		return;
	}
	resource_textures[stdIdentifier] = aTexture;
}


void AddonRender() {
	if (!addon) {
		return;
	}
	addon->Render();
}


void OnMumbleIdentityUpdate(void* aEventArgs)
{
	MumbleIdentity = (Mumble::Identity*) aEventArgs;
}

void AddonLoad(AddonAPI* aHostApi)
{
	APIDefs = aHostApi;
	ImGui::SetCurrentContext(APIDefs->ImguiContext);
	ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))APIDefs->ImguiMalloc, (void(*)(void*, void*))APIDefs->ImguiFree); // on imgui 1.80+

	// Addon host
	MumbleLink = (Mumble::Data*)APIDefs->GetResource(MUMBLE_LINK_RESOURCE.c_str());
	NexusLink = (NexusLinkData*)APIDefs->GetResource(NLINK_NAME.c_str());
	APIDefs->SubscribeEvent(IDENTITY_EVENT.c_str(), OnMumbleIdentityUpdate);

	// Keybinds handler
	APIDefs->RegisterKeybindWithString(KEY_BOSSES_TOGGLE_RENDER.c_str(), ProcessKeybind, "CTRL+SHIFT+B");
	APIDefs->RegisterKeybindWithString(KEY_BOSSES_TOGGLE_NOTIFICATION.c_str(), ProcessKeybind, "CTRL+SHIFT+O");


	// Addon init
	addon = new Addon();

	// Resources
	APIDefs->LoadTextureFromResource(GW2BOSSES_RESOURCE_COREWORLDBOSSES_IN_PROGRESS.c_str(), IMAGE_COREWORLDBOSSES_IN_PROGRESS, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource(GW2BOSSES_RESOURCE_COREWORLDBOSSES_UPCOMING.c_str(), IMAGE_COREWORLDBOSSES_UPCOMING, hSelf, ReceiveTexture);

	// Render register
	APIDefs->RegisterRender(ERenderType::ERenderType_Render, AddonRender);
}

void AddonUnload()
{
	// Unregister keybinds
	APIDefs->UnregisterKeybind(KEY_BOSSES_TOGGLE_RENDER.c_str());
	APIDefs->UnregisterKeybind(KEY_BOSSES_TOGGLE_NOTIFICATION.c_str());

	// Unregister render
	APIDefs->UnregisterRender(AddonRender);
	// Unregister events
	APIDefs->UnsubscribeEvent(IDENTITY_EVENT.c_str(), OnMumbleIdentityUpdate);

	delete addon;
}

extern "C" __declspec(dllexport) AddonDefinition * GetAddonDef()
{
	AddonDef = new AddonDefinition();
	AddonDef->Signature = -1;
	AddonDef->APIVersion = NEXUS_API_VERSION;
	AddonDef->Name = "World bosses";
	Version.Major = 1;
	Version.Minor = 0;
	Version.Build = 0;
	Version.Revision = 0;
	AddonDef->Version = Version;
	AddonDef->Author = "Sognus";
	AddonDef->Description = "Adds meta events and world bosses notifications to in-game map.";
	AddonDef->Load = AddonLoad;
	AddonDef->Unload = AddonUnload;
	AddonDef->Flags = EAddonFlags::EAddonFlags_None;

	/* not necessary if hosted on Raidcore, but shown anyway for the  example also useful as a backup resource */
	AddonDef->Provider = EUpdateProvider::EUpdateProvider_GitHub;
	AddonDef->UpdateLink = "https://github.com/Sognus/GW2-Bosses";

	return AddonDef;
}