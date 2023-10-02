#include "Globals.h"
#include "Event.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

#ifndef PERIODIC_EVENT_H
#define PERIODIC_EVENT_H

class PeriodicEvent : public Event {
public:
    PeriodicEvent(
        std::string name,
        float x,
        float y,
        int midnightOffsetSeconds,
        int periodicitySeconds,
        std::string hexColor
    );

    PeriodicEvent(
        std::string name,
        ImVec2 location,
        int midnightOffsetSeconds,
        int periodicitySeconds,
        std::string hexColor
    );

    void AddPeriodicEntry(
        std::string entryName,
        std::string description,
        int offsetSeconds,
        int durationSeconds,
        std::string entryColorHex
    );

    // Function to serialize the PeriodicEvent to a JSON object
    json ToJson() const;

    // Getter for periodicity_seconds
    int GetPeriodicitySeconds() const;

    // Setter for periodicity_seconds
    void SetPeriodicitySeconds(int newPeriodicitySeconds);

    // Getter for periodic_entries
    const std::vector<json>& GetPeriodicEntries() const;

    // Setter for periodic_entries
    void SetPeriodicEntries(const std::vector<json>& newPeriodicEntries);

    // Getter for midnight_offset_seconds
    int GetMidnightOffsetSeconds() const;

    // Setter for midnight_offset_seconds
    void SetMidnightOffsetSeconds(int newMidnightOffsetSeconds);

private:
    int periodicity_seconds;
    std::vector<json> periodic_entries;
    int midnight_offset_seconds; // Added midnight offset as a private field
};

#endif // PERIODIC_EVENT_H