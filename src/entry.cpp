#include <Windows.h>
#include <string>
#include <cmath>
#include <unordered_map> // HashMap

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#include "nexus/Nexus.h"
#include "imgui\imgui.h"
#include "imgui\imgui_extensions.h"
#include "resource.h"
#include "mumble/Mumble.h"

HMODULE hSelf;

AddonAPI APIDefs;
AddonDefinition* AddonDef;
Mumble::Data* MumbleLink = nullptr;
NexusLinkData* NexusLink = nullptr;

// Texture storage
std::unordered_map<std::string, Texture*> textureMap;

// DEBUG
int x = 0;
float textureScale = 1.0;
float piePercentage = 10;

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

void ReceiveTexture(std::string aIdentifier, Texture* aTexture)
{
}


void AddonLoad(AddonAPI aHostApi) 
{
	APIDefs = aHostApi;
	MumbleLink = (Mumble::Data*)APIDefs.GetResource("DL_MUMBLE_LINK");

	ImGui::SetCurrentContext(aHostApi.ImguiContext);

	x = 500;
}



void AddonUnload() 
{
}



void AddonRender()
{
	if (!MumbleLink->Context.IsMapOpen) return;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);

	// Calculate current bounding box 
	float mapCenterX = MumbleLink->Context.Compass.Center.X;
	float mapCenterY = MumbleLink->Context.Compass.Center.Y;
	float mapScale = MumbleLink->Context.Compass.Scale;

	float left = mapCenterX - ((io.DisplaySize.x / 2) * mapScale);
	float right = mapCenterX + ((io.DisplaySize.x / 2) * mapScale);
	float top = mapCenterY - ((io.DisplaySize.y / 2) * mapScale);
	float bottom = mapCenterY + ((io.DisplaySize.y / 2) * mapScale);

	ImVec2 topLeft = ImVec2(left, top);
	ImVec2 bottomRight = ImVec2(right, bottom);

	// Calculate scaling factors for X and Y axes
	float scaleX = io.DisplaySize.x / (right - left);
	float scaleY = io.DisplaySize.y / (bottom - top);



	ImDrawList* drawList = ImGui::GetWindowDrawList();



	if (ImGui::Begin("PLENGA", (bool*)0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing)) {
		
		// Draw Lines
		ImVec2 windowSize = ImGui::GetWindowSize();
		// Calculate the center of the window
		ImVec2 center = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);
		// Get the ImGui draw list for the current ImGui window
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		// Define the length of the crosshair lines
		float lineLength = 20.0f;
		// Define the color of the crosshair lines (in RGBA format)
		ImU32 color = IM_COL32(255, 0, 0, 255); // Red color
		// Set the line thickness (optional)
		float thickness = 2.0f; // Replace with your desired thickness
		// Draw the horizontal line
		ImVec2 horizontalStart(center.x - lineLength, center.y);
		ImVec2 horizontalEnd(center.x + lineLength, center.y);
		drawList->AddLine(horizontalStart, horizontalEnd, color, thickness);
		// Draw the vertical line
		ImVec2 verticalStart(center.x, center.y - lineLength);
		ImVec2 verticalEnd(center.x, center.y + lineLength);
		drawList->AddLine(verticalStart, verticalEnd, color, thickness);


		// Draw Texture
		auto iterator = textureMap.find("TEXTURE_CIRCLE_GREEN");
		
		if (iterator != textureMap.end() || true) {
			//Texture* texture = iterator->second;

			// Try to draw position
			float mapX = 46545.0;
			float mapY = 21450.0;

			float pixelX = (mapX - left) * scaleX;
			float pixelY = (mapY - top) * scaleY;

			ImVec2 pixelCenter = ImVec2(pixelX, pixelY);

			//ImGui::SetCursorPos(ImVec2(pixelX - texture->Width / 2, pixelY - texture->Height / 2));
			ImGui::SetCursorPos(pixelCenter);
			//textureScale = 1.0;
			//ImGui::Image(texture->Resource, ImVec2(texture->Width * textureScale, texture->Height * textureScale));

			float radius = 30.0f * textureScale;
			float totalAngle = (piePercentage / 100.0f) * (2 * M_PI);

			int segments = 64; // Number of segments
			drawList->AddCircleFilled(pixelCenter, radius, IM_COL32(0, 255, 0, 255), segments);

			// Shift the starting angle to 12 o'clock
			float angleStep = totalAngle / segments;
			float startingAngle = -1.57079633; // Start at 12 o'clock

			for (int i = 0; i < segments; ++i) {
				float startAngle = startingAngle + angleStep * i;
				float endAngle = startingAngle + angleStep * (i + 1);
				drawList->PathLineTo(pixelCenter);
				drawList->PathArcTo(pixelCenter, radius, startAngle, endAngle);
				drawList->PathFillConvex(color);
			}


			// Text
			ImVec2 textSize = ImGui::CalcTextSize("Boss");
			ImVec2 textPos = ImVec2(pixelCenter.x - (textSize.x / 2), pixelCenter.y + radius + 5);
			ImGui::SetCursorPos(textPos);
			ImGui::Text("Boss", piePercentage);

			// Tooltip
			ImVec2 mousePos = io.MousePos;
			float distance = sqrt((mousePos.x - pixelCenter.x) * (mousePos.x - pixelCenter.x) +(mousePos.y - pixelCenter.y) * (mousePos.y - pixelCenter.y));
			if (distance <= radius) {
				ImGui::SetTooltip("%s", "Examble boss tooltip\nWith multiple lines\nxddTree");
			}



			piePercentage = piePercentage + 0.1f;
			if (piePercentage > 100.0f) {
				piePercentage = 0.0f;
			}

			/*
			float min_zoom = 0.8533;
			float max_zoom = 17.066;
			float max_scale = 3.0;
			float min_scale = 1.0;
			float zoom_level = MumbleLink->Context.Compass.Scale;

			float scale = (zoom_level - min_zoom) / (max_zoom - min_zoom);  // Calculate the relative position
			textureScale = max_scale - scale * (max_scale - min_scale); // Linear interpolation - zoom in = larger scale

			ImGui::Image(texture->Resource, ImVec2(texture->Width * textureScale, texture->Height * textureScale));
						x += 1;
			if (x > 1920) x = 0;

			*/





		}
		else {
			ImGui::Text("Error loading TEXTURE");
		}

	}
	ImGui::End();

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