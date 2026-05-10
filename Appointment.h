#pragma once
#include <iostream>
using namespace std;

class Appointment {
public:
    int appointment_id;
    int doc_id;
    int patient_id;
    char date[11];
    char timeslot[6];
    char status[12];
    Appointment() : appointment_id(0), doc_id(0), patient_id(0)
    {
        date[0] = '\0';
        timeslot[0] = '\0';
        status[0] = '\0';
    }

    Appointment(int appointment_id, int doc_id, int patient_id, const char* date, const char* timeslot, const char* status) {
        this->appointment_id = appointment_id;
        this->doc_id = doc_id;
        this->patient_id = patient_id;
        int i;
        for (i = 0; i < 10 && date[i] != '\0'; i++)
            this->date[i] = date[i];
        this->date[i] = '\0';

        for (i = 0; i < 5 && timeslot[i] != '\0'; i++)
            this->timeslot[i] = timeslot[i];
        this->timeslot[i] = '\0';

        for (i = 0; i < 11 && status[i] != '\0'; i++)
            this->status[i] = status[i];
        this->status[i] = '\0';
    }

    bool operator==(Appointment& obj) {
        if (doc_id != obj.doc_id) {
            return false;
        }
        for (int i = 0; i < 10; i++) {
            if (date[i] != obj.date[i]) 
                return false;
    }

        for (int i = 0; i < 5; i++) {
            if (timeslot[i] != obj.timeslot[i]) {
                return false;
            }
       }
        bool thisCancelled = (status[0] == 'c' && status[1] == 'a');
        bool objCancelled = (obj.status[0] == 'c' && obj.status[1] == 'a');

        return !thisCancelled && !objCancelled;
    }

    friend ostream& operator<<(ostream& out, Appointment& obj) {
        out << "Appointment Details:" << endl<< "Appointment ID= " << obj.appointment_id << endl<< "Doctor ID= " << obj.doc_id << endl<< "Patient ID= " << obj.patient_id << endl<< "Date= " << obj.date << endl<< "Time Slot= " << obj.timeslot << endl<< "Status= " << obj.status << endl;
        return out;
    }
    int getId() { return appointment_id; }
};