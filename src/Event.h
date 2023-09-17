#include "Globals.h"

#ifndef EVENT_H
#define EVENT_H

class Event {
public:
    Event(std::string name, ImVec2 location);
    Event(std::string name, float x, float y);

    std::string GetName() const;
    
    float GetScale();
    void SetScale(float newScale);

    float GetPercentage();
    void SetPercentage(float newPercentage);

    ImVec2 GetLocation() const;

private:
    std::string name;
    ImVec2 location;
    float scale;
    float percentage;
};

#endif //EVENT_H