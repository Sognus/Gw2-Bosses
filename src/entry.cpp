#include <Windows.h>
#include <string>
#include <cmath>
#include <unordered_map> // HashMap

#include "Globals.h"

// MODULE
HMODULE hSelf;

// Nexus
AddonAPI APIDefs;
AddonDefinition* AddonDef;
Mumble::Data* MumbleLink;
NexusLinkData* NexusLink;

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


void AddonUnload()
{
	//delete addon;
}


void AddonRender(bool aIsUIVisible) {
	addon->Render(aIsUIVisible);
}


void AddonLoad(AddonAPI aHostApi, void* mallocfn, void* freefn)
{
	APIDefs = aHostApi;
	MumbleLink = (Mumble::Data*)APIDefs.GetResource("DL_MUMBLE_LINK");
	ImGui::SetCurrentContext(aHostApi.ImguiContext);
	addon = new Addon();

	APIDefs.RegisterRender(AddonRender);
}



extern "C" __declspec(dllexport) AddonDefinition * GetAddonDef()
{
	AddonDef = new AddonDefinition();
	AddonDef->Signature = -1;
	AddonDef->APIVersion = NEXUS_API_VERSION;
	AddonDef->Name = "World bosses";
	Version.Major = 0;
	Version.Minor = 0;
	Version.Build = 1;
	Version.Revision = 0;
	AddonDef->Version = Version;
	AddonDef->Author = "Sognus";
	AddonDef->Description = "Adds meta events and world bosses notifications to in-game map.";
	AddonDef->Load = AddonLoad;
	AddonDef->Unload = AddonUnload;
	AddonDef->Flags = EAddonFlags::None;

	/* not necessary if hosted on Raidcore, but shown anyway for the  example also useful as a backup resource */
	AddonDef->Provider = EUpdateProvider::GitHub;
	AddonDef->UpdateLink = "https://github.com/Sognus/GW2-Bosses";

	return AddonDef;
}