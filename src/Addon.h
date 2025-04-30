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

struct ComboBoxItem {
    int value;
    std::string text;
};

class SizedMemoryChar {
public:
    char* ptr;
    size_t size;

    /// <summary>
    /// Constructor to create buffer from const char* string - null terminator included
    /// </summary>
    /// <param name="str"></param>
    SizedMemoryChar(const char* str) {
        size = strlen(str);
        ptr = new char[size + 1]; // +1 for null terminator
        std::strcpy(ptr, str);
    }

    /// <summary>
    /// Constructor to create empty buffer of SizedMemoryChar - null terminator not included
    /// </summary>
    /// <param name="bufferSize"></param>
    SizedMemoryChar(size_t bufferSize) : size(bufferSize) {
        ptr = new char[size];
        std::memset(ptr, 0, size);
    }



    void Set(const char* data) {
        size_t dataSize = strlen(data);
        if (dataSize > size) {
            // If dataSize exceeds buffer size, only copy until the buffer size
            dataSize = size;
        }
        std::strncpy(ptr, data, dataSize);
        // Null-terminate the string
        ptr[dataSize] = '\0';
    }

    void Set(std::string data) {
        this->Set(data.c_str());
    }


    // Destructor
    ~SizedMemoryChar() {
        delete[] ptr;
    }

};

struct EditorBuffer {
    Event* editorEditedEvent;
    SizedMemoryChar* hexBuffer;
};


class Addon {
public:
    Addon();  // Constructor
    ~Addon(); // Destructor


     /*
      *      Render
      */

    void Render(); // Addon render function
    void RenderOptions(); // Addon options render function

    /*
     *      Behaviour
     */

    // DPI scale
    bool showDebugCrosshair;
    bool enableDPIScaleOverride;
    float DPIScaleOverride;
    bool useNexusNotifications;
  

     // Global render enabled-disabled flag
    bool render;
    // Show/Hide notification boxes
    bool showNotifications;
    // Aditional notify offset
    int additionalNotifyOffsetIndex;
    std::vector<ComboBoxItem> additionalOffsetChoices;
    // Editor selection
    std::string editorSelectedEventName;
    EditorBuffer editorBuffer;

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

    void SendUpcomingEventAlert(Event* event);
    void SendInProgressEventAlert(Event* event);

    void LoadEvents();
    void LoadEventsFallback();
    void LoadEventOverrides();
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