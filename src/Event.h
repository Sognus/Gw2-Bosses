#include "Globals.h"
#include "nlohmann/json.hpp"
#include "utils/JsonExtension.h"
#include "IJsonhandled.h"

#ifndef EVENT_H
#define EVENT_H

using json = nlohmann::json;

class Event : public IJsonHandled {
public:
    bool markedForAlert;

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

    Event(
        std::string name,
        ImVec2 location,
        std::string eventType,
        std::string colorHex,
        bool aEnabled
    );

    Event(
        std::string name,
        float x,
        float y,
        std::string eventType,
        std::string colorHex,
        bool aEnabled
    );


    std::string GetName() const;

    float GetScale() const;
    void SetScale(float newScale);

    float GetPercentage() const;
    void SetPercentage(float newPercentage);

    /// <summary>
    /// Getter for location by value
    /// </summary>
    /// <returns></returns>
    ImVec2 GetLocation();

    /// <summary>
    /// Getter for location by ref
    /// </summary>
    /// <returns></returns>
    ImVec2& GetLocationPtr();

    void SetLocation(ImVec2 location);

    std::string GetEventType() const;
    void SetEventType(std::string newEventType);

    std::string GetColorHex();
    void SetColorHex(std::string newColorHex);

    // Functions to serialize and deserialize the Event to and from a JSON object
    json ToJson() const override;
    void FromJson(const json& json) override;
    static Event* CreateFromJson(const json& json);

    // Formatted event name
    virtual std::string GetFormattedEventName();

    // Deep copy
    virtual Event* DeepCopy();

    // Extra data getter
    template <typename T>
    T GetData(const std::string& key, const T& defaultValue);

    void SetEnabled(bool aEnabled);

    bool IsEnabled();

    bool* GetEnabledPtr();

    // Destructor
    virtual ~Event();

private:
    ImVec2 location;
    float scale;
    float percentage;
    std::string event_type;
    std::string color_hex;
    bool enabled;

    /// <summary>
    /// Extra data
    /// </summary>
    std::vector<json> data;
    

protected:
    std::string name;

    Event();
};

#endif //EVENT_H