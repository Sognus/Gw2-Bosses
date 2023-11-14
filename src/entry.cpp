#include <Windows.h>
#include <string>
#include <cmath>
#include <unordered_map> // HashMap

#include "Globals.h"

// MODULE
HMODULE hSelf;

// Nexus
AddonAPI* APIDefs;
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


void AddonRender() {
	if (!addon) {
		return;
	}
	addon->Render();
}


void OnMumbleIdentityUpdate(void* aEventArgs)
{
	Mumble::Identity* identity = (Mumble::Identity*)aEventArgs;
}

void AddonLoad(AddonAPI* aHostApi)
{
	APIDefs = aHostApi;
	ImGui::SetCurrentContext(APIDefs->ImguiContext);
	ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))APIDefs->ImguiMalloc, (void(*)(void*, void*))APIDefs->ImguiFree); // on imgui 1.80+

	// Addon host
	std::string nexusLinkName = "";
	nexusLinkName.append(NLINK_NAME);
	nexusLinkName.append(std::to_string(GetCurrentProcessId()));

	MumbleLink = (Mumble::Data*)APIDefs->GetResource(MUMBLE_LINK_RESOURCE.c_str());
	NexusLink = (NexusLinkData*)APIDefs->GetResource(nexusLinkName.c_str());
	APIDefs->SubscribeEvent(IDENTITY_EVENT.c_str(), OnMumbleIdentityUpdate);

	// Addon init
	addon = new Addon();

	// Render register
	APIDefs->RegisterRender(ERenderType::Render, AddonRender);
}



extern "C" __declspec(dllexport) AddonDefinition * GetAddonDef()
{
	AddonDef = new AddonDefinition();
	AddonDef->Signature = -1;
	AddonDef->APIVersion = NEXUS_API_VERSION;
	AddonDef->Name = "World bosses";
	Version.Major = 0;
	Version.Minor = 9;
	Version.Build = 0;
	Version.Revision = 1;
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