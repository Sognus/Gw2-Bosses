#include "CyclicalCoreWorldbossEventQueue.h"


CyclicalCoreWorldbossEventQueue::CyclicalCoreWorldbossEventQueue() {
    head = nullptr;
}

CyclicalCoreWorldbossEventQueue::~CyclicalCoreWorldbossEventQueue() {
    while (queue.empty()) {
        CoreWorldbossEvent* element = queue.front();
        queue.pop();
        delete element;
    }
}


// Expensive af
void CyclicalCoreWorldbossEventQueue::push(CoreWorldbossEvent* value) {
    if (queue.empty()) {
        head = value;
    }
    queue.push(value);
    
    // Transfer elements to a vector
    std::vector<CoreWorldbossEvent*> tempVector;
    while (!queue.empty()) {
        tempVector.push_back(queue.front());
        queue.pop();
    }

    // Sort the vector using your comparison method
    std::sort(tempVector.begin(), tempVector.end(), CoreWorldbossEvent::ComparePriority);

    // Re-populate the queue with sorted elements
    for (const auto& element : tempVector) {
        queue.push(element);
    }
    
    // Find head again after sort
    CoreWorldbossEvent* frontElement = queue.front();
    while (frontElement != head) {
        queue.push(frontElement);
        queue.pop();
        frontElement = queue.front();
    }
}


/* Please not
void CyclicalCoreWorldbossEventQueue::remove(CoreWorldbossEvent* toRemove) {

}
*/

CoreWorldbossEvent* CyclicalCoreWorldbossEventQueue::peek() {
    if (queue.empty()) {
        return nullptr;
    }
    return queue.front();
}

void CyclicalCoreWorldbossEventQueue::printState() {
    std::string buffer = "CyclicalCoreWorldbossEventQueue state => ";
    if (!queue.empty()) {
        CoreWorldbossEvent* initial = queue.front();
        do {
            CoreWorldbossEvent* frontElement = queue.front();
            queue.pop();
            queue.push(frontElement);

            buffer = buffer + "[" + frontElement->GetName() + " = " + std::to_string(frontElement->GetMidnightOffsetSeconds()) + "] ";

        } while (queue.front() != initial);
    }

    APIDefs->Log(ELogLevel::ELogLevel_DEBUG, buffer.c_str());
}

CoreWorldbossEvent* CyclicalCoreWorldbossEventQueue::popEvent() {
    CoreWorldbossEvent* frontElement = queue.front();
    queue.pop();
    queue.push(frontElement);
    head = queue.front();
    return frontElement;
}

void CyclicalCoreWorldbossEventQueue::pop() {
    CyclicalCoreWorldbossEventQueue::popEvent();
}
