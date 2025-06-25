#include "Event.h"

Event::Event(std::string name, ImVec2 location, std::string eventType, std::string colorHex)
    : name(name), location(location), event_type(eventType), color_hex(colorHex), scale(0.0f), percentage(0.0f) {
    markedForAlert = false;
    enabled = true;
}

Event::Event(std::string name, float x, float y, std::string eventType, std::string colorHex)
    : name(name), event_type(eventType), color_hex(colorHex), scale(0.0f), percentage(0.0f) {
    markedForAlert = false;
    location.x = x;
    location.y = y;
    enabled = true;
}

Event::Event(std::string name, ImVec2 location, std::string eventType, std::string colorHex, bool aEnabled)
    : name(name), location(location), event_type(eventType), color_hex(colorHex), scale(0.0f), percentage(0.0f) {
    markedForAlert = false;
    enabled = aEnabled;
}

Event::Event(std::string name, float x, float y, std::string eventType, std::string colorHex, bool aEnabled)
    : name(name), event_type(eventType), color_hex(colorHex), scale(0.0f), percentage(0.0f) {
    markedForAlert = false;
    location.x = x;
    location.y = y;
    enabled = aEnabled;
}


Event::Event() {
    percentage = 0.0f;
    scale = 0.0f;
    markedForAlert = false;
    enabled = true;
}

std::string Event::GetName() const {
    return name;
}

// TODO: Deprecated
float Event::GetScale() const {
    return scale;
}

// TODO: Deprecated
void Event::SetScale(float newScale) {
    scale = newScale;
}

// TODO: Deprecated
float Event::GetPercentage() const {
    return percentage;
}

// TODO: Deprecated
void Event::SetPercentage(float newPercentage) {
    percentage = newPercentage;
}

ImVec2 Event::GetLocation() {
    return location;
}

void Event::SetLocation(ImVec2 newLocation)
{
    location = newLocation;
}

std::string Event::GetEventType() const {
    return event_type;
}

void Event::SetEventType(std::string newEventType) {
    event_type = newEventType;
}

// TODO: Deprecated
std::string Event::GetColorHex() {
    return color_hex;
}

ImVec2& Event::GetLocationPtr() {
    return location;
}


// TODO: Deprecated
void Event::SetColorHex(std::string newColorHex) {
    color_hex = newColorHex;
}

template <typename T>
T GetData(const std::string& key, const T& defaultValue) {
    return json_getOrElse(this->data, key, defaultValue);
}

json Event::ToJson() const {
    json eventData;
    eventData["name"] = name;
    eventData["alert"] = markedForAlert;
    eventData["location"]["x"] = location.x;
    eventData["location"]["y"] = location.y;
    eventData["event_type"] = event_type;
    eventData["color_hex"] = color_hex;
    
    // TODO: Deprecated
    eventData["scale"] = scale;
    // TODO: Deprecated
    eventData["percentage"] = percentage;

    // Enabled flag
    eventData["enabled"] = enabled;

    // Serialize extra data
    json extraData;
    for (const auto& entry : data) {
        extraData.push_back(entry);
    }
    eventData["data"] = extraData;
    
    
    return eventData;
}

void Event::FromJson(const json& jsonData) {
    name = json_getOrElse<std::string>(jsonData, "name", "");
    markedForAlert = json_getOrElse<bool>(jsonData, "alert", false);
    location.x = json_getOrElse<float>(jsonData["location"], "x", 0.0f);
    location.y = json_getOrElse<float>(jsonData["location"], "y", 0.0f);
    event_type = json_getOrElse<std::string>(jsonData, "event_type", "");
    color_hex = json_getOrElse<std::string>(jsonData, "color_hex", "");
    enabled = json_getOrElse<bool>(jsonData, "enabled", true);
    
    // TODO: Deprecated
    scale = json_getOrElse<float>(jsonData, "scale", 0.0f);
    // TODO: Deprecated
    percentage = json_getOrElse<float>(jsonData, "percentage", 0.0f);

    // Deserialize extra data
    if (jsonData.find("data") != jsonData.end()) {
        const json& extraDataArray = jsonData["data"];
        if (extraDataArray.is_array()) {
            data.clear();
            for (const auto& entry : extraDataArray) {
                data.push_back(entry);
            }
        }
    }

}


Event* Event::CreateFromJson(const json& json) {
    Event* eventInstance = new Event();
    eventInstance->FromJson(json);
    return eventInstance;
}

std::string Event::GetFormattedEventName()
{
    return this->name;
}

Event* Event::DeepCopy() {
    return new Event(*this);
}

void Event::SetEnabled(bool aEnabled)
{
    this->enabled = aEnabled;
}

bool Event::IsEnabled() {
    return this->enabled;
}

bool* Event::GetEnabledPtr()
{
    return &enabled;
}

// Destructor
Event::~Event() {

}