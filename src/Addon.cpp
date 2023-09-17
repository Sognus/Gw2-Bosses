#include "Globals.h"
#include "utils/Render.h"
#include "utils/Map.h"
#include "utils/BoundingBox.h"
#include "Event.h"

Event exampleEvent = Event("Example Event", 46545.0f, 21450.0f);
Event exampleEvent2 = Event("Example Event 2", 57600.0f, 21592.0f);

Addon::Addon() {
	// Constructor
}

Addon::~Addon() {
	// Destructor implementation
}


void Addon::Render() {
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);

	if (ImGui::Begin("GW2_BOSSES", (bool*)0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing)) {
		render_debug_crosshair();
		this->RenderEvents();
		this->Update();
	}
	ImGui::End();
}

void Addon::RenderEvents() {
	if (!MumbleLink->Context.IsMapOpen) return;
	
	render_event(exampleEvent);
	render_event(exampleEvent2);
}

void Addon::Update() {
	float piePercentage = exampleEvent.GetPercentage();
	piePercentage = piePercentage + 0.1f;
	if (piePercentage > 100.0f) {
		piePercentage = 0.0f;
	}
	exampleEvent.SetPercentage(piePercentage);


	float piePercentage2 = exampleEvent2.GetPercentage();
	piePercentage2 = piePercentage2 + 0.2f;
	if (piePercentage2 > 100.0f) {
		piePercentage2 = 0.0f;
	}
	exampleEvent2.SetPercentage(piePercentage2);
}