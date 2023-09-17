#include "Event.h"


Event::Event(std::string name, ImVec2 location) : name(name), location(location), scale(1.0f), percentage(0.0f) {}

Event::Event(std::string name, float x, float y) : name(name), location(ImVec2(x, y)), scale(1.0f), percentage(0.0f) {}

std::string Event::GetName() const {
    return name;
}

ImVec2 Event::GetLocation() const {
    return location;
}

float Event::GetScale() {
    return scale;
}

void Event::SetScale(float newScale) {
    scale = newScale;
}

float Event::GetPercentage() {
    return percentage;
}

void Event::SetPercentage(float newPercentage) {
    percentage = newPercentage;
}

