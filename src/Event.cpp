#include "Event.h"

Event::Event(std::string name, ImVec2 location, std::string eventType, std::string colorHex)
    : name(name), location(location), event_type(eventType), color_hex(colorHex), scale(0.0f), percentage(0.0f) {}

Event::Event(std::string name, float x, float y, std::string eventType, std::string colorHex)
    : name(name), event_type(eventType), color_hex(colorHex), scale(0.0f), percentage(0.0f) {
    location.x = x;
    location.y = y;
}

Event::Event() {

}

std::string Event::GetName() const {
    return name;
}

float Event::GetScale() const {
    return scale;
}

void Event::SetScale(float newScale) {
    scale = newScale;
}

float Event::GetPercentage() const {
    return percentage;
}

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

std::string Event::GetColorHex() {
    return color_hex;
}

ImVec2& Event::GetLocationPtr() {
    return location;
}


void Event::SetColorHex(std::string newColorHex) {
    color_hex = newColorHex;
}

json Event::ToJson() const {
    json eventData;
    eventData["name"] = name;
    eventData["location"]["x"] = location.x;
    eventData["location"]["y"] = location.y;
    eventData["event_type"] = event_type;
    eventData["color_hex"] = color_hex;
    eventData["scale"] = scale;
    eventData["percentage"] = percentage;
    return eventData;
}

void Event::FromJson(const json& jsonData) {
    name = jsonData["name"].get<std::string>();
    location.x = jsonData["location"]["x"].get<float>();
    location.y = jsonData["location"]["y"].get<float>();
    event_type = jsonData["event_type"].get<std::string>();
    color_hex = jsonData["color_hex"].get<std::string>();
    scale = jsonData["scale"].get<float>();
    percentage = jsonData["percentage"].get<float>();
}


Event* Event::CreateFromJson(const json& json) {
    Event* eventInstance = new Event();
    eventInstance->FromJson(json);
    return eventInstance;
}

// Destructor
Event::~Event() {

}