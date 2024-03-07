#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <queue>  
#include <functional> 
#include <algorithm>

#include "Globals.h"
#include "utils/Render.h"
#include "utils/Map.h"
#include "utils/Strings.h"
#include "utils/BoundingBox.h"
#include "utils/Time.h"
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

     /*
      *      Render
      */

    void Render(); // Addon render function

    /*
     *      Behaviour
     */

     // Global render enabled-disabled flag
    bool render;
    // Show/Hide notification boxes
    bool showNotifications;

private:
    /*
     *      Render & Update
     */

    void RenderEvents();
    void RenderNotificationsMap();
    void RenderNotifications();
    void Update();
    
    /*
     *      Events
     */ 

    void LoadEvents();
    void LoadEventsFallback();
    void LoadCoreWorldbossesFallback();
    void ExportEventsJson();

    Event* GetEvent(const std::string& eventName);
    void AddEvent(Event* event);
    void AddCoreWorldbossEvent(Event* event);

    // All available events
    std::unordered_map<std::string, Event*> events;
    // Available core world bosses
    CyclicalCoreWorldbossEventQueue* worldBossesNotifications;
    // Notification box - event that are in progress
    std::vector<Event*> notificationBoxInProgress;
    // Notification box - event that are upcoming
    std::vector<Event*> notificationBoxUpcoming;

};


#endif // ADDON_H