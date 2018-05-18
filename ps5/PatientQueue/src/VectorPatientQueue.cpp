// Source code for the vector implementation of a priority queue.

#include "strlib.h"
#include "vector.h"
#include "VectorPatientQueue.h"

// Defining the private Patient structure.
struct Patient {
    Patient(): name(""), priority(0), timestamp(0){}
    string name;
    int priority;
    int timestamp;
};

const string EMPTY_QUEUE = "There are no patients to process; the priority queue is empty.";

VectorPatientQueue::VectorPatientQueue() {
    // NOTE: the constructor automatically creates an empty vector
    // since the Vector<patient> pq is a private attribute.

    // Initializing clock to zero.
    clock = 0;
}

VectorPatientQueue::~VectorPatientQueue() {

}

void VectorPatientQueue::clear() {
    pq.clear();
}

string VectorPatientQueue::frontName() {

    // Updating clock
    clock++;

    if (isEmpty()){
        throw string(EMPTY_QUEUE);
    }

    int mostUrgentIdx = findMostUrgentPatient();

    return pq[mostUrgentIdx].name;
}

int VectorPatientQueue::frontPriority() {

    // Updating clock
    clock++;

    if (isEmpty()){
        throw string(EMPTY_QUEUE);
    }

    int mostUrgentIdx = findMostUrgentPatient();

    return pq[mostUrgentIdx].priority;
}

bool VectorPatientQueue::isEmpty() {

    return pq.isEmpty();
}

void VectorPatientQueue::newPatient(string name, int priority) {

    // Updating clock
    clock++;

    Patient newPerson;
    newPerson.name = name;
    newPerson.priority = priority;
    newPerson.timestamp = clock;

    // Appending the new patient to the PQ vector
    pq.add(newPerson);

}

string VectorPatientQueue::processPatient() {

    // Updating clock
    clock++;

    string mostUrgentPatientName = frontName();

    // Calling helper function to find most urgent patient index
    int mostUrgentIdx = findMostUrgentPatient();

    // Removing the patient from the queue
    pq.remove(mostUrgentIdx);

    return mostUrgentPatientName;
}

void VectorPatientQueue::upgradePatient(string name, int newPriority) {

    // Updating clock
    clock++;

    int idxUpgrade;
    bool foundCandidate = false;

    for (int ii = 0; ii < pq.size(); ii++) {
        if (pq[ii].name == name and pq[ii].priority > newPriority){
            if (foundCandidate){
                idxUpgrade = comparePatientsPriority(idxUpgrade, ii);
            }
            else {
                foundCandidate = true;
                idxUpgrade = ii;
            }
        }
    }

    if (!foundCandidate){
        throw string("There is no patient named " + name +
                     " who can be upgraded to priority " + integerToString(newPriority) + ".");
    }
    else {
        pq[idxUpgrade].priority = newPriority;
        pq[idxUpgrade].timestamp = clock;
    }

}

string VectorPatientQueue::toString() {

    // Updating clock
    clock ++;

    std::stringstream buffer;

    buffer << "{";
    for (int ii = 0; ii < pq.size(); ii++) {

        if (ii < (pq.size() - 1)) {
            buffer << integerToString(pq[ii].priority) << ":" << pq[ii].name << ", ";
        }
        else {
            buffer << integerToString(pq[ii].priority) << ":" << pq[ii].name << "} ";
        }

    }

    if (isEmpty()) {
        buffer << "}";
    }

    return buffer.str();
}

/*
 * This helper function loops through the current patient priority queue and
 * returns the vector index associated with the patient with most urgent priority.
*/
int VectorPatientQueue::findMostUrgentPatient() {
    int mostUrgentIdx = 0;
    Patient mostUrgent = pq[0];
    for (int ii = 1; ii < pq.size(); ii++) {
        if (pq[ii].priority < mostUrgent.priority){
            mostUrgent = pq[ii];
            mostUrgentIdx =  ii;
        }
        if (pq[ii].priority == mostUrgent.priority and
                pq[ii].timestamp < mostUrgent.timestamp){
            mostUrgent = pq[ii];
            mostUrgentIdx =  ii;
        }
    }
    return mostUrgentIdx;
}

/*
 * This helper function takes the index of two patients in the vector and
 * returns the index of the patient with the most urgent priority.
*/

int VectorPatientQueue::comparePatientsPriority(int idx1, int idx2) {

    if (pq[idx1].priority > pq[idx2].priority) {
        return idx2;
    }
    else if (pq[idx1].priority == pq[idx2].priority) {
        if (pq[idx1].timestamp < pq[idx2].timestamp) {
            return idx1;
        }
        else {
            return idx2;
        }
    }
    else {
        return idx1;
    }

}
