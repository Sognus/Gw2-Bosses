#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <queue>  
#include <functional> 

#include "Globals.h"
#include "utils/Render.h"
#include "utils/Map.h"
#include "utils/Strings.h"
#include "utils/BoundingBox.h"
#include "Event.h"
#include "PeriodicEvent.h"
#include "CoreWorldbossEvent.h"
#include "CyclicalCoreWorldbossEventQueue.h"


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
    void LoadCoreWorldbossesFallback();
    void ExportEventsJson();

    Event* GetEvent(const std::string& eventName);
    void AddEvent(Event *event);
    void AddCoreWorldbossEvent(Event* event);

    std::unordered_map<std::string, Event*> events;
    CyclicalCoreWorldbossEventQueue* worldBossesNotifications;
};


#endif // ADDON_H