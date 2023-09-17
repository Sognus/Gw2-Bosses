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


void AddonLoad(AddonAPI aHostApi) 
{
	APIDefs = aHostApi;
	MumbleLink = (Mumble::Data*)APIDefs.GetResource("DL_MUMBLE_LINK");
	ImGui::SetCurrentContext(aHostApi.ImguiContext);
	addon = new Addon();
}



void AddonUnload() 
{
	delete addon;
}


void AddonRender() {
	addon->Render();
}

extern "C" __declspec(dllexport) AddonDefinition * GetAddonDef()
{
	AddonDef = new AddonDefinition();
	AddonDef->Signature = -1;
	AddonDef->Name = "World bosses";
	AddonDef->Version = "0.0.1";
	AddonDef->Author = "Sognus";
	AddonDef->Description = "Adds meta events and world bosses notifications to in-game map.";
	AddonDef->Load = AddonLoad;
	AddonDef->Unload = AddonUnload;
	AddonDef->Flags = EAddonFlags::None;

	AddonDef->Render = AddonRender;

	/* not necessary if hosted on Raidcore, but shown anyway for the  example also useful as a backup resource */
	AddonDef->Provider = EUpdateProvider::GitHub;
	AddonDef->UpdateLink = "https://github.com/Sognus/GW2-Bosses";

	return AddonDef;
}