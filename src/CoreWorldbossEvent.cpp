#include "CoreWorldbossEvent.h"

CoreWorldbossEvent::CoreWorldbossEvent() {
}

CoreWorldbossEvent::CoreWorldbossEvent(
    std::string name,
    float x,
    float y,
    int notifyOffsetSeconds,
    int midnightOffsetSeconds,
    int durationSeconds,
    std::string hexColor
) : Event(
    name,
    x,
    y,
    CORE_WORLD_BOSSES_EVENT_TYPE,
    hexColor
    ),
    notify_offset_seconds(notifyOffsetSeconds),
    midnight_offset_seconds(midnightOffsetSeconds),
    duration_seconds(durationSeconds) {
}

CoreWorldbossEvent::CoreWorldbossEvent(
    std::string name,
    ImVec2 location,
    int notifyOffsetSeconds,
    int midnightOffsetSeconds,
    int durationSeconds,
    std::string hexColor
) : Event(
        name,
        location,
        CORE_WORLD_BOSSES_EVENT_TYPE,
        hexColor
    ),
    notify_offset_seconds(notifyOffsetSeconds),
    midnight_offset_seconds(midnightOffsetSeconds),
    duration_seconds(durationSeconds) {
}

CoreWorldbossEvent::CoreWorldbossEvent(
    std::string name,
    ImVec2 location,
    int midnightOffsetSeconds,
    int durationSeconds,
    std::string hexColor
) : Event(
        name,
        location,
        CORE_WORLD_BOSSES_EVENT_TYPE,
        hexColor
    ),
    notify_offset_seconds(midnightOffsetSeconds),
    midnight_offset_seconds(midnightOffsetSeconds),
    duration_seconds(durationSeconds) {

}

CoreWorldbossEvent::CoreWorldbossEvent(
    std::string name,
    float x,
    float y,
    int midnightOffsetSeconds,
    int durationSeconds,
    std::string hexColor
) : Event(
        name,
        x,
        y,
        CORE_WORLD_BOSSES_EVENT_TYPE,
        hexColor
    ),
    notify_offset_seconds(midnightOffsetSeconds),
    midnight_offset_seconds(midnightOffsetSeconds),
    duration_seconds(durationSeconds) {

}

int CoreWorldbossEvent::GetNotifyOffsetSeconds() {
    return notify_offset_seconds;
}

int CoreWorldbossEvent::GetMidnightOffsetSeconds() {
    return midnight_offset_seconds;
}

int CoreWorldbossEvent::GetDurationSeconds() {
    return duration_seconds;
}

CoreWorldbossEvent::~CoreWorldbossEvent() {}

CoreWorldbossEvent* CoreWorldbossEvent::CreateFromJson(const json& json) {
    CoreWorldbossEvent* eventInstance = new CoreWorldbossEvent();
    eventInstance->FromJson(json);
    return eventInstance;
}

/// <summary>
/// Formats name by stripping ID from it
/// </summary>
/// <returns></returns>
std::string CoreWorldbossEvent::GetFormattedEventName()
{
    size_t spacePos = this->name.find_last_of(" ");
    std::string formattedName;

    if (spacePos != std::string::npos) {
        formattedName = this->name.substr(0, spacePos);
    }
    else {
        formattedName = this->name;
    }

    return formattedName;

}

CoreWorldbossEvent* CoreWorldbossEvent::DeepCopy()
{
    return new CoreWorldbossEvent(*this);
}

json CoreWorldbossEvent::ToJson() const {
    json eventData = Event::ToJson();
    eventData["midnight_offset_seconds"] = midnight_offset_seconds;
    eventData["notify_offset_seconds"] = notify_offset_seconds;
    eventData["duration_seconds"] = duration_seconds;
    return eventData;
}

void CoreWorldbossEvent::FromJson(const json& json) {
    Event::FromJson(json);

    if (json.find("midnight_offset_seconds") != json.end()) {
        midnight_offset_seconds = json["midnight_offset_seconds"].get<int>();
    }

    if (json.find("notify_offset_seconds") != json.end()) {
        notify_offset_seconds = json["notify_offset_seconds"].get<int>();
    }

    if (json.find("duration_seconds") != json.end()) {
        duration_seconds = json["duration_seconds"].get<int>();
    }

}

bool CoreWorldbossEvent::ComparePriority(const CoreWorldbossEvent* event1, const CoreWorldbossEvent* event2) {
    // First check midnight_offset_seconds, lower number = better priority
    if (event1->midnight_offset_seconds < event2->midnight_offset_seconds) {
        return true;
    }
    else if (event1->midnight_offset_seconds == event2->midnight_offset_seconds) {
        // If they are the same, then check notify_offset_seconds, lower number = better priority
        if (event1->notify_offset_seconds < event2->notify_offset_seconds) {
            return true;
        }
        else if (event1->notify_offset_seconds == event2->notify_offset_seconds) {
            // If notify_offset_seconds are also the same, compare names alphabetically
            return event1->GetName() < event2->GetName();
        }
    }

    return false;
}

// Equality comparison based on name
bool CoreWorldbossEvent::operator==(const CoreWorldbossEvent& other) const {
    return name.compare(other.name) == 0;
}

// Equality comparison based on name for pointers
bool CoreWorldbossEvent::operator==(const CoreWorldbossEvent* other) const {
    if (other == nullptr) {
        return false;
    }
    return name.compare(other->name) == 0;
}
