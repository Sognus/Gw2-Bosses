#include "Event.h"

Event::Event(std::string name, ImVec2 location, std::string eventType, std::string colorHex)
    : name(name), location(location), event_type(eventType), color_hex(colorHex), scale(0.0f), percentage(0.0f) {}

Event::Event(std::string name, float x, float y, std::string eventType, std::string colorHex)
    : name(name), event_type(eventType), color_hex(colorHex), scale(0.0f), percentage(0.0f) {
    location.x = x;
    location.y = y;
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

ImVec2 Event::GetLocation() const {
    return location;
}

std::string Event::GetEventType() const {
    return event_type;
}

void Event::SetEventType(std::string newEventType) {
    event_type = newEventType;
}

std::string Event::GetColorHex() const {
    return color_hex;
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