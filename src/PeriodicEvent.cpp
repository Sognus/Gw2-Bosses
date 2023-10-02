#include "PeriodicEvent.h"


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

    periodic_entries.push_back(entry);
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