#include "CyclicalCoreWorldbossEventQueue.h"


CyclicalCoreWorldbossEventQueue::CyclicalCoreWorldbossEventQueue() {
    this->head = nullptr;
    this->editMode = false;
}


CyclicalCoreWorldbossEventQueue::~CyclicalCoreWorldbossEventQueue() {
    while (!deque.empty()) {
        CoreWorldbossEvent* element = deque.front();
        deque.pop_front();
    }
}


// Expensive af
void CyclicalCoreWorldbossEventQueue::push(CoreWorldbossEvent* value) {
    if (deque.empty()) {
        head = value;
    }
    deque.push_back(value);

    if (editMode == false) {
        this->sort();
    }
}

void CyclicalCoreWorldbossEventQueue::sort() {
    // Skip on empty queue
    if (deque.empty()) {
        return;
    }

    // Transfer elements to a vector
    std::vector<CoreWorldbossEvent*> tempVector;
    while (!deque.empty()) {
        tempVector.push_back(deque.front());
        deque.pop_front();
    }

    // Sort the vector using your comparison method
    std::sort(tempVector.begin(), tempVector.end(), CoreWorldbossEvent::ComparePriority);

    // Re-populate the queue with sorted elements
    for (const auto& element : tempVector) {
        deque.push_back(element);
    }

    // Find head again after sort - either same element or first of 
    
    CoreWorldbossEvent* frontElement = deque.front();
/*   
    while (frontElement != head) {
        deque.push_back(frontElement);
        deque.pop_front();
        frontElement = deque.front();
    }
*/
    if (frontElement != head) {
        frontElement = deque.front();
    }

}


/* Please not
void CyclicalCoreWorldbossEventQueue::remove(CoreWorldbossEvent* toRemove) {
w   
}
*/

CoreWorldbossEvent* CyclicalCoreWorldbossEventQueue::peek() {
    if (deque.empty()) {
        return nullptr;
    }
    return deque.front();
}

void CyclicalCoreWorldbossEventQueue::printState() {
    std::string buffer = "";
    if (editMode == true) {
        buffer += "[EDIT MODE] ";
    }
    buffer += "CyclicalCoreWorldbossEventQueue state => ";
    
    
    if (!deque.empty()) {
        CoreWorldbossEvent* initial = deque.front();
        do {
            CoreWorldbossEvent* frontElement = deque.front();
            deque.pop_front();
            deque.push_back(frontElement);

            buffer = buffer + "[" + frontElement->GetName() + " = " + std::to_string(frontElement->GetMidnightOffsetSeconds()) + "] ";

        } while (deque.front() != initial);
    }

    APIDefs->Log(ELogLevel::ELogLevel_DEBUG, ADDON_NAME.c_str(), buffer.c_str());
}

CoreWorldbossEvent* CyclicalCoreWorldbossEventQueue::popEvent() {
    // It is not possible to pop in edit mode
    if (editMode == true) {
        return nullptr;
    }
    // Return null pointer on empty queue
    if (deque.empty()) {
        return nullptr;
    }

    CoreWorldbossEvent* frontElement = deque.front();
    deque.pop_front();
    deque.push_back(frontElement);
    head = deque.front();
    return frontElement;
}

void CyclicalCoreWorldbossEventQueue::pop() {
    CyclicalCoreWorldbossEventQueue::popEvent();
}


bool CyclicalCoreWorldbossEventQueue::SetEditMode(bool modeState)
{
    // Desired mode already set
    if (editMode == modeState) {
        return false;
    }

    // Enabling edit mode
    if (modeState == true) {
        modeState = true;
        return true;
    }

    // Disable edit mode and sort
    if (modeState == false) {
        modeState = false;
        this->sort();
        return true;
    }

    // just in case
    return false;
}
