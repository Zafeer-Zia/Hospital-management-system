
#pragma once 
#include <iostream>
using namespace std;

class Bill {
public:
    int appointment_id;
    int bill_id;
    int patient_id;
    char date[11];
    double amount;
    char status[12];
    Bill() : bill_id(0), appointment_id(0), patient_id(0), amount(0)
    {
        date[0] = '\0';
        status[0] = '\0';
    }

    Bill(int bill_id, int appointment_id, int patient_id, const char* date, double amount, const char* status)
    {
        this->appointment_id = appointment_id;
        this->bill_id = bill_id;
        this->patient_id = patient_id;
        int i;
        for (i = 0; i < 10 && date[i] != '\0'; i++){
            this->date[i] = date[i];
    }
        this->date[i] = '\0';
        this->amount = amount;
        for (i = 0; i < 11 && status[i] != '\0'; i++) {
            this->status[i] = status[i];
        }
        this->status[i] = '\0';
    
    }
    friend ostream& operator<<(ostream& out, Bill& obj) {
        out << "Bill ID= " << obj.bill_id << " | Appointment ID= " << obj.appointment_id << " | Patient ID= " << obj.patient_id << endl;
        out << "Amount= Rs " << obj.amount << " | Status= " << obj.status << " | Date= " << obj.date << endl;
        return out;
    }
    int getId() { 
        return bill_id;
    }
};