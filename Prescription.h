#pragma once
#include <iostream>
using namespace std;

class Prescription {
public:
    int prescription_id;
    int appointment_id;
    int patient_id;
    int doctor_id;
    char date[11];
    char medicines[500];
    char notes[301];
    Prescription()
    {
        prescription_id = 0;
        appointment_id = 0;
        patient_id = 0;
        doctor_id = 0;
        date[0] = '\0';
        medicines[0] = '\0';
        notes[0] = '\0';
    }
    Prescription(int prescription_id, int appointment_id, int patient_id, int doctor_id, const char* date, const char* medicines, const char* notes) {
        this->prescription_id = prescription_id;
        this->appointment_id = appointment_id;
        this->patient_id = patient_id;
        this->doctor_id = doctor_id;
        int i;
        for (i = 0; i < 10 && date[i] != '\0'; i++)
            this->date[i] = date[i];
        this->date[i] = '\0';

        for (i = 0; i < 499 && medicines[i] != '\0'; i++)
            this->medicines[i] = medicines[i];
        this->medicines[i] = '\0';

        for (i = 0; i < 300 && notes[i] != '\0'; i++)
            this->notes[i] = notes[i];
        this->notes[i] = '\0';
    }

    friend ostream& operator<<(ostream& out, Prescription& obj) {
        out << "Prescription ID= " << obj.prescription_id << "  Appointment ID= " << obj.appointment_id << "  Date= " << obj.date << endl;
        out << "Medicines= " << obj.medicines << endl;
        out << "Notes= " << obj.notes << endl;
        return out;
    }
    int getId() { 
        return prescription_id;
    }
};