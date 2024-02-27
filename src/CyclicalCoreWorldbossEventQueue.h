#include <vector>
#include <queue>
#include "Globals.h"
#include "CoreWorldbossEvent.h"


#ifndef BOSSES_CYCLICAL_QUEUE
#define BOSSES_CYCLICAL_QUEUE


class CyclicalCoreWorldbossEventQueue {
private: 
    // Edit mode to disable sorting when adding bulk of data
    bool editMode;
    void sort();

protected:
    std::queue<CoreWorldbossEvent*> queue;
    CoreWorldbossEvent* head;
public:
    CyclicalCoreWorldbossEventQueue();
    ~CyclicalCoreWorldbossEventQueue();

    void push(CoreWorldbossEvent* value);

    // Please not
    //void remove(CoreWorldbossEvent* toRemove);

    CoreWorldbossEvent* peek();

    void printState();

    CoreWorldbossEvent* popEvent();

    void pop();

    /// <summary>
    /// Sets edit mode to desired state - enabled/disabled
    /// </summary>
    /// <param name="modeState">true - set to enabled; false - set to disabled</param>
    /// <returns>Result of operation</returns>
    bool SetEditMode(bool modeState);


};

#endif //BOSSES_CYCLICAL_QUEUE