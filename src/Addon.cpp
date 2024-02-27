#include "Addon.h"

namespace fs = std::filesystem;

Addon::Addon() {
	worldBossesNotifications = new CyclicalCoreWorldbossEventQueue();
	Addon::LoadEvents();

	// Default behaviour 
	render = true;
	showNotifications = true;
}

Addon::~Addon() {
	// Clean events
	APIDefs->Log(ELogLevel::ELogLevel_DEBUG, "Bosses addon destructor called");
	for (auto& kv : events) {
		delete kv.second;
	}
	delete worldBossesNotifications;
}


void Addon::Render() {
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);

	// Render events
	if (ImGui::Begin("GW2_BOSSES_EVENTS", (bool*)0, 
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize)) {
		#ifdef _DEBUG
			render_debug_crosshair();
		#endif
		this->RenderEvents();
	}
	ImGui::End();

	// Render notifications
	this->RenderNotifications();


	this->Update();
}

void Addon::LoadEvents() {
	// Get addon directory
	std::string pathFolder = APIDefs->GetAddonDirectory(AddonDef->Name);

	// Create folder if not exist
	if (!fs::exists(pathFolder)) {
		try {
			fs::create_directory(pathFolder);
		}
		catch (const std::exception& e) {
			std::string message = "Could not create addon directory: ";
			message.append(pathFolder);
			APIDefs->Log(ELogLevel::ELogLevel_CRITICAL, message.c_str());

			// Suppress the warning for the unused variable 'e'
			#pragma warning(suppress: 4101)
			e;
		}
	}

	// Load flags
	boolean eventsLoaded = false;

	const char* dataFilename = "data.json";
	std::string pathData = pathFolder + preferred_separator_char() + dataFilename;

	// Load events from data.json
	if (fs::exists(pathData)) {
		std::ifstream dataFile(pathData);

		if (dataFile.is_open()) {
			json jsonData;
			dataFile >> jsonData;

			json jsonDataEvents = jsonData["events"];

			for (auto& [key, jsonEvent] : jsonDataEvents.items()) {
				// Skip event when type is not defined
				if (jsonEvent.find("event_type") == jsonEvent.end()) {
					continue;
				}
				
				// Handle events
				if (jsonEvent["event_type"].get<std::string>().starts_with("periodic_timer")) {
					PeriodicEvent* periodicEventObject = PeriodicEvent::CreateFromJson(jsonEvent);
					this->AddEvent(periodicEventObject);
				}

				// Handle core world bosses
				if (jsonEvent["event_type"].get<std::string>().starts_with("core_world_bosses")) {
					CoreWorldbossEvent* coreWorldBossEvent = CoreWorldbossEvent::CreateFromJson(jsonEvent);
					this->AddCoreWorldbossEvent(coreWorldBossEvent);
					this->worldBossesNotifications->push(coreWorldBossEvent);
				}

			}

			eventsLoaded = true;
		}

	}

	// Load events via fallback if loaded flag was not set to positive value
	if (!eventsLoaded) {
		Addon::LoadEventsFallback();
		Addon::LoadCoreWorldbossesFallback();
		Addon::ExportEventsJson();
	}
}

void Addon::RenderEvents() {
	// Do not render when render is disabled
	if (!render) return;
	// Do not render when user is not looking at map
	if (!MumbleLink->Context.IsMapOpen || !NexusLink->IsGameplay) return;

	// Render events
	for (const auto& kvp : events) {
		const std::string& eventName = kvp.first;
		Event* eventPtr = kvp.second;

		if (eventPtr) {

			if (eventPtr->GetEventType().compare("core_world_bosses") == 0) {
				continue;
			}
			if (eventPtr->GetEventType().compare("base") == 0) {
				render_base_event(*eventPtr);
			}
			else if 
			(
				eventPtr->GetEventType().compare("periodic") == 0 ||
				eventPtr->GetEventType().compare("periodic_timer") == 0 ||
				eventPtr->GetEventType().compare("periodic_timer_convergences") == 0
			){
				PeriodicEvent* periodicEventPtr = static_cast<PeriodicEvent*>(eventPtr);
				if (periodicEventPtr) {
					if (eventPtr->GetEventType().compare("periodic") == 0) {
						render_periodic_event(*periodicEventPtr);
						continue;
					}
					if (eventPtr->GetEventType().compare("periodic_timer") == 0) {
						render_periodic_circular_event(*periodicEventPtr);
						continue;
					}
					if (eventPtr->GetEventType().compare("periodic_timer_convergences") == 0) {
						render_periodic_circular_event_convergences(*periodicEventPtr);
						continue;
					}
				}
			}
		}
	}
}

void Addon::RenderNotifications() {
	if (!this->showNotifications) return;
	// Dont show if not gameplay
	if(!NexusLink->IsGameplay) return;

	if (ImGui::Begin("GW2 BOSSES: Notifications", (bool*)0, 0)) {
		// TODO: show more than 1 boss
		// idea is to have some kind of constant that will multiply 15 minute like start - (notifyOffset * CONSTANT)
		// then here in render it should do stuff like divide notifyOffset by 15 minutes and sort it to different boxes

		// TODO:
		// In render there must be InProgress and in Upcoming flag set 
		// Set it by angle of current being in angle of pie part between startAngle and EndAngle
		// Different pie parts will need flags if they should trigger 

		// TODO: Add localized time after ImGui::Text(name.c_str());
		// TODO: With sorting add list of active metaevents to some other box InProgress and Upcoming

		if (ImGui::CollapsingHeader("Upcoming core world bosses (next 15 minutes)", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (Event* notificationEvent : notificationBoxUpcoming) {
				std::string eventName = notificationEvent->GetName();
				size_t spacePos = eventName.find_last_of(" ");
				std::string name;
				
				if (spacePos != std::string::npos) {
					name = eventName.substr(0, spacePos);
				}
				else {
					name = eventName;
				}

				ImGui::Text(name.c_str());
				ImGui::Separator();
			}
		}
		if (ImGui::CollapsingHeader("Core world bosses in progress", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (Event* notificationEvent : notificationBoxInProgress) {
				std::string eventName = notificationEvent->GetName();
				size_t spacePos = eventName.find_last_of(" ");
				std::string name;

				if (spacePos != std::string::npos) {
					name = eventName.substr(0, spacePos);
				}
				else {
					name = eventName;
				}

				ImGui::Text(name.c_str());
				ImGui::Separator();
			}
		}
	}
	ImGui::End();
}

void Addon::Update() {
	long current_time = get_time_since_midnight();

	// Get events into upcoming notification 
	while (true) {
		CoreWorldbossEvent* coreEvent = worldBossesNotifications->peek();

		// Queue empty - skipping
		if (coreEvent == nullptr) {
			break;
		}

		// Get notification start
		long eventStart = coreEvent->GetMidnightOffsetSeconds();
		long notificationUnclamped = eventStart - coreEvent->GetNotifyOffsetSeconds();
		long notificationStart = std::clamp(notificationUnclamped, TIME_CLAMP_MIDNIGHT_LOWER, TIME_CLAMP_MIDNIGHT_UPPER);
		long eventEndUnclamped = eventStart + coreEvent->GetDurationSeconds();
		long eventEnd = std::clamp(eventEndUnclamped, TIME_CLAMP_MIDNIGHT_LOWER, TIME_CLAMP_MIDNIGHT_UPPER);

		// Add to upcoming
		if (current_time >= notificationStart && current_time < eventStart) {
			this->notificationBoxUpcoming.push_back(coreEvent);
			// Event was handled pop queue
			worldBossesNotifications->pop();
		}
		// Add to in progress
		else if (current_time >= eventStart && current_time <= eventEnd) {
			this->notificationBoxInProgress.push_back(coreEvent);
			worldBossesNotifications->pop();
		}
		// Go through all past events - usually only for initialization/first update
		if (current_time > eventEnd) {
			worldBossesNotifications->pop();
		}
		else {
			// Current event is not yet upcomming, because queue is sorted, skip 
			break;
		}
	}

	// Move events from upcomming to in progress
	for (auto it = this->notificationBoxUpcoming.begin(); it != this->notificationBoxUpcoming.end(); /* no increment */) {
		Event* eventPtr = *it;

		// Handle Core event
		if (eventPtr->GetEventType().starts_with("core_world_bosses")) {
			CoreWorldbossEvent* coreEvent = static_cast<CoreWorldbossEvent*>(eventPtr);
			long eventStart = coreEvent->GetMidnightOffsetSeconds();
			long notificationUnclamped = eventStart - coreEvent->GetNotifyOffsetSeconds();
			long notificationStart = std::clamp(notificationUnclamped, TIME_CLAMP_MIDNIGHT_LOWER, TIME_CLAMP_MIDNIGHT_UPPER);
			long eventEndUnclamped = eventStart + coreEvent->GetDurationSeconds();
			long eventEnd = std::clamp(eventEndUnclamped, TIME_CLAMP_MIDNIGHT_LOWER, TIME_CLAMP_MIDNIGHT_UPPER);
			
			// Event is still upcoming - skipping
			if (current_time >= notificationStart && current_time < eventStart) {
				++it;
				continue;
			}

			// Event is between start and end - push erase from upcoming and move to in progress
			if (current_time >= eventStart && current_time <= eventEnd) {
				// Move to in progress
				this->notificationBoxInProgress.push_back(eventPtr);
				// Erase and move to next iterator
				it = this->notificationBoxUpcoming.erase(it);
				continue;
			}

			// Just in case
			if (current_time > eventEnd) {
				it = this->notificationBoxUpcoming.erase(it);
				continue;
			}
		}

		// Handle periodic event
		if(
			eventPtr->GetEventType().compare("periodic") == 0 ||
			eventPtr->GetEventType().compare("periodic_timer") == 0 ||
			eventPtr->GetEventType().compare("periodic_timer_convergences") == 0
		){
			PeriodicEvent* periodicEventPtr = static_cast<PeriodicEvent*>(eventPtr);

			if (periodicEventPtr->GetNotifyUpcoming() == false) {
				it = this->notificationBoxUpcoming.erase(it);
				continue;
			}
		}

		// Move to next element
		++it;
	}

	// Remove events from in progress
	for (auto it = this->notificationBoxInProgress.begin(); it != this->notificationBoxInProgress.end(); /* no increment */) {
		Event* eventPtr = *it;

		// Handle Core event
		if (eventPtr->GetEventType().starts_with("core_world_bosses")) {
			CoreWorldbossEvent* coreEvent = static_cast<CoreWorldbossEvent*>(eventPtr);
			long eventStart = coreEvent->GetMidnightOffsetSeconds();
			long eventEndUnclamped = eventStart + coreEvent->GetDurationSeconds();
			long eventEnd = std::clamp(eventEndUnclamped, TIME_CLAMP_MIDNIGHT_LOWER, TIME_CLAMP_MIDNIGHT_UPPER);

			// Remove from in progress if current time is past eventEnd
			if (current_time > eventEnd) {
				it = this->notificationBoxInProgress.erase(it);
				continue;
			}
		}

		// Handle periodic event
		if (
			eventPtr->GetEventType().compare("periodic") == 0 ||
			eventPtr->GetEventType().compare("periodic_timer") == 0 ||
			eventPtr->GetEventType().compare("periodic_timer_convergences") == 0
			) {
			PeriodicEvent* periodicEventPtr = static_cast<PeriodicEvent*>(eventPtr);

			if (periodicEventPtr->GetNotifyInProgress() == false) {
				it = this->notificationBoxInProgress.erase(it);
				continue;
			}
		}

		// Move to next element
		++it;
	}


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

void Addon::AddCoreWorldbossEvent(Event* eventInstance) {
	events[eventInstance->GetName().c_str()] = eventInstance;
}


void Addon::ExportEventsJson() {
	// Get addon directory
	std::string path = APIDefs->GetAddonDirectory(AddonDef->Name);
	std::string jsonDataPath = path + "/data.json";
	std::ofstream outputFile(jsonDataPath);

	if (outputFile.is_open()) {
		json eventsWrapper;
		for (const auto& kvp : events) {
			const std::string& eventName = kvp.first;
			IJsonHandled* eventPtr = kvp.second;

			json eventData = eventPtr->ToJson();
			eventsWrapper[eventName] = eventData;
		}

		json finalJson;
		finalJson["events"] = eventsWrapper;
		outputFile << finalJson.dump(4) << std::endl;

		outputFile.close();
	}
	else {
		std::string message = "Unable to open file " + jsonDataPath + " for saving events";
		APIDefs->Log(ELogLevel::ELogLevel_CRITICAL, message.c_str());
	}
}

void Addon::LoadCoreWorldbossesFallback() {
	// WorldBosses
	{
		worldBossesNotifications->SetEditMode(true);

		// Taidha
		{
			float x = 48872.0f;
			float y = 33548.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 0;
			int duration = 900;

			int timesPerDay = 8;
			int offset = 10800;

			std::string baseName = "Admiral Taidha Covington";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}


		// Svanir Shaman
		{
			float x = 48872.0f;
			float y = 33548.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 900;
			int duration = 900;

			int timesPerDay = 12;
			int offset = 7200;

			std::string baseName = "Svanir Shaman Chief";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}

		// Svanir Shaman
		{
			float x = 56071.0f;
			float y = 29379.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 900;
			int duration = 900;

			int timesPerDay = 12;
			int offset = 7200;

			std::string baseName = "Svanir Shaman Chief";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}

		// Megadestroyer
		{
			float x = 51939.0f;
			float y = 39395.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 1800;
			int duration = 900;

			int timesPerDay = 8;
			int offset = 10800;

			std::string baseName = "Megadestroyer";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}


		// Fire elemental
		{
			float x = 40346.0f;
			float y = 33755.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 2700;
			int duration = 900;

			int timesPerDay = 12;
			int offset = 7200;

			std::string baseName = "Fire Elemental";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}


		// Shatterer
		{
			float x = 62512.0f;
			float y = 29023.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 3600;
			int duration = 900;

			int timesPerDay = 8;
			int offset = 10800;

			std::string baseName = "Shatterer";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}


		// Great Jungle Wurm
		{
			float x = 42365.0f;
			float y = 33145.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 4500;
			int duration = 900;

			int timesPerDay = 12;
			int offset = 7200;

			std::string baseName = "Great Jungle Wurm";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}

		// Modniir Ulgoth
		{
			float x = 49079.0f;
			float y = 26174.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 5400;
			int duration = 900;

			int timesPerDay = 8;
			int offset = 10800;

			std::string baseName = "Modniir Ulgoth";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}


		// Shadow Behemoth
		{
			float x = 44837.0f;
			float y = 29997.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 6300;
			int duration = 900;

			int timesPerDay = 12;
			int offset = 7200;

			std::string baseName = "Shadow Behemoth";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}


		// Golem Mark II
		{
			float x = 53954.0f;
			float y = 38916.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 7200;
			int duration = 900;

			int timesPerDay = 8;
			int offset = 10800;

			std::string baseName = "Golem Mark II";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}


		// Claw of Jormag
		{
			float x = 56032.0f;
			float y = 25417.0f;
			int notifyOffsetSeconds = 900;
			int midnightOffsetSeconds = 9000;
			int duration = 900;

			int timesPerDay = 8;
			int offset = 10800;

			std::string baseName = "Claw of Jormag";

			for (int i = 0; i < timesPerDay; i++) {

				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					midnightOffsetSeconds + (i * offset),
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
			}
		}


		// Tequatl
		{
			float x = 48412.0f;
			float y = 38488.0f;
			int notifyOffsetSeconds = 900;
			//int midnightOffsetSeconds = 0;
			int duration = 900;

			int timesPerDay = 6;
			int offset = 10800;

			std::string baseName = "Tequatl";

			int i = 0;

			// 0
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					0, // MIDNIGHT UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 1
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					10800,// 3 AM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 2
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					25200, // 7AM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 3
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					41400, // 11:30AM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 4
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					57600, // 4PM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 5
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					68400, // 7PM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}
		}


		// Karka Queen
		{
			float x = 46346.0f;
			float y = 35978.0f;
			int notifyOffsetSeconds = 900;
			//int midnightOffsetSeconds = 9000;
			int duration = 900;

			int timesPerDay = 6;
			int offset = 10800;

			std::string baseName = "Karka Queen";

			int i = 0;

			// 0
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					7200, // 2AM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 1
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					21600,// 6 AM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 2
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					37800, // 10:30 UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 3
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					54000, // 3PM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 4
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					64800, // 6PM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 5
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					82800, // 11PM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}
		}


		// Evolved Jungle Wurm
		{
			float x = 49480.0f;
			float y = 34069.0f;
			int notifyOffsetSeconds = 900;
			//int midnightOffsetSeconds = 9000;
			int duration = 900;

			int timesPerDay = 6;
			int offset = 10800;

			std::string baseName = "Evolved Jungle Wurm";

			int i = 0;

			// 0
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					3600, // 1AM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 1
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					14400,// 4 AM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 2
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					28800, // 8AM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 3
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					45000, // 12:30PM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 4
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					61200, // 5PM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}

			// 5
			{
				CoreWorldbossEvent* ev = new  CoreWorldbossEvent(
					baseName + " " + std::to_string(i + 1),
					x,
					y,
					notifyOffsetSeconds,
					72000, // 8PM UTC
					duration,
					"000000"
				);

				Addon::AddCoreWorldbossEvent(ev);
				i++;
			}
		}


		// Disable edit mode and sort
		worldBossesNotifications->SetEditMode(false);
	}
}

void Addon::LoadEventsFallback() {


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
			"7E4E11"
		);

		domain_of_vabbi->AddPeriodicEntry(
			"Forged With Fire",
			"Forged With Fire",
			3000,
			1800,
			"976320"
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
			24668.0f,
			22698.0f,
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
			"Target Practice",
			"Target Practice",
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

	PeriodicEvent* convergences;
	{
		convergences = new PeriodicEvent(
			"Convergences",
			23503.0f,
			22698.0f,
			0,
			7200,
			"A06608"
		);

		convergences->AddPeriodicEntryDay(
			"Convergences",
			"Convergences",
			5400,
			600,
			"FFD792",
			10800 // Override of 2h periode into 3h
		);

		convergences->SetEventType("periodic_timer_convergences");
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
		Addon::AddEvent(convergences);
	}
}