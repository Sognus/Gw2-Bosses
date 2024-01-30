#include <filesystem>
#include "Globals.h"
#include "utils/Render.h"
#include "utils/Map.h"
#include "utils/BoundingBox.h"
#include "Event.h"
#include "PeriodicEvent.h"
#include "Addon.h"

namespace fs = std::filesystem;

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

	if (ImGui::Begin("GW2_BOSSES", (bool*)0, 
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize)) {
		#ifdef _DEBUG
			render_debug_crosshair();
		#endif
		this->RenderEvents();
		this->Update();
	}
	ImGui::End();
}

void Addon::LoadEvents() {
	// Get addon directory
	std::string path = APIDefs->GetAddonDirectory(AddonDef->Name);

	if (!fs::exists(path)) {
		try {
			fs::create_directory(path);
		}
		catch (const std::exception& e) {
			std::string message = "Could not create addon directory: ";
			message.append(path);
			APIDefs->Log(ELogLevel::ELogLevel_CRITICAL, message.c_str());
			return;
		}
	}

	// Hardcoded events because I don't have mental strength to continue

	// Dry top block
	PeriodicEvent* dry_top;
	{
		dry_top = new PeriodicEvent(
			"Dry top",
			37129.0f,
			32802.0f,
			0,
			7200,
			"4A3B02"
		);

		dry_top->AddPeriodicEntry(
			"Crash Site",
			"Crash Site",
			0,
			2400,
			3600,
			"4A3B02"
		);
		dry_top->AddPeriodicEntry(
			"Sandstorm",
			"Sandstorm",
			2400,
			1200,
			3600,
			"715F18"
		);
		dry_top->AddPeriodicEntry(
			"Crash Site",
			"Crash Site",
			3600,
			2400,
			3600,
			"4A3B02"
		);
		dry_top->AddPeriodicEntry(
			"Sandstorm",
			"Sandstorm",
			6000, // offset from midnight
			1200, // duration
			3600, // Offset next 
			"715F18"
		);

		dry_top->SetEventType("periodic_timer");

	}

	// Verdant brink block
	PeriodicEvent* verdant_brink;
	{
		verdant_brink = new PeriodicEvent(
			"Verdant Brink",
			35049.0f,
			31779.0f,
			0,
			7200,
			"545E0D"
		);

		verdant_brink->AddPeriodicEntry(
			"Night",
			"Night",
			0,
			600,
			"545E0D"
		);
		verdant_brink->AddPeriodicEntry(
			"Bosses",
			"Bosses",
			600,
			1200,
			"667118"
		);
		verdant_brink->AddPeriodicEntry(
			"Day",
			"Day",
			1800,
			4500,
			"414A02"
		);
		verdant_brink->AddPeriodicEntry(
			"Night",
			"Night",
			6300,
			1800,
			"545E0D"
		);

		verdant_brink->SetEventType("periodic_timer");
	}

	// Auric Basin Block
	PeriodicEvent* auric_basin;
	{
		auric_basin = new PeriodicEvent(
			"Auric Basin", // name
			34303.0f, // x
			33915.0f, // y
			0,  // midnight offset
			7200, // periodicity
			"FF0000" // color               
		);
		auric_basin->AddPeriodicEntry(
			"Pylons",
			"Pylons",
			0,
			2700,
			"414A02"
		);
		auric_basin->AddPeriodicEntry(
			"Pylons",
			"Pylons",
			0,
			2700,
			"414A02"
		);
		auric_basin->AddPeriodicEntry(
			"Challenges",
			"Challenges",
			2700,
			900,
			"545E0D"
		);
		auric_basin->AddPeriodicEntry(
			"Octovine",
			"Octovine",
			3600,
			1200,
			"667118"
		);
		auric_basin->AddPeriodicEntry(
			"Reset",
			"Reset",
			4800,
			600,
			"545E0D"
		);
		auric_basin->AddPeriodicEntry(
			"Pylons",
			"Pylons",
			5400,
			1800,
			"414A02"
		);

		auric_basin->SetEventType("periodic_timer");
	}

	// Tangled Depths Block
	PeriodicEvent* tangled_depths;
	{
		tangled_depths = new PeriodicEvent(
			"Tangled depths", // name
			37010.0f, // x
			35040.0f, // y
			0,  // midnight offset
			7200, // periodicity
			"FF0000" // color               
		);
		tangled_depths->AddPeriodicEntry(
			"Outposts",
			"Outposts",
			0,
			1500,
			"414a02"
		);
		tangled_depths->AddPeriodicEntry(
			"Pre",
			"Preparations",
			1500,
			300,
			"545e0d"
		);
		tangled_depths->AddPeriodicEntry(
			"Chak Gerent",
			"Chak Gerent",
			1800,
			1200,
			"667118"
		);
		tangled_depths->AddPeriodicEntry(
			"Outposts",
			"Outposts",
			3000,
			4200,
			"414a02"
		);
		tangled_depths->SetEventType("periodic_timer");
	}

	// Dragon Stand block
	PeriodicEvent* dragons_stand;
	{
		dragons_stand = new PeriodicEvent(
			"Dragon's stand", // name
			35709.0f, // x
			37182.0f, // y
			0,  // midnight offset
			7200, // periodicity
			"FF0000" // color               
		);
		dragons_stand->AddPeriodicEntry(
			"Metaevent progress",
			"Metaevent progress",
			0,
			5400,
			"667118"
		);
		dragons_stand->AddPeriodicEntry(
			"Metaevent start",
			"Metaevent start",
			5400,
			1800,
			"C7E400"
		);

		dragons_stand->SetEventType("periodic_timer");
	}

	PeriodicEvent* lake_doric;
	{
		lake_doric = new PeriodicEvent(
			"Lake Doric", // name
			45564.0f, // x
			27004.0f, // y
			0,  // midnight offset
			7200, // periodicity
			"FF0000" // color               
		);
		lake_doric->AddPeriodicEntry(
			"New Loamhurst",
			"New Loamhurst",
			0,
			1800,
			"4B7F40"
		);
		lake_doric->AddPeriodicEntry(
			"Noran's Homestead",
			"Noran's Homestead",
			1800,
			1800,
			"52763B"
		);
		lake_doric->AddPeriodicEntry(
			"Saidra's Haven",
			"Noran's Homestead",
			3600,
			2700,
			"34561E"
		);
		lake_doric->AddPeriodicEntry(
			"New Loamhurst",
			"New Loamhurst",
			6300,
			900,
			"4B7F40"
		);

		lake_doric->SetEventType("periodic_timer");
	}

	// Crystal oasis block
	PeriodicEvent* crystal_oasis;
	{
		crystal_oasis = new PeriodicEvent(
			"Crystal Oasis",
			58692.0f,
			43752.0f,
			0,
			7200,
			"FF0000"
		);

		crystal_oasis->AddPeriodicEntry(
			"Rest",
			"Rest",
			0,
			300,
			"623903"
		);

		crystal_oasis->AddPeriodicEntry(
			"Casino: Rounds 1-3",
			"Casino: Rounds 1-3",
			300,
			960,
			"7E4E11"
		);

		crystal_oasis->AddPeriodicEntry(
			"Casino: Pinata",
			"Casino: Pinata",
			1260,
			540,
			"976320"
		);

		crystal_oasis->AddPeriodicEntry(
			"Rest",
			"Rest",
			1800,
			5400,
			"623903"
		);

		crystal_oasis->SetEventType("periodic_timer");
	}

	// Crystal oasis block
	PeriodicEvent* desert_highlands;
	{
		desert_highlands = new PeriodicEvent(
			"Desert Highlands",
			59964.0f,
			41384.0f,
			0,
			7200,
			"FF0000"
		);

		desert_highlands->AddPeriodicEntry(
			"Rest",
			"Rest",
			0,
			3600,
			"623903"
		);

		desert_highlands->AddPeriodicEntry(
			"Buried Treasure",
			"Buried Treasure",
			3600,
			1200,
			"7E4E11"
		);

		desert_highlands->AddPeriodicEntry(
			"Rest",
			"Rest",
			4800,
			2400,
			"623903"
		);

		desert_highlands->SetEventType("periodic_timer");
	}

	// Elon Riverlands block
	PeriodicEvent* elon_riverlands;
	{
		elon_riverlands = new PeriodicEvent(
			"Elon Riverlands",
			60715.0f,
			45646.0f,
			0,
			7200,
			"FF0000"
		);

		elon_riverlands->AddPeriodicEntry(
			"Doppelganger",
			"Doppelganger ",
			0,
			900,
			"976320"
		);

		elon_riverlands->AddPeriodicEntry(
			"Rest",
			"Rest",
			900,
			4500,
			"623903"
		);

		elon_riverlands->AddPeriodicEntry(
			"Augury Rock",
			"Augury Rock",
			5400,
			1500,
			"7E4E11"
		);

		elon_riverlands->AddPeriodicEntry(
			"Doppelganger",
			"Doppelganger",
			6900,
			300,
			"976320"
		);

		elon_riverlands->SetEventType("periodic_timer");
	}


	// The Desolation block
	PeriodicEvent* the_desolation;
	{
		the_desolation = new PeriodicEvent(
			"The Desolation",
			59943.0f,
			50257.0f,
			0,
			7200,
			"FF0000"
		);

		the_desolation->AddPeriodicEntry(
			"Rest",
			"Rest",
			0,
			1800,
			"623903"
		);

		the_desolation->AddPeriodicEntry(
			"Junundu Rising",
			"Junundu Rising",
			1800,
			1200,
			"623903"
		);

		the_desolation->AddPeriodicEntry(
			"Rest",
			"Rest",
			3000,
			600,
			"623903"
		);

		the_desolation->AddPeriodicEntry(
			"Maws of Torment",
			"Maws of Torment",
			3600,
			1200,
			"976320"
		);

		the_desolation->AddPeriodicEntry(
			"Rest",
			"Rest",
			4800,
			600,
			"623903"
		);

		the_desolation->AddPeriodicEntry(
			"Junundu Rising",
			"Junundu Rising",
			5400,
			1200,
			"623903"
		);

		the_desolation->AddPeriodicEntry(
			"Rest",
			"Rest",
			6600,
			600,
			"623903"
		);

		the_desolation->SetEventType("periodic_timer");
	}

	// Domain of Vabbi block
	PeriodicEvent* domain_of_vabbi;
	{
		domain_of_vabbi = new PeriodicEvent(
			"Domain of Vabbi",
			66332.0f,
			53596.0f,
			0,
			7200,
			"FF0000"
		);

		domain_of_vabbi->AddPeriodicEntry(
			"Forged With Fire",
			"Forged With Fire",
			0,
			1800,
			"976320"
		);

		domain_of_vabbi->AddPeriodicEntry(
			"Serpent's Ire",
			"Serpent's Ire",
			1800,
			1200,
			"FF0000"
		);

		domain_of_vabbi->AddPeriodicEntry(
			"Forged With Fire",
			"Forged With Fire",
			3000,
			1800,
			"7E4E11"
		);

		domain_of_vabbi->AddPeriodicEntry(
			"Rest",
			"Rest",
			4800,
			2400,
			"623903"
		);

		domain_of_vabbi->SetEventType("periodic_timer");
	}

	// Domain of Istan block
	PeriodicEvent* domain_of_istan;
	{
		domain_of_istan = new PeriodicEvent(
			"Domain of Istan",
			57165.0f,
			62605.0f,
			0,
			7200,
			"FF0000"
		);

		domain_of_istan->AddPeriodicEntry(
			"Palawadan",
			"Palawadan",
			0,
			900,
			"7E3494"
		);

		domain_of_istan->AddPeriodicEntry(
			"Rest",
			"Rest",
			900,
			5400,
			"5D266D"
		);

		domain_of_istan->AddPeriodicEntry(
			"Palawadan",
			"Palawadan",
			6300,
			900,
			"7E3494"
		);

		domain_of_istan->SetEventType("periodic_timer");
	}

	// Jahai Bluffs block
	PeriodicEvent* jahai_bluffs;
	{
		jahai_bluffs = new PeriodicEvent(
			"Jahai Bluffs",
			65135.0f,
			57421.0f,
			0,
			7200,
			"FF0000"
		);

		jahai_bluffs->AddPeriodicEntry(
			"Rest",
			"Rest",
			0,
			3600,
			"5D266D"
		);

		jahai_bluffs->AddPeriodicEntry(
			"Escorts",
			"Escorts",
			3600,
			900,
			"6C2D7F"
		);

		jahai_bluffs->AddPeriodicEntry(
			"Death-Branded Shatterer",
			"Death-Branded Shatterer",
			4500,
			900,
			"7E3494"
		);

		jahai_bluffs->AddPeriodicEntry(
			"Rest",
			"Rest",
			5400,
			1800,
			"5D266D"
		);

		jahai_bluffs->SetEventType("periodic_timer");
	}


	// Thunderhead Peaks block
	PeriodicEvent* thunderhead_peaks;
	{
		thunderhead_peaks = new PeriodicEvent(
			"Thunderhead Peaks",
			57950.0f,
			37800.0f,
			0,
			7200,
			"FF0000"
		);

		thunderhead_peaks->AddPeriodicEntry(
			"Thunderhead Keep",
			"Thunderhead Keep",
			0,
			300,
			"7E3494"
		);

		thunderhead_peaks->AddPeriodicEntry(
			"Rest",
			"Rest",
			300,
			2400,
			"5D266D"
		);

		thunderhead_peaks->AddPeriodicEntry(
			"The Oil Floes",
			"The Oil Floes",
			2700,
			900,
			"7E3494"
		);

		thunderhead_peaks->AddPeriodicEntry(
			"Rest",
			"Rest",
			3600,
			2700,
			"5D266D"
		);

		thunderhead_peaks->AddPeriodicEntry(
			"Thunderhead Keep",
			"Thunderhead Keep",
			6300,
			900,
			"7E3494"
		);

		thunderhead_peaks->SetEventType("periodic_timer");
	}

	// Grothmar Valley block
	PeriodicEvent* grothmar_valley;
	{
		grothmar_valley = new PeriodicEvent(
			"Grothmar Valley",
			60957.0f,
			19174.0f,
			0,
			7200,
			"FF0000"
		);

		grothmar_valley->AddPeriodicEntry(
			"Rest",
			"Rest",
			0,
			600,
			"033A62"
		);

		grothmar_valley->AddPeriodicEntry(
			"Effigy",
			"Effigy",
			600,
			900,
			"206697"
		);

		grothmar_valley->AddPeriodicEntry(
			"Rest",
			"Rest",
			1500,
			780,
			"033A62"
		);

		grothmar_valley->AddPeriodicEntry(
			"Doomlore shrine",
			"Doomlore shrine",
			2280,
			1320,
			"206697"
		);

		grothmar_valley->AddPeriodicEntry(
			"Rest",
			"Rest",
			3600,
			300,
			"033A62"
		);

		grothmar_valley->AddPeriodicEntry(
			"Ooze pits",
			"Ooze pits",
			3900,
			1200,
			"206697"
		);

		grothmar_valley->AddPeriodicEntry(
			"Rest",
			"Rest",
			5100,
			900,
			"033A62"
		);

		grothmar_valley->AddPeriodicEntry(
			"Metal concert",
			"Metal concert",
			6000,
			900,
			"206697"
		);

		grothmar_valley->AddPeriodicEntry(
			"Rest",
			"Rest",
			6900,
			300,
			"033A62"
		);

		grothmar_valley->SetEventType("periodic_timer");
	}

	// Bjora Marches block
	PeriodicEvent* bjora_marches;
	{
		bjora_marches = new PeriodicEvent(
			"Bjora Marches",
			57267.0f,
			18383.0f,
			0,
			7200,
			"FF0000"
		);

		bjora_marches->AddPeriodicEntry(
			"Shards and Construct",
			"Shards and Construct",
			0,
			300,
			"206697"
		);

		bjora_marches->AddPeriodicEntry(
			"Icebrood Champions",
			"Shards and Construct",
			300,
			900,
			"11507E"
		);

		bjora_marches->AddPeriodicEntry(
			"Rest",
			"Rest",
			1200,
			2700,
			"033A62"
		);


		bjora_marches->AddPeriodicEntry(
			"Drakkar",
			"Drakkar",
			3900,
			2100,
			"206697"
		);

		bjora_marches->AddPeriodicEntry(
			"Rest",
			"Rest",
			6000,
			300,
			"206697"
		);

		bjora_marches->AddPeriodicEntry(
			"Defend Jora's Keep",
			"Defend Jora's Keep",
			6300,
			900,
			"11507E"
		);


		bjora_marches->SetEventType("periodic_timer");
	}

	// Seitung Province block
	PeriodicEvent* seitung_province;
	{
		seitung_province = new PeriodicEvent(
			"Seitung Province",
			23247.0f,
			102143.0f,
			0,
			7200,
			"FF0000"
		);

		seitung_province->AddPeriodicEntry(
			"Rest",
			"Rest",
			0,
			5400,
			"09565E"
		);

		seitung_province->AddPeriodicEntry(
			"Aetherblade Assault",
			"Aetherblade Assault",
			5400,
			1800,
			"208B97"
		);

		seitung_province->SetEventType("periodic_timer");
	}

	// New Kaineng City block
	PeriodicEvent* new_kaineng_city;
	{
		new_kaineng_city = new PeriodicEvent(
			"New Kaineng City",
			27975.0f,
			99331.0f,
			0,
			7200,
			"FF0000"
		);

		new_kaineng_city->AddPeriodicEntry(
			"Kaineng Blackout",
			"Kaineng Blackout",
			0,
			2400,
			"208B97"
		);

		new_kaineng_city->AddPeriodicEntry(
			"Rest",
			"Rest",
			2400,
			4800,
			"09565E"
		);

		new_kaineng_city->SetEventType("periodic_timer");
	}

	// The Echovald Wilds block
	PeriodicEvent* the_echovald_wilds;
	{
		the_echovald_wilds = new PeriodicEvent(
			"The Echovald Wilds",
			31118.0f,
			102764.0f,
			0,
			7200,
			"FF0000"
		);

		the_echovald_wilds->AddPeriodicEntry(
			"Rest",
			"Rest",
			0,
			1800,
			"09565E"
		);

		the_echovald_wilds->AddPeriodicEntry(
			"Gang War",
			"Gang War",
			1800,
			2100,
			"208B97"
		);

		the_echovald_wilds->AddPeriodicEntry(
			"Rest",
			"Rest",
			3900,
			2100,
			"09565E"
		);

		the_echovald_wilds->AddPeriodicEntry(
			"Fort Aspenwood",
			"Fort Aspenwood",
			6000,
			1200,
			"10737F"
		);

		the_echovald_wilds->SetEventType("periodic_timer");
	}


	// Skywatch archipelago block
	PeriodicEvent* skywatch_archipelago;
	{
		skywatch_archipelago = new PeriodicEvent(
			"Skywatch archipelago",
			25433.0f,
			23305.0f,
			0,
			7200,
			"FF0000"
		);

		skywatch_archipelago->AddPeriodicEntry(
			"Rest",
			"Rest",
			0,
			3600,
			"A06608"
		);

		skywatch_archipelago->AddPeriodicEntry(
			"Unlocking The Wizard's Tower",
			"Unlocking The Wizard's Tower",
			3600,
			1500,
			"D19C3C"
		);

		skywatch_archipelago->AddPeriodicEntry(
			"Rest",
			"Rest",
			5100,
			2100,
			"A06608"
		);


		skywatch_archipelago->SetEventType("periodic_timer");
	}


	// Amnytas block
	PeriodicEvent* amnytas;
	{
		amnytas = new PeriodicEvent(
			"Amnytas",
			24087.0f,
			20402.0f,
			0,
			7200,
			"FF0000"
		);

		amnytas->AddPeriodicEntry(
			"Defense of Amnytas",
			"Defense of Amnytas",
			0,
			1500,
			"D19C3C"
		);

		amnytas->AddPeriodicEntry(
			"Rest",
			"Rest",
			1500,
			5700,
			"A06608"
		);


		amnytas->SetEventType("periodic_timer");
	}

	// Wizard's Tower block
	PeriodicEvent* wizard_tower;
	{
		wizard_tower = new PeriodicEvent(
			"Wizard's Tower",
			24098.0f,
			22715.0f,
			0,
			7200,
			"FF0000"
		);

		wizard_tower->AddPeriodicEntry(
			"Fly by Night",
			"Fly by Night",
			0,
			1200,
			"D19C3C"
		);

		wizard_tower->AddPeriodicEntry(
			"Rest",
			"Rest",
			1200,
			2400,
			"A06608"
		);

		wizard_tower->AddPeriodicEntry(
			"Target Practice",
			"Target Practice",
			3600,
			1800,
			"D19C3C"
		);


		wizard_tower->AddPeriodicEntry(
			"Convergences + Target Practice",
			"Convergences + Target Practice",
			5400,
			600,
			"FFD792"
		);

		wizard_tower->AddPeriodicEntry(
			"Target Practice + Fly by Night",
			"Target Practice + Fly by Night",
			6000,
			900,
			"E0B76F"
		);

		wizard_tower->AddPeriodicEntry(
			"Fly by Night",
			"Fly by Night",
			6900,
			300,
			"D19C3C"
		);


		wizard_tower->SetEventType("periodic_timer");
	}

	// Add events block
	{
		// TODO: Coremap bosses

		// HoT
		Addon::AddEvent(dry_top);
		Addon::AddEvent(verdant_brink);
		Addon::AddEvent(auric_basin);
		Addon::AddEvent(tangled_depths);
		Addon::AddEvent(dragons_stand);

		// LS S3
		Addon::AddEvent(lake_doric);

		// PoF
		Addon::AddEvent(crystal_oasis);
		Addon::AddEvent(desert_highlands);
		Addon::AddEvent(the_desolation);
		Addon::AddEvent(domain_of_vabbi);

		// LS S4
		// TODO: Awakened Invasion
		Addon::AddEvent(domain_of_istan);
		Addon::AddEvent(jahai_bluffs);
		Addon::AddEvent(thunderhead_peaks);

		// IBS
		Addon::AddEvent(grothmar_valley);
		Addon::AddEvent(bjora_marches);
		// TODO: Dragonstorm

		// EoD
		Addon::AddEvent(seitung_province);
		Addon::AddEvent(new_kaineng_city);
		Addon::AddEvent(the_echovald_wilds);

		// Soto
		Addon::AddEvent(skywatch_archipelago);
		Addon::AddEvent(amnytas);
		Addon::AddEvent(wizard_tower);
	}

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