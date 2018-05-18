// This is the H file you will edit and turn in. (TODO: Remove this comment!)

#pragma once

#include <iostream>
#include <string>
#include "patientqueue.h"
using namespace std;

// Declaring the Patient struct.  The struct is intended to be private,
// thus only declared here but defined in the .cpp file.
struct Patient;

class VectorPatientQueue : public PatientQueue {
public:
    VectorPatientQueue();
    ~VectorPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private:
    int clock;
    Vector<Patient> pq;
    int findMostUrgentPatient();
    int comparePatientsPriority(int idx1, int idx2);
};
