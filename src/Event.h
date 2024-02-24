#include "Globals.h"
#include "nlohmann/json.hpp"
#include "IJsonhandled.h"

#ifndef EVENT_H
#define EVENT_H

using json = nlohmann::json;

class Event : public IJsonHandled {
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

    // Functions to serialize and deserialize the Event to and from a JSON object
    json ToJson() const override;
    void FromJson(const json& json) override;
    static Event CreateFromJson(const json& json);

    // Destructor
    virtual ~Event();

private:

    std::string name;
    ImVec2 location;
    float scale;
    float percentage;
    std::string event_type;
    std::string color_hex;

protected:
    Event();
};

#endif //EVENT_H