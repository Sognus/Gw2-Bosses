#include "CyclicalCoreWorldbossEventQueue.h"


CyclicalCoreWorldbossEventQueue::CyclicalCoreWorldbossEventQueue() {
    this->head = nullptr;
    this->editMode = false;
}


CyclicalCoreWorldbossEventQueue::~CyclicalCoreWorldbossEventQueue() {
    while (!queue.empty()) {
        CoreWorldbossEvent* element = queue.front();
        queue.pop();
        // element possibly leaks memory but windows is fucking trash OS
    }
}


// Expensive af
void CyclicalCoreWorldbossEventQueue::push(CoreWorldbossEvent* value) {
    if (queue.empty()) {
        head = value;
    }
    queue.push(value);

    if (editMode == false) {
        this->sort();
    }
}

void CyclicalCoreWorldbossEventQueue::sort() {
    // Skip on empty queue
    if (queue.empty()) {
        return;
    }

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
    std::string buffer = "";
    if (editMode == true) {
        buffer += "[EDIT MODE] ";
    }
    buffer += "CyclicalCoreWorldbossEventQueue state => ";
    
    
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
    // It is not possible to pop in edit mode
    if (editMode == true) {
        return nullptr;
    }
    // Return null pointer on empty queue
    if (queue.empty()) {
        return nullptr;
    }

    CoreWorldbossEvent* frontElement = queue.front();
    queue.pop();
    queue.push(frontElement);
    head = queue.front();
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
