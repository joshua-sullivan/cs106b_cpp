// Source code for the linked-list implementation of a priority queue.

#include "LinkedListPatientQueue.h"
#include "strlib.h"

const string EMPTY_QUEUE = "There are no patients to process; the priority queue is empty.";

LinkedListPatientQueue::LinkedListPatientQueue() {

    ptrFrontPatient = nullptr;
}

LinkedListPatientQueue::~LinkedListPatientQueue() {

    clear();
}

void LinkedListPatientQueue::clear() {

    while (ptrFrontPatient != nullptr) {
        PatientNode* ptrNextPatient = ptrFrontPatient->next;
        delete ptrFrontPatient;
        ptrFrontPatient = ptrNextPatient;
    }
}

string LinkedListPatientQueue::frontName() {

    if (isEmpty()) {
        throw string(EMPTY_QUEUE);
    }

    return ptrFrontPatient->name;
}

int LinkedListPatientQueue::frontPriority() {

    if (isEmpty()) {
        throw string(EMPTY_QUEUE);
    }

    return ptrFrontPatient->priority;
}

bool LinkedListPatientQueue::isEmpty() {

    return ptrFrontPatient == nullptr;
}

void LinkedListPatientQueue::newPatient(string name, int priority) {

    // Initialize a new patient node and set its attributes from the function input
    PatientNode* ptrNewPatient = new PatientNode;
    ptrNewPatient->name = name;
    ptrNewPatient->priority = priority;
    // Since we don't know where it goes in the queue, we leave this as null for now.
    ptrNewPatient->next = nullptr;

    // If the front prointer is not null then there is something in the queue, now we need to
    // compare the new patient to the existing patients in the queue.
    if (ptrFrontPatient != nullptr) {

        // Looping through all the patients in the queue and finding the new patients spot.
        PatientNode* ptrCurrentPatient = ptrFrontPatient;
        do {

            if (ptrCurrentPatient == ptrFrontPatient and
                    priority < ptrFrontPatient->priority) {

                ptrNewPatient->next = ptrFrontPatient;
                ptrFrontPatient = ptrNewPatient;

                break;
            }

            PatientNode* ptrNextPatient = ptrCurrentPatient->next;

            if (ptrNextPatient == nullptr) {

                ptrCurrentPatient->next = ptrNewPatient;
                break;
            }

            if (priority < ptrNextPatient->priority and
                    priority >= ptrCurrentPatient->priority) {

                ptrNewPatient->next = ptrNextPatient;
                ptrCurrentPatient->next = ptrNewPatient;
                break;
            }

            ptrCurrentPatient = ptrNextPatient;

        } while (ptrCurrentPatient != nullptr);

    }
    // Else the queue is empty, and the front pointer should point to the new patient node
    else {
        ptrFrontPatient = ptrNewPatient;
    }
}

string LinkedListPatientQueue::processPatient() {

    string toReturn = frontName();

    PatientNode* ptrCpFrontPatient = ptrFrontPatient;
    ptrFrontPatient = ptrCpFrontPatient->next;
    delete ptrCpFrontPatient;

    return toReturn;
}

void LinkedListPatientQueue::upgradePatient(string name, int newPriority) {

    bool foundUpgradePatient = false;
    bool foundPostUpgradePrior = false;
    bool newSpotAtFront = (newPriority < ptrFrontPatient->priority);

    PatientNode* ptrUpgradePatient = nullptr;
    PatientNode* ptrPostUpgradePriorPatient = nullptr;
    PatientNode* ptrPreUpgradePriorPatient = nullptr;

    PatientNode* ptrCurrentPatient = ptrFrontPatient;
    while (ptrCurrentPatient != nullptr) {

        // Finding most urgent priority instance of patient with name "name"
        if (ptrCurrentPatient->name == name and ptrCurrentPatient->priority > newPriority) {
            foundUpgradePatient  = true;
            ptrUpgradePatient = ptrCurrentPatient;
            ptrUpgradePatient->priority = newPriority;

            // If it's at the front of the queue, you're done.
            if (ptrCurrentPatient == ptrFrontPatient){
                return;
            }

            if (foundPostUpgradePrior or newSpotAtFront) {
                break;
            }
        }

        PatientNode* ptrNextPatient = ptrCurrentPatient->next;

        // Finding the location in the queue where the patient should be upgraded to.
        if((ptrNextPatient == nullptr) or (ptrNextPatient->priority > newPriority and
                ptrCurrentPatient->priority <= newPriority)) {

            foundPostUpgradePrior = true;
            ptrPostUpgradePriorPatient = ptrCurrentPatient;

            if (foundUpgradePatient) {
                break;
            }
        }

        ptrCurrentPatient = ptrNextPatient;
    }

    if (foundUpgradePatient) {

        // Finding the patient right before the upgrading patient (prior to the upgrade)
        for (PatientNode* ptrTemp = ptrFrontPatient; ptrTemp != nullptr; ptrTemp = ptrTemp->next) {

            if (ptrTemp->next == ptrUpgradePatient){

                ptrPreUpgradePriorPatient = ptrTemp;
                break;
            }
        }



        if (!newSpotAtFront) {
            // If new spot is not at the front, then shift all the pointers around
            ptrPreUpgradePriorPatient->next = ptrUpgradePatient->next;
            ptrUpgradePatient->next = ptrPostUpgradePriorPatient->next;
            ptrPostUpgradePriorPatient->next = ptrUpgradePatient;
        }
        else {
            // If new spot is at the front, similar shift as above, but now the front pointer must be re-directed.
            ptrPreUpgradePriorPatient->next = ptrUpgradePatient->next;
            ptrUpgradePatient->next = ptrFrontPatient;
            ptrFrontPatient = ptrUpgradePatient;
        }


    }
    else {
        throw string("There is no patient named " + name +
                     " who can be upgraded to priority " + integerToString(newPriority) + ".");
    }

}

string LinkedListPatientQueue::toString() {

    std::stringstream buffer;

    buffer << "{";
    for (PatientNode* ptrTemp = ptrFrontPatient; ptrTemp != nullptr; ptrTemp = ptrTemp->next) {

        if (ptrTemp->next != nullptr) {
            buffer << integerToString(ptrTemp->priority) << ":" << ptrTemp->name << ", ";
        }
        else {
            buffer << integerToString(ptrTemp->priority) << ":" << ptrTemp->name << "} ";
        }
    }

    if (isEmpty()) {
        buffer << "}";
    }

    return buffer.str();
}
