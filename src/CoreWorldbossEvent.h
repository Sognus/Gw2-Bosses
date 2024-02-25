#include "Globals.h"
#include "Event.h"
#include "nlohmann/json.hpp"

#ifndef CORE_WORLD_BOSS_EVENT
#define CORE_WORLD_BOSS_EVENT

using json = nlohmann::json;
const std::string CORE_WORLD_BOSSES_EVENT_TYPE = "core_world_bosses";

class CoreWorldbossEvent : public Event {
public:
    // Constructors
    CoreWorldbossEvent(
        std::string name,
        float x,
        float y,
        int notifyOffsetSeconds,
        int midnightOffsetSeconds,
        int durationSeconds,
        std::string hexColor
    );

    CoreWorldbossEvent(
        std::string name,
        ImVec2 location, // x, y 
        int notifyOffsetSeconds,
        int midnightOffsetSeconds,
        int durationSeconds,
        std::string hexColor
    );

    CoreWorldbossEvent(
        std::string name,
        ImVec2 location,
        // notifyOffsetSeconds assumed same as midnightOffsetSeconds
        int midnightOffsetSeconds,
        int durationSeconds,
        std::string hexColor
    );

    CoreWorldbossEvent(
        std::string name,
        float x,
        float y,
        // notifyOffsetSeconds assumed same as midnightOffsetSeconds
        int midnightOffsetSeconds,
        int durationSeconds,
        std::string hexColor
    );

    // Getters
    int GetNotifyOffsetSeconds();
    int GetMidnightOffsetSeconds();
    int GetDurationSeconds();;

    // Functions to serialize and deserialize the Event to and from a JSON object
    json ToJson() const override;
    void FromJson(const json& jsonData) override;
    static CoreWorldbossEvent* CreateFromJson(const json& jsonData);

    // Destructor
    virtual ~CoreWorldbossEvent();

    // Operators
    bool operator==(const CoreWorldbossEvent& other) const;
    bool operator==(const CoreWorldbossEvent* other) const;

private:
    // Event is being notified between notify_offset_seconds and midnight_offset_seconds
    int notify_offset_seconds;
    // When event is happening
    int midnight_offset_seconds;
    // Event is happening between midnight_offset_seconds and midnight_offset_seconds + duration_seconds
    int duration_seconds;

protected:
    // Protected empty constructor
    CoreWorldbossEvent();

    // Priority comparison function
    static bool ComparePriority(const CoreWorldbossEvent* event1, const CoreWorldbossEvent* event2);
};

#endif;