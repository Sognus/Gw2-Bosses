#include "PeriodicEvent.h"


PeriodicEvent::PeriodicEvent() {

}

PeriodicEvent::PeriodicEvent(
    std::string name,
    ImVec2 location,
    int midnightOffsetSeconds,
    int periodicitySeconds,
    std::string hexColor
) : Event(name, location, "periodic", hexColor), periodicity_seconds(periodicitySeconds), midnight_offset_seconds(midnightOffsetSeconds) {
    // Init
}

PeriodicEvent::PeriodicEvent(
    std::string name,
    float x,
    float y,
    int midnightOffsetSeconds,
    int periodicitySeconds,
    std::string hexColor
) : Event(name, x, y, "periodic", hexColor), periodicity_seconds(periodicitySeconds), midnight_offset_seconds(midnightOffsetSeconds) {
    // Init
}




void PeriodicEvent::AddPeriodicEntryGeneric(
    std::string entryName,
    std::string description,
    int offsetSeconds,
    int durationSeconds,
    std::string entryColorHex,
    std::string periodicityType, 
    int periodicity_override
) {
    json entry;
    entry["name"] = entryName;
    entry["description"] = description;
    entry["offset_seconds"] = offsetSeconds;
    entry["duration_seconds"] = durationSeconds;
    entry["color_hex"] = entryColorHex;
    entry["offset_next"] = 7200;
    entry["periodicity_type"] = periodicityType; // Periode is bound inside period
    entry["periocitity_override"] = periodicity_override;
    periodic_entries.push_back(entry);
}






void PeriodicEvent::AddPeriodicEntryDay(
    std::string entryName,
    std::string description,
    int offsetSeconds,
    int durationSeconds,
    int offsetNext,
    std::string entryColorHex,
    int periodicity_override
) {
    json entry;
    entry["name"] = entryName;
    entry["description"] = description;
    entry["offset_seconds"] = offsetSeconds;
    entry["duration_seconds"] = durationSeconds;
    entry["color_hex"] = entryColorHex;
    entry["offset_next"] = offsetNext;
    entry["periodicity_type"] = "day"; // Periode is bound to day
    entry["periocitity_override"] = periodicity_override;

    periodic_entries.push_back(entry);
}


void PeriodicEvent::AddPeriodicEntryDay(
    std::string entryName,
    std::string description,
    int offsetSeconds,
    int durationSeconds,
    std::string entryColorHex,
    int periodicity_override
) {
    json entry;
    entry["name"] = entryName;
    entry["description"] = description;
    entry["offset_seconds"] = offsetSeconds;
    entry["duration_seconds"] = durationSeconds;
    entry["color_hex"] = entryColorHex;
    entry["offset_next"] = 7200;
    entry["periodicity_type"] = "day"; // Periode is bound to day
    entry["periocitity_override"] = periodicity_override;

    periodic_entries.push_back(entry);
}



void PeriodicEvent::AddPeriodicEntry(
    std::string entryName,
    std::string description,
    int offsetSeconds,
    int durationSeconds,
    int offsetNext,
    std::string entryColorHex
) {
    json entry;
    entry["name"] = entryName;
    entry["description"] = description;
    entry["offset_seconds"] = offsetSeconds;
    entry["duration_seconds"] = durationSeconds;
    entry["color_hex"] = entryColorHex;
    entry["offset_next"] = offsetNext;
    entry["periodicity_type"] = "periode"; // Periode is bound inside period
    entry["periocitity_override"] = 7200; // Assume 2h periode

    periodic_entries.push_back(entry);
}




void PeriodicEvent::AddPeriodicEntry(
    std::string entryName,
    std::string description,
    int offsetSeconds,
    int durationSeconds,
    std::string entryColorHex
) {
    json entry;
    entry["name"] = entryName;
    entry["description"] = description;
    entry["offset_seconds"] = offsetSeconds;
    entry["duration_seconds"] = durationSeconds;
    entry["color_hex"] = entryColorHex;
    entry["offset_next"] = 7200;
    entry["periodicity_type"] = "periode"; // Periode is bound inside period
    entry["periocitity_override"] = 7200; // Assume 2h periode

    periodic_entries.push_back(entry);
}

// Getter for periodicity_seconds
int PeriodicEvent::GetPeriodicitySeconds() const {
    return periodicity_seconds;
}

// Setter for periodicity_seconds
void PeriodicEvent::SetPeriodicitySeconds(int newPeriodicitySeconds) {
    periodicity_seconds = newPeriodicitySeconds;
}

// Getter for periodic_entries
const std::vector<json>& PeriodicEvent::GetPeriodicEntries() const {
    return periodic_entries;
}

// Setter for periodic_entries
void PeriodicEvent::SetPeriodicEntries(const std::vector<json>& newPeriodicEntries) {
    periodic_entries = newPeriodicEntries;
}

// Getter for midnight_offset_seconds
int PeriodicEvent::GetMidnightOffsetSeconds() const {
    return midnight_offset_seconds;
}

// Setter for midnight_offset_seconds
void PeriodicEvent::SetMidnightOffsetSeconds(int newMidnightOffsetSeconds) {
    midnight_offset_seconds = newMidnightOffsetSeconds;
}

json PeriodicEvent::ToJson() const {
    json eventData = Event::ToJson(); // Serialize base Event data

    // Add PeriodicEvent-specific fields
    eventData["periodicity_seconds"] = periodicity_seconds;

    // Serialize periodic entries
    json periodicEntriesArray;
    for (const auto& entry : periodic_entries) {
        periodicEntriesArray.push_back(entry);
    }
    eventData["periodic_entries"] = periodicEntriesArray;

    return eventData;
}

void PeriodicEvent::FromJson(const json& jsonData) {
    Event::FromJson(jsonData);

    if (jsonData.find("periodicity_seconds") != jsonData.end()) {
        periodicity_seconds = jsonData["periodicity_seconds"].get<int>();
    }

    if (jsonData.find("periodic_entries") != jsonData.end()) {
        const json& periodicEntriesArray = jsonData["periodic_entries"];
        if (periodicEntriesArray.is_array()) {
            periodic_entries.clear();
            for (const auto& entry : periodicEntriesArray) {
                periodic_entries.push_back(entry);
            }
        }
    }

    if (jsonData.find("midnight_offset_seconds") != jsonData.end()) {
        midnight_offset_seconds = jsonData["midnight_offset_seconds"].get<int>();
    }
}



PeriodicEvent* PeriodicEvent::CreateFromJson(const json& jsonData) {
    PeriodicEvent* eventInstance = new PeriodicEvent();
    eventInstance->FromJson(jsonData);
    return eventInstance;
}

// Destructor
PeriodicEvent::~PeriodicEvent() {
}