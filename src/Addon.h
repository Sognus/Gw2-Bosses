#include <unordered_map>
#include "Event.h"


#ifndef ADDON_H
#define ADDON_H

class Addon {
public:
    Addon();  // Constructor
    ~Addon(); // Destructor

    void Render(); // Addon render function

private:
    void RenderEvents();
    void Update();

    void LoadEvents();
    void LoadEventsFallback();
    void ExportEventsJson();

    Event* GetEvent(const std::string& eventName);
    void AddEvent(Event *event);
    std::unordered_map<std::string, Event*> events;
};


#endif // ADDON_H