#include "Globals.h"
#include "Event.h"
#include "nlohmann/json.hpp"

#ifndef PERIODIC_EVENT_H
#define PERIODIC_EVENT_H

using json = nlohmann::json;

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

    void AddPeriodicEntry(
        std::string entryName,
        std::string description,
        int offsetSeconds,
        int durationSeconds,
        int offsetNext,
        std::string entryColorHex
    );

    void AddPeriodicEntryGeneric(
        std::string entryName,
        std::string description,
        int offsetSeconds,
        int durationSeconds,
        std::string entryColorHex,
        std::string periodicityType,
        int periodicity_override
    );

    void AddPeriodicEntryDay(
        std::string entryName,
        std::string description,
        int offsetSeconds,
        int durationSeconds,
        int offsetNext,
        std::string entryColorHex,
        int periodicity_override
    );

    void AddPeriodicEntryDay(
        std::string entryName,
        std::string description,
        int offsetSeconds,
        int durationSeconds,
        std::string entryColorHex,
        int periodicity_override
    );

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

    // Functions to serialize and deserialize the Event to and from a JSON object
    json ToJson() const override;
    void FromJson(const json& json) override;
    static PeriodicEvent* CreateFromJson(const json& json);

    // Destructor
    ~PeriodicEvent() override;

private:
    int periodicity_seconds;
    std::vector<json> periodic_entries;
    int midnight_offset_seconds; // Added midnight offset as a private field

protected:
    PeriodicEvent();
};

#endif // PERIODIC_EVENT_H