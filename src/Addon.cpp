#include "Globals.h"
#include "utils/Render.h"
#include "utils/Map.h"
#include "utils/BoundingBox.h"
#include "Event.h"
#include "PeriodicEvent.h"
#include "Addon.h"


Addon::Addon() {
	Addon::LoadEvents();
}

Addon::~Addon() {
	// Clean events
	for (auto& kv : events) {
		delete kv.second;
	}
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

void Addon::LoadEvents() {
	Event* exampleEvent = new Event("Example Event", 46545.0f, 21450.0f, "base", "FF0000");
	Event* exampleEvent2 = new Event("Example Event 2", 57600.0f, 21592.0f, "base", "FF0000");

	PeriodicEvent* periodicEvent3;
	{
		periodicEvent3 = new PeriodicEvent(
			"Auric Basin", // name
			34303.0f, // x
			33915.0f, // y
			0,  // midnight offset
			7200, // periodicity
			"FF0000" // color               
		);
		periodicEvent3->AddPeriodicEntry(
			"Pylons",
			"Pylons",
			0,
			2700,
			"414A02"
		);
		periodicEvent3->AddPeriodicEntry(
			"Pylons",
			"Pylons",
			0,
			2700,
			"414A02"
		);
		periodicEvent3->AddPeriodicEntry(
			"Challenges",
			"Challenges",
			2700,
			900,
			"545E0D"
		);
		periodicEvent3->AddPeriodicEntry(
			"Octovine",
			"Octovine",
			3600,
			1200,
			"667118"
		);
		periodicEvent3->AddPeriodicEntry(
			"Reset",
			"Reset",
			4800,
			600,
			"545E0D"
		);
		periodicEvent3->AddPeriodicEntry(
			"Pylons",
			"Pylons",
			5400,
			1800,
			"414A02"
		);
	}
	
	PeriodicEvent* periodicEvent4;
	{
		periodicEvent4 = new PeriodicEvent(
			"Tangled depths", // name
			37010.0f, // x
			35040.0f, // y
			0,  // midnight offset
			7200, // periodicity
			"FF0000" // color               
		);
		periodicEvent4->AddPeriodicEntry(
			"Outposts",
			"Outposts",
			0,
			1500,
			"414a02"
		);
		periodicEvent4->AddPeriodicEntry(
			"Pre",
			"Preparations",
			1500,
			300,
			"545e0d"
		);
		periodicEvent4->AddPeriodicEntry(
			"Chak Gerent",
			"Chak Gerent",
			1800,
			1200,
			"667118"
		);
		periodicEvent4->AddPeriodicEntry(
			"Outposts",
			"Outposts",
			3000,
			4200,
			"414a02"
		);
		periodicEvent4->SetEventType("periodic_timer");
	}


	Addon::AddEvent(exampleEvent);
	Addon::AddEvent(exampleEvent2);
	Addon::AddEvent(periodicEvent3);
	Addon::AddEvent(periodicEvent4);

}

void Addon::RenderEvents() {
	if (!MumbleLink->Context.IsMapOpen || !NexusLink->IsGameplay) return;
	
	for (const auto& kvp : events) {
		const std::string& eventName = kvp.first;
		Event* eventPtr = kvp.second;

		if (eventPtr) {
			if (eventPtr->GetEventType() == "base") {
				render_base_event(*eventPtr);
			}
			else if (eventPtr->GetEventType() == "periodic" || eventPtr->GetEventType() == "periodic_timer") {
				PeriodicEvent* periodicEventPtr = static_cast<PeriodicEvent*>(eventPtr);
				if (periodicEventPtr) {
					if (eventPtr->GetEventType() == "periodic") {
						render_periodic_event(*periodicEventPtr);
					}
					if (eventPtr->GetEventType() == "periodic_timer") {
						render_periodic_circular_event(*periodicEventPtr);
					}
				}
			}
		}
	}
}

void Addon::Update() {
	Event* exampleEvent = Addon::GetEvent("Example Event");
	Event* exampleEvent2 = Addon::GetEvent("Example Event 2");

	float piePercentage = exampleEvent->GetPercentage();
	piePercentage = piePercentage + 0.1f;
	if (piePercentage > 100.0f) {
		piePercentage = 0.0f;
	}
	exampleEvent->SetPercentage(piePercentage);


	float piePercentage2 = exampleEvent2->GetPercentage();
	piePercentage2 = piePercentage2 + 0.2f;
	if (piePercentage2 > 100.0f) {
		piePercentage2 = 0.0f;
	}
	exampleEvent2->SetPercentage(piePercentage2);
}

Event* Addon::GetEvent(const std::string& eventName) {
	auto it = events.find(eventName);
	if (it != events.end()) {
		return it->second;
	}
	return nullptr; // Event not found
}

void Addon::AddEvent(Event* eventInstance) {
	events[eventInstance->GetName().c_str()] = eventInstance;
}