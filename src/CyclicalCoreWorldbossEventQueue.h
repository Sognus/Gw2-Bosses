#include <vector>
#include <queue>
#include "Globals.h"
#include "CoreWorldbossEvent.h"


#ifndef BOSSES_CYCLICAL_QUEUE
#define BOSSES_CYCLICAL_QUEUE


class CyclicalCoreWorldbossEventQueue {
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


};

#endif //BOSSES_CYCLICAL_QUEUE