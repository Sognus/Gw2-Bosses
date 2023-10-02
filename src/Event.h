#include "Globals.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

#ifndef EVENT_H
#define EVENT_H

class Event {
public:
    Event(
        std::string name,
        ImVec2 location,
        std::string eventType,
        std::string colorHex
    );

    Event(
        std::string name,
        float x,
        float y,
        std::string eventType,
        std::string colorHex
    );

    std::string GetName() const;

    float GetScale() const;
    void SetScale(float newScale);

    float GetPercentage() const;
    void SetPercentage(float newPercentage);

    ImVec2 GetLocation() const;

    std::string GetEventType() const;
    void SetEventType(std::string newEventType);

    std::string GetColorHex() const;
    void SetColorHex(std::string newColorHex);

    // Function to serialize the Event to a JSON object
    json ToJson() const;

private:
    std::string name;
    ImVec2 location;
    float scale;
    float percentage;
    std::string event_type;
    std::string color_hex;
};

#endif //EVENT_H