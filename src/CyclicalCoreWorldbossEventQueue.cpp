#include "CyclicalCoreWorldbossEventQueue.h"

CoreWorldBossEventNode::CoreWorldBossEventNode(CoreWorldbossEvent* value) : data(value), next(nullptr), prev(nullptr) {}

CyclicalCoreWorldbossEventQueue::CyclicalCoreWorldbossEventQueue() : head(nullptr) {}

void CyclicalCoreWorldbossEventQueue::push(CoreWorldbossEvent* value) {
    if (head == nullptr) {
        head = new CoreWorldBossEventNode(value);
        head->next = head;
        head->prev = head;
    }
    else {
        CoreWorldBossEventNode* newNode = new CoreWorldBossEventNode(value);

        if (head->prev == head && head->next == head) {
            head->next = newNode;
            head->prev = newNode;
            newNode->next = head;
            newNode->prev = head;
        }
        else {
            bool cycle = false;
            CoreWorldBossEventNode* initial = head;
            CoreWorldBossEventNode* current = head;
            CoreWorldBossEventNode* lower = nullptr;
            CoreWorldBossEventNode* upper = nullptr;

            while (true) {
                if (lower != nullptr && upper == nullptr) {
                    if (current->data->GetMidnightOffsetSeconds() > newNode->data->GetMidnightOffsetSeconds() || (current == initial && cycle == true) ) {
                        upper = current;
                    }
                }

                if (current == initial) {
                    if (cycle == true) {
                        break;
                    }
                    else {
                        cycle = true;
                    }
                }

                if (lower == nullptr) {
                    if (newNode->data->GetMidnightOffsetSeconds() > current->data->GetMidnightOffsetSeconds()) {
                        lower = current;
                    }
                    else if (newNode->data->GetMidnightOffsetSeconds() == current->data->GetMidnightOffsetSeconds()) {
                        if (newNode->data->GetName().compare(current->data->GetName()) >= 0) {
                            CoreWorldBossEventNode* hold = current->next;
                            current->next = newNode;
                            newNode->prev = current;
                            newNode->next = hold;
                        }
                        else {
                            newNode->prev = current->prev;
                            newNode->next = current;
                        }
                        break;
                    }
                }

                if (lower != nullptr && upper != nullptr) {
                    lower->next = newNode;
                    newNode->prev = lower;
                    newNode->next = upper;
                    upper->prev = newNode;
                    break;
                }

                current = current->next;
            }
        }
    }
}

void CyclicalCoreWorldbossEventQueue::remove(CoreWorldbossEvent* toRemove) {
    bool cycle = false;
    CoreWorldBossEventNode* initial = head;
    CoreWorldBossEventNode* current = head;

    while (true) {
        if (current == initial) {
            if (cycle == true) {
                break;
            }
            else {
                cycle = true;
            }
        }

        if (current->data == toRemove) {
            CoreWorldBossEventNode* holdPrev = current->prev;
            CoreWorldBossEventNode* holdNext = current->next;

            if (holdPrev != current) {
                holdPrev->next = holdNext;
            }
            else {
                head = (holdNext == current) ? nullptr : holdNext;
            }

            if (holdNext != current) {
                holdNext->prev = holdPrev;
            }

            delete current;
        }

        current = current->next;
    }
}

CoreWorldbossEvent* CyclicalCoreWorldbossEventQueue::peek() {
    return head->data;
}

void CyclicalCoreWorldbossEventQueue::printState() {
    bool cycle = false;
    CoreWorldBossEventNode* initial = head;
    CoreWorldBossEventNode* current = head;

    std::string buffer = "CyclicalCoreWorldbossEventQueue state => ";

    while (current != nullptr) {
        
        if (current == initial) {
            if (cycle) {
                break;
            }
            else {
                cycle = true;
            }
        }
        
        buffer = buffer + "[" + current->data->GetName() + " = " + std::to_string(current->data->GetMidnightOffsetSeconds()) + "] ";


        current = current->next;

        buffer = buffer + ", ";
    }

    APIDefs->Log(ELogLevel::ELogLevel_DEBUG, buffer.c_str());
}

CoreWorldbossEvent* CyclicalCoreWorldbossEventQueue::popEvent() {
    CoreWorldBossEventNode* hold = head;
    head = head->next;
    return hold->data;
}

void CyclicalCoreWorldbossEventQueue::pop() {
    head = head->next;
}
