#include "Globals.h"
#include "CoreWorldbossEvent.h"


#ifndef BOSSES_CYCLICAL_QUEUE
#define BOSSES_CYCLICAL_QUEUE

class CoreWorldBossEventNode {
public:
    CoreWorldbossEvent* data;
    CoreWorldBossEventNode* next;
    CoreWorldBossEventNode* prev;

    CoreWorldBossEventNode(CoreWorldbossEvent* value);
};

class CyclicalCoreWorldbossEventQueue {
protected:
    CoreWorldBossEventNode* head;
public:
    CyclicalCoreWorldbossEventQueue();

    void push(CoreWorldbossEvent* value);

    void remove(CoreWorldbossEvent* toRemove);

    CoreWorldbossEvent* peek();

    void printState();

    CoreWorldbossEvent* popEvent();

    void pop();


};

#endif //BOSSES_CYCLICAL_QUEUE