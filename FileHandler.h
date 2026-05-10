#pragma once
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "Exceptions.h"
#include <fstream>
using namespace std;

class FileHandler
{
public:
    void parse_line(char* token, const char* line, int& index, int maxLen)
    {
        int i = 0;
        while (line[index] != ',' && line[index] != '\n' && line[index] != '\0' && line[index] != '\r' && i < maxLen - 1)
        {
            token[i] = line[index];
            i++;
            index++;
        }
        token[i] = '\0';
        if (line[index] == ',' || line[index] == '\n' || line[index] == '\r')
        {
            index++;
        }
    }

    void char_to_int(char* buf, int& result)
    {
        result = 0;
        for (int i = 0; buf[i] != '\0'; i++)
        {
            result = result * 10 + (buf[i] - '0');
        }
    }

    void char_to_double(char* buf, double& result)
    {
        result = 0;
        int j = 0;
        while (buf[j] != '.' && buf[j] != '\0')
        {
            result = result * 10 + (buf[j] - '0');
            j++;
        }
        if (buf[j] == '.')
        {
            j++;
            double factor = 0.1;
            while (buf[j] != '\0')
            {
                result = result + (buf[j] - '0') * factor;
                factor = factor * 0.1;
                j++;
            }
        }
    }

    void load_patients(Storage<Patient>& storage)
    {
        ifstream file("patients.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        char line[300];
        while (file.getline(line, 300))
        {
            int index = 0;
            char id_buf[10], name_buf[51], age_buf[5], gender_buf[3], contact_buf[12], password_buf[51], balance_buf[15];
            parse_line(id_buf, line, index, 10);
            parse_line(name_buf, line, index, 51);
            parse_line(age_buf, line, index, 5);
            parse_line(gender_buf, line, index, 3);
            parse_line(contact_buf, line, index, 12);
            parse_line(password_buf, line, index, 51);
            parse_line(balance_buf, line, index, 15);
            
            int patient_id, age;
            double balance;
            char_to_int(id_buf, patient_id);
            char_to_int(age_buf, age);
            char_to_double(balance_buf, balance);
            bool gender = (gender_buf[0] == 'M');
            Patient p(patient_id, name_buf, age, gender, password_buf, contact_buf, balance);
            storage.add(p);
            
        }
        file.close();
    }

    void load_doctors(Storage<Doctor>& storage)
    {
        ifstream file("doctors.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        char line[300];
        while (file.getline(line, 300))
        {
            if (line[0] == '\0' || line[0] == '\r') continue;
            int index = 0;
            char id_buf[10], name_buf[51], spec_buf[51], contact_buf[12], password_buf[51], fee_buf[15];
            parse_line(id_buf, line, index, 10);
            parse_line(name_buf, line, index, 51);
            parse_line(spec_buf, line, index, 51);
            parse_line(contact_buf, line, index, 12);
            parse_line(password_buf, line, index, 51);
            parse_line(fee_buf, line, index, 15);

            
            int doctor_id;
            double fee;
            char_to_int(id_buf, doctor_id);
            char_to_double(fee_buf, fee);
            Doctor d(doctor_id, name_buf, spec_buf, password_buf, contact_buf, fee);
            storage.add(d);
        }
        file.close();
    }
    void load_admin(Admin& admin)
    {
        ifstream file("admin.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        char line[200];
        file.getline(line, 200);
        int index = 0;
        char id_buf[10], name_buf[51], password_buf[51];
        parse_line(id_buf, line, index, 10);
        parse_line(name_buf, line, index, 51);
        parse_line(password_buf, line, index, 51);
        int admin_id;
        char_to_int(id_buf, admin_id);
        admin = Admin(admin_id, name_buf, password_buf);
        file.close();
    }

    void load_appointments(Storage<Appointment>& storage)
    {
        ifstream file("appointments.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        char line[200];
        while (file.getline(line, 200))
        {
            int index = 0;
            char appointment_id_buf[10], patient_id_buf[10], doctor_id_buf[10], date_buf[11], slot_buf[6], status_buf[12];
            parse_line(appointment_id_buf, line, index, 10);
            parse_line(patient_id_buf, line, index, 10);
            parse_line(doctor_id_buf, line, index, 10);
            parse_line(date_buf, line, index, 11);
            parse_line(slot_buf, line, index, 6);
            parse_line(status_buf, line, index, 12);
            int appointment_id, patient_id, doctor_id;
            char_to_int(appointment_id_buf, appointment_id);
            char_to_int(patient_id_buf, patient_id);
            char_to_int(doctor_id_buf, doctor_id);
            Appointment a(appointment_id, doctor_id, patient_id, date_buf, slot_buf, status_buf);
            storage.add(a);
        }
        file.close();
    }

    void load_bills(Storage<Bill>& storage)
    {
        ifstream file("bills.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        char line[200];
        while (file.getline(line, 200))
        {
            int index = 0;
            char bill_id_buf[10], patient_id_buf[10], appointment_id_buf[10], amount_buf[15], status_buf[12], date_buf[11];
            parse_line(bill_id_buf, line, index, 10);
            parse_line(patient_id_buf, line, index, 10);
            parse_line(appointment_id_buf, line, index, 10);
            parse_line(amount_buf, line, index, 15);
            parse_line(status_buf, line, index, 12);
            parse_line(date_buf, line, index, 11);
            int bill_id, patient_id, appointment_id;
            double amount;
            char_to_int(bill_id_buf, bill_id);
            char_to_int(patient_id_buf, patient_id);
            char_to_int(appointment_id_buf, appointment_id);
            char_to_double(amount_buf, amount);
            Bill b(bill_id, appointment_id, patient_id, date_buf, amount, status_buf);
            storage.add(b);
        }
        file.close();
    }

    void load_prescriptions(Storage<Prescription>& storage)
    {
        ifstream file("prescriptions.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        char line[900];
        while (file.getline(line, 900))
        {
            int index = 0;
            char prescription_id_buf[10], appointment_id_buf[10], patient_id_buf[10], doctor_id_buf[10], date_buf[11], medicines_buf[500], notes_buf[301];
            parse_line(prescription_id_buf, line, index, 10);
            parse_line(appointment_id_buf, line, index, 10);
            parse_line(patient_id_buf, line, index, 10);
            parse_line(doctor_id_buf, line, index, 10);
            parse_line(date_buf, line, index, 11);
            parse_line(medicines_buf, line, index, 500);
            parse_line(notes_buf, line, index, 301);
            int prescription_id, appointment_id, patient_id, doctor_id;
            char_to_int(prescription_id_buf, prescription_id);
            char_to_int(appointment_id_buf, appointment_id);
            char_to_int(patient_id_buf, patient_id);
            char_to_int(doctor_id_buf, doctor_id);
            Prescription p(prescription_id, appointment_id, patient_id, doctor_id, date_buf, medicines_buf, notes_buf);
            storage.add(p);
        }
        file.close();
    }

    void append_patient(Patient& p)
    {
        ofstream file("patients.txt", ios::app);
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        file << p.getId() << "," << p.getName() << "," << p.getAge() << ",";
        if (p.getGender())
        {
            file << "M";
        }
        else
        {
            file << "F";
        }
        file << "," << p.getContact() << "," << p.getPassword() << "," << p.getBalance() << "\n";
        file.close();
    }

    void append_doctor(Doctor& d)
    {
        ofstream file("doctors.txt", ios::app);
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        file << d.getId() << "," << d.getName() << "," << d.getSpecialization() << "," << d.getContact() << "," << d.getPassword() << "," << d.getFees() << "\n";
        file.close();
    }

    void append_appointment(Appointment& a)
    {
        ofstream file("appointments.txt", ios::app);
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        file << a.appointment_id << "," << a.patient_id << "," << a.doc_id << "," << a.date << "," << a.timeslot << "," << a.status << "\n";
        file.close();
    }

    void append_bill(Bill& b)
    {
        ofstream file("bills.txt", ios::app);
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        file << b.bill_id << "," << b.patient_id << "," << b.appointment_id << "," << b.amount << "," << b.status << "," << b.date << "\n";
        file.close();
    }

    void append_prescription(Prescription& p)
    {
        ofstream file("prescriptions.txt", ios::app);
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        file << p.prescription_id << "," << p.appointment_id << "," << p.patient_id << "," << p.doctor_id << "," << p.date << "," << p.medicines << "," << p.notes << "\n";
        file.close();
    }

    void update_patients(Storage<Patient>& storage)
    {
        ofstream file("patients.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        for (int i = 0; i < storage.size(); i++)
        {
            file << storage.data[i].getId() << "," << storage.data[i].getName() << "," << storage.data[i].getAge() << ",";
            if (storage.data[i].getGender())
            {
                file << "M";
            }
            else
            {
                file << "F";
            }
            file << "," << storage.data[i].getContact() << "," << storage.data[i].getPassword() << "," << storage.data[i].getBalance() << "\n";
        }
        file.close();
    }

    void update_doctors(Storage<Doctor>& storage)
    {
        ofstream file("doctors.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        for (int i = 0; i < storage.size(); i++)
        {
            file << storage.data[i].getId() << "," << storage.data[i].getName() << "," << storage.data[i].getSpecialization() << "," << storage.data[i].getContact() << "," << storage.data[i].getPassword() << "," << storage.data[i].getFees() << "\n";
        }
        file.close();
    }

    void update_appointments(Storage<Appointment>& storage)
    {
        ofstream file("appointments.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        for (int i = 0; i < storage.size(); i++)
        {
            file << storage.data[i].appointment_id << "," << storage.data[i].patient_id << "," << storage.data[i].doc_id << "," << storage.data[i].date << "," << storage.data[i].timeslot << "," << storage.data[i].status << "\n";
        }
        file.close();
    }

    void update_bills(Storage<Bill>& storage)
    {
        ofstream file("bills.txt");
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        for (int i = 0; i < storage.size(); i++)
        {
            file << storage.data[i].bill_id << "," << storage.data[i].patient_id << "," << storage.data[i].appointment_id << "," << storage.data[i].amount << "," << storage.data[i].status << "," << storage.data[i].date << "\n";
        }
        file.close();
    }

    void log_security(const char* timestamp, const char* role, const char* entered_id, const char* result)
    {
        ofstream file("security_log.txt", ios::app);
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        file << timestamp << "," << role << "," << entered_id << "," << result << "\n";
        file.close();
    }

    void append_discharged(Patient& p)
    {
        ofstream file("discharged.txt", ios::app);
        if (!file.is_open())
        {
            throw FileNotFoundException();
        }
        file << p.getId() << "," << p.getName() << "," << p.getAge() << ",";
        if (p.getGender())
        {
            file << "M";
        }
        else
        {
            file << "F";
        }
        file << "," << p.getContact() << "," << p.getPassword() << "," << p.getBalance() << "\n";
        file.close();
    }
};