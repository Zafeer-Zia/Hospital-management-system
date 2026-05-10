#include <iostream>
#include "FileHandler.h"
#include "Validators.h"
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Exceptions.h"
#include <ctime>
#include <fstream>
#include "Interface.h"

using namespace std;

Storage<Patient> patients;
Storage<Doctor> doctors;
Storage<Appointment> appointments;
Storage<Bill> bills;
Storage<Prescription> prescriptions;
FileHandler fh;
Validators validator;

void get_current_date(char* date_arr_index)
{
    time_t t = time(0);
    tm* now = localtime(&t);
    int day = now->tm_mday;
    int month = now->tm_mon + 1;
    int year = now->tm_year + 1900;
    date_arr_index[0] = '0' + day / 10;
    date_arr_index[1] = '0' + day % 10;
    date_arr_index[2] = '-';
    date_arr_index[3] = '0' + month / 10;
    date_arr_index[4] = '0' + month % 10;
    date_arr_index[5] = '-';
    date_arr_index[6] = '0' + year / 1000;
    date_arr_index[7] = '0' + (year / 100) % 10;
    date_arr_index[8] = '0' + (year / 10) % 10;
    date_arr_index[9] = '0' + year % 10;
    date_arr_index[10] = '\0';
}

int char_to_int_simple(char* buf)
{
    int result = 0;
    for (int i = 0; buf[i] != '\0'; i++)
    {
        result = result * 10 + (buf[i] - '0');
    }
    return result;
}

bool match_char(const char* a, const char* b)
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
        {
            return false;
        }
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

int get_max_appointment_id()
{
    int max = 0;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].appointment_id > max)
        {
            max = appointments.data[i].appointment_id;
        }
    }
    return max;
}

int get_max_bill_id()
{
    int max = 0;
    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.data[i].bill_id > max)
        {
            max = bills.data[i].bill_id;
        }
    }
    return max;
}

int get_max_prescription_id()
{
    int max = 0;
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.data[i].prescription_id > max)
        {
            max = prescriptions.data[i].prescription_id;
        }
    }
    return max;
}

void load_all_files()
{
    try
    {
        fh.load_patients(patients);
    }
    catch (FileNotFoundException& e)
    {
        cout << e.what() << " (patients.txt)" << endl;
    }
    try
    {
        fh.load_doctors(doctors);
    }
    catch (FileNotFoundException& e)
    {
        cout << e.what() << " (doctors.txt)" << endl;
    }
    try
    {
        fh.load_appointments(appointments);
    }
    catch (FileNotFoundException& e)
    {
        cout << e.what() << " (appointments.txt)" << endl;
    }
    try
    {
        fh.load_bills(bills);
    }
    catch (FileNotFoundException& e)
    {
        cout << e.what() << " (bills.txt)" << endl;
    }
    try
    {
        fh.load_prescriptions(prescriptions);
    }
    catch (FileNotFoundException& e)
    {
        cout << e.what() << " (prescriptions.txt)" << endl;
    }
}

void book_appointment(Patient* p)
{
    char spec[51];
    cout << "Enter specialization to search: ";
    cin >> spec;

    bool found = false;
    for (int i = 0; i < doctors.size(); i++)
    {
        char doc_spec[51];
        char* ds = doctors.data[i].getSpecialization();
        int j = 0;
        while (ds[j] != '\0')
        {
            if (ds[j] >= 'A' && ds[j] <= 'Z')
            {
                doc_spec[j] = ds[j] + 32;
            }
            else
            {
                doc_spec[j] = ds[j];
            }
            j++;
        }
        doc_spec[j] = '\0';

        char input_lower[51];
        j = 0;
        while (spec[j] != '\0')
        {
            if (spec[j] >= 'A' && spec[j] <= 'Z')
            {
                input_lower[j] = spec[j] + 32;
            }
            else
            {
                input_lower[j] = spec[j];
            }
            j++;
        }
        input_lower[j] = '\0';

        if (match_char(doc_spec, input_lower))
        {
            cout << "ID= " << doctors.data[i].getId()
                << " | Name= " << doctors.data[i].getName()
                << " | Fee= PKR " << doctors.data[i].getFees() << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No doctors available for that specialization." << endl;
        return;
    }

    int doctor_id;
    cout << "Enter Doctor ID: ";
    cin >> doctor_id;

    Doctor* doc = doctors.findById(doctor_id);
    if (doc == nullptr)
    {
        cout << "Doctor not found." << endl;
        return;
    }

    char date[11];
    int attempts = 0;
    while (attempts < 3)
    {
        cout << "Enter date (DD-MM-YYYY): ";
        cin >> date;
        if (validator.validateDate(date))
        {
            break;
        }
        cout << "Invalid date. Use format DD-MM-YYYY." << endl;
        attempts++;
    }
    if (attempts == 3)
    {
        return;
    }

    const char* slots[8] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };
    cout << "Available time slots:" << endl;
    for (int i = 0; i < 8; i++)
    {
        bool taken = false;
        for (int j = 0; j < appointments.size(); j++)
        {
            if (appointments.data[j].doc_id == doctor_id &&
                match_char(appointments.data[j].date, date) &&
                match_char(appointments.data[j].timeslot, slots[i]) &&
                !match_char(appointments.data[j].status, "cancelled"))
            {
                taken = true;
                break;
            }
        }
        if (!taken)
        {
            cout << slots[i] << endl;
        }
    }

    char slot[6];
    attempts = 0;
    while (attempts < 3)
    {
        cout << "Enter time slot (e.g. 09:00): ";
        cin >> slot;
        if (!validator.validateTimeSlot(slot))
        {
            cout << "Invalid time slot." << endl;
            attempts++;
            continue;
        }
        bool taken = false;
        for (int j = 0; j < appointments.size(); j++)
        {
            if (appointments.data[j].doc_id == doctor_id &&
                match_char(appointments.data[j].date, date) &&
                match_char(appointments.data[j].timeslot, slot) &&
                !match_char(appointments.data[j].status, "cancelled"))
            {
                taken = true;
                break;
            }
        }
        if (taken)
        {
            try
            {
                throw SlotUnavailableException();
            }
            catch (SlotUnavailableException& e)
            {
                cout << e.what() << endl;
            }
            attempts++;
            continue;
        }
        break;
    }
    if (attempts == 3)
    {
        return;
    }

    if (p->getBalance() < doc->getFees())
    {
        try
        {
            throw InsufficientFundsException();
        }
        catch (InsufficientFundsException& e)
        {
            cout << e.what() << endl;
        }
        return;
    }

    *p -= doc->getFees();

    int new_appointment_id = get_max_appointment_id() + 1;
    Appointment a(new_appointment_id, doctor_id, p->getId(), date, slot, "pending");
    appointments.add(a);
    fh.append_appointment(a);

    char today[11];
    get_current_date(today);
    int new_bill_id = get_max_bill_id() + 1;
    Bill b(new_bill_id, new_appointment_id, p->getId(), today, doc->getFees(), "unpaid");
    bills.add(b);
    fh.append_bill(b);

    fh.update_patients(patients);

    cout << "Appointment booked successfully. Appointment ID: " << new_appointment_id << endl;
}

void cancel_appointment(Patient* p)
{
    bool has_pending = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].patient_id == p->getId() && match_char(appointments.data[i].status, "pending"))
        {
            Doctor* doc = doctors.findById(appointments.data[i].doc_id);
            cout << "ID= " << appointments.data[i].appointment_id
                << " | Doctor= " << (doc ? doc->getName() : "Unknown")
                << " | Date= " << appointments.data[i].date
                << " | Slot= " << appointments.data[i].timeslot << endl;
            has_pending = true;
        }
    }
    if (!has_pending)
    {
        cout << "You have no pending appointments." << endl;
        return;
    }

    int appt_id;
    cout << "Enter Appointment ID to cancel: ";
    cin >> appt_id;

    Appointment* appt = appointments.findById(appt_id);
    if (appt == nullptr || appt->patient_id != p->getId() || !match_char(appt->status, "pending"))
    {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    Doctor* doc = doctors.findById(appt->doc_id);
    double fee = doc ? doc->getFees() : 0;

    appt->status[0] = 'c';
    appt->status[1] = 'a';
    appt->status[2] = 'n';
    appt->status[3] = 'c';
    appt->status[4] = 'e';
    appt->status[5] = 'l';
    appt->status[6] = 'l';
    appt->status[7] = 'e';
    appt->status[8] = 'd';
    appt->status[9] = '\0';
    fh.update_appointments(appointments);

    *p += fee;
    fh.update_patients(patients);

    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.data[i].appointment_id == appt_id)
        {
            bills.data[i].status[0] = 'c';
            bills.data[i].status[1] = 'a';
            bills.data[i].status[2] = 'n';
            bills.data[i].status[3] = 'c';
            bills.data[i].status[4] = 'e';
            bills.data[i].status[5] = 'l';
            bills.data[i].status[6] = 'l';
            bills.data[i].status[7] = 'e';
            bills.data[i].status[8] = 'd';
            bills.data[i].status[9] = '\0';
            break;
        }
    }
    fh.update_bills(bills);

    cout << "Appointment cancelled. PKR " << fee << " refunded to your balance." << endl;
}

void view_my_appointments(Patient* p)
{
    bool found = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].patient_id == p->getId())
        {
            Doctor* doc = doctors.findById(appointments.data[i].doc_id);
            cout << "ID= " << appointments.data[i].appointment_id
                << " | Doctor= " << (doc ? doc->getName() : "Unknown")
                << " | Spec= " << (doc ? doc->getSpecialization() : "Unknown")
                << " | Date= " << appointments.data[i].date
                << " | Slot= " << appointments.data[i].timeslot
                << " | Status= " << appointments.data[i].status << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No appointments found." << endl;
    }
}

void view_my_medical_records(Patient* p)
{
    bool found = false;
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.data[i].patient_id == p->getId())
        {
            Doctor* doc = doctors.findById(prescriptions.data[i].doctor_id);
            cout << "Date= " << prescriptions.data[i].date
                << " | Doctor= " << (doc ? doc->getName() : "Unknown")
                << " | Medicines= " << prescriptions.data[i].medicines
                << " | Notes= " << prescriptions.data[i].notes << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No medical records found." << endl;
    }
}

void view_my_bills(Patient* p)
{
    bool found = false;
    double total_unpaid = 0;
    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.data[i].patient_id == p->getId())
        {
            cout << "Bill ID= " << bills.data[i].bill_id
                << " | Appointment ID= " << bills.data[i].appointment_id
                << " | Amount= PKR " << bills.data[i].amount
                << " | Status= " << bills.data[i].status
                << " | Date= " << bills.data[i].date << endl;
            if (match_char(bills.data[i].status, "unpaid"))
            {
                total_unpaid = total_unpaid + bills.data[i].amount;
            }
            found = true;
        }
    }
    if (!found)
    {
        cout << "No bills found." << endl;
    }
    else
    {
        cout << "Total outstanding unpaid: PKR " << total_unpaid << endl;
    }
}

void pay_bill(Patient* p)
{
    bool has_unpaid = false;
    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.data[i].patient_id == p->getId() && match_char(bills.data[i].status, "unpaid"))
        {
            cout << "Bill ID= " << bills.data[i].bill_id
                << " | Amount= PKR " << bills.data[i].amount
                << " | Date= " << bills.data[i].date << endl;
            has_unpaid = true;
        }
    }
    if (!has_unpaid)
    {
        cout << "No unpaid bills." << endl;
        return;
    }

    int bill_id;
    cout << "Enter Bill ID to pay: ";
    cin >> bill_id;

    Bill* bill = bills.findById(bill_id);
    if (bill == nullptr || bill->patient_id != p->getId() || !match_char(bill->status, "unpaid"))
    {
        cout << "Invalid bill ID." << endl;
        return;
    }

    if (p->getBalance() < bill->amount)
    {
        try
        {
            throw InsufficientFundsException();
        }
        catch (InsufficientFundsException& e)
        {
            cout << e.what() << endl;
        }
        return;
    }

    *p -= bill->amount;
    bill->status[0] = 'p';
    bill->status[1] = 'a';
    bill->status[2] = 'i';
    bill->status[3] = 'd';
    bill->status[4] = '\0';
    fh.update_bills(bills);
    fh.update_patients(patients);

    cout << "Bill paid successfully. Remaining balance: PKR " << p->getBalance() << endl;
}

void topup_balance(Patient* p)
{
    int attempts = 0;
    while (attempts < 3)
    {
        double amount;
        cout << "Enter amount to add (PKR): ";
        cin >> amount;
        try
        {
            if (!validator.validatePositiveFloat(amount))
            {
                throw InvalidInputException();
            }
            *p += amount;
            fh.update_patients(patients);
            cout << "Balance updated. New balance: PKR " << p->getBalance() << endl;
            return;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
        }
    }
}

void patient_menu(Patient* p)
{
    int choice = 0;
    while (true)
    {
        cout << "\nWelcome, " << p->getName() << endl;
        cout << "Balance: PKR " << p->getBalance() << endl;
        cout << "========================" << endl;
        cout << "1. Book Appointment" << endl;
        cout << "2. Cancel Appointment" << endl;
        cout << "3. View My Appointments" << endl;
        cout << "4. View My Medical Records" << endl;
        cout << "5. View My Bills" << endl;
        cout << "6. Pay Bill" << endl;
        cout << "7. Top Up Balance" << endl;
        cout << "8. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            book_appointment(p);
        }
        else if (choice == 2)
        {
            cancel_appointment(p);
        }
        else if (choice == 3)
        {
            view_my_appointments(p);
        }
        else if (choice == 4)
        {
            view_my_medical_records(p);
        }
        else if (choice == 5)
        {
            view_my_bills(p);
        }
        else if (choice == 6)
        {
            pay_bill(p);
        }
        else if (choice == 7)
        {
            topup_balance(p);
        }
        else if (choice == 8)
        {
            cout << "Logged out." << endl;
            return;
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }
}

void view_today_appointments(Doctor* doc)
{
    char today[11];
    get_current_date(today);
    bool found = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].doc_id == doc->getId() && match_char(appointments.data[i].date, today))
        {
            Patient* p = patients.findById(appointments.data[i].patient_id);
            cout << "ID= " << appointments.data[i].appointment_id
                << " | Patient= " << (p ? p->getName() : "Unknown")
                << " | Slot= " << appointments.data[i].timeslot
                << " | Status= " << appointments.data[i].status << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No appointments scheduled for today." << endl;
    }
}

void mark_completed(Doctor* doc)
{
    char today[11];
    get_current_date(today);

    bool found = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].doc_id == doc->getId() &&
            match_char(appointments.data[i].date, today) &&
            match_char(appointments.data[i].status, "pending"))
        {
            Patient* p = patients.findById(appointments.data[i].patient_id);
            cout << "ID= " << appointments.data[i].appointment_id
                << " | Patient= " << (p ? p->getName() : "Unknown")
                << " | Slot= " << appointments.data[i].timeslot << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No pending appointments today." << endl;
        return;
    }

    int appt_id;
    cout << "Enter Appointment ID: ";
    cin >> appt_id;

    Appointment* appt = appointments.findById(appt_id);
    if (appt == nullptr || appt->doc_id != doc->getId() ||
        !match_char(appt->status, "pending") || !match_char(appt->date, today))
    {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    appt->status[0] = 'c';
    appt->status[1] = 'o';
    appt->status[2] = 'm';
    appt->status[3] = 'p';
    appt->status[4] = 'l';
    appt->status[5] = 'e';
    appt->status[6] = 't';
    appt->status[7] = 'e';
    appt->status[8] = 'd';
    appt->status[9] = '\0';
    fh.update_appointments(appointments);

    cout << "Appointment marked as completed." << endl;
}

void mark_noshow(Doctor* doc)
{
    char today[11];
    get_current_date(today);

    bool found = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].doc_id == doc->getId() &&
            match_char(appointments.data[i].date, today) &&
            match_char(appointments.data[i].status, "pending"))
        {
            Patient* p = patients.findById(appointments.data[i].patient_id);
            cout << "ID= " << appointments.data[i].appointment_id
                << " | Patient= " << (p ? p->getName() : "Unknown")
                << " | Slot= " << appointments.data[i].timeslot << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No pending appointments today." << endl;
        return;
    }

    int appt_id;
    cout << "Enter Appointment ID: ";
    cin >> appt_id;

    Appointment* appt = appointments.findById(appt_id);
    if (appt == nullptr || appt->doc_id != doc->getId() ||
        !match_char(appt->status, "pending") || !match_char(appt->date, today))
    {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    appt->status[0] = 'n';
    appt->status[1] = 'o';
    appt->status[2] = '-';
    appt->status[3] = 's';
    appt->status[4] = 'h';
    appt->status[5] = 'o';
    appt->status[6] = 'w';
    appt->status[7] = '\0';
    fh.update_appointments(appointments);

    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.data[i].appointment_id == appt_id)
        {
            bills.data[i].status[0] = 'c';
            bills.data[i].status[1] = 'a';
            bills.data[i].status[2] = 'n';
            bills.data[i].status[3] = 'c';
            bills.data[i].status[4] = 'e';
            bills.data[i].status[5] = 'l';
            bills.data[i].status[6] = 'l';
            bills.data[i].status[7] = 'e';
            bills.data[i].status[8] = 'd';
            bills.data[i].status[9] = '\0';
            break;
        }
    }
    fh.update_bills(bills);

    cout << "Appointment marked as no-show." << endl;
}

void write_prescription(Doctor* doc)
{
    int appt_id;
    cout << "Enter Appointment ID: ";
    cin >> appt_id;

    Appointment* appt = appointments.findById(appt_id);
    if (appt == nullptr || appt->doc_id != doc->getId() || !match_char(appt->status, "completed"))
    {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.data[i].appointment_id == appt_id)
        {
            cout << "Prescription already written for this appointment." << endl;
            return;
        }
    }

    char medicines[500];
    char notes[301];
    cout << "Enter medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    cin.ignore();
    cin.getline(medicines, 500);

    cout << "Enter notes (max 300 chars): ";
    cin.getline(notes, 301);

    char today[11];
    get_current_date(today);
    int new_pres_id = get_max_prescription_id() + 1;
    Prescription pres(new_pres_id, appt_id, appt->patient_id, doc->getId(), today, medicines, notes);
    prescriptions.add(pres);
    fh.append_prescription(pres);

    cout << "Prescription saved." << endl;
}

void view_patient_history(Doctor* doc)
{
    int patient_id;
    cout << "Enter Patient ID: ";
    cin >> patient_id;

    Patient* p = patients.findById(patient_id);
    if (p == nullptr)
    {
        cout << "Access denied. You can only view records of your own patients." << endl;
        return;
    }

    bool has_completed = false;
    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].patient_id == patient_id &&
            appointments.data[i].doc_id == doc->getId() &&
            match_char(appointments.data[i].status, "completed"))
        {
            has_completed = true;
            break;
        }
    }
    if (!has_completed)
    {
        cout << "Access denied. You can only view records of your own patients." << endl;
        return;
    }

    bool found = false;
    for (int i = 0; i < prescriptions.size(); i++)
    {
        if (prescriptions.data[i].patient_id == patient_id && prescriptions.data[i].doctor_id == doc->getId())
        {
            cout << "Date= " << prescriptions.data[i].date
                << " | Medicines= " << prescriptions.data[i].medicines
                << " | Notes= " << prescriptions.data[i].notes << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No records found." << endl;
    }
}

void doctor_menu(Doctor* doc)
{
    int choice = 0;
    while (true)
    {
        cout << "\n Welcome, " << doc->getName() << " | Specialization: " << doc->getSpecialization() << endl;
        cout << "===============================================" << endl;
        cout << "1. View Today's Appointments" << endl;
        cout << "2. Mark Appointment Complete" << endl;
        cout << "3. Mark Appointment No-Show" << endl;
        cout << "4. Write Prescription" << endl;
        cout << "5. View Patient Medical History" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            view_today_appointments(doc);
        }
        else if (choice == 2)
        {
            mark_completed(doc);
        }
        else if (choice == 3)
        {
            mark_noshow(doc);
        }
        else if (choice == 4)
        {
            write_prescription(doc);
        }
        else if (choice == 5)
        {
            view_patient_history(doc);
        }
        else if (choice == 6)
        {
            cout << "Logged out." << endl;
            return;
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }
}

void add_doctor()
{
    char name[51], spec[51], contact[12], password[51];
    double fee;

    cout << "Enter name: ";
    cin.ignore();
    cin.getline(name, 51);

    cout << "Enter specialization: ";
    cin.getline(spec, 51);

    int attempts = 0;
    while (attempts < 3)
    {
        cout << "Enter contact (11 digits): ";
        cin >> contact;
        if (validator.validateContact(contact))
        {
            break;
        }
        cout << "Invalid contact." << endl;
        attempts++;
    }
    if (attempts == 3)
    {
        return;
    }

    attempts = 0;
    while (attempts < 3)
    {
        cout << "Enter password (min 6 chars): ";
        cin >> password;
        if (validator.validatePassword(password))
        {
            break;
        }
        cout << "Invalid password." << endl;
        attempts++;
    }
    if (attempts == 3)
    {
        return;
    }

    attempts = 0;
    while (attempts < 3)
    {
        cout << "Enter consultation fee: ";
        cin >> fee;
        if (validator.validatePositiveFloat(fee))
        {
            break;
        }
        cout << "Invalid fee." << endl;
        attempts++;
    }
    if (attempts == 3)
    {
        return;
    }

    int max_id = 0;
    for (int i = 0; i < doctors.size(); i++)
    {
        if (doctors.data[i].getId() > max_id)
        {
            max_id = doctors.data[i].getId();
        }
    }
    int new_id = max_id + 1;

    Doctor d(new_id, name, spec, password, contact, fee);
    doctors.add(d);
    fh.append_doctor(d);

    cout << "Doctor added successfully. ID: " << new_id << endl;
}

void remove_doctor()
{
    for (int i = 0; i < doctors.size(); i++)
    {
        cout << "ID= " << doctors.data[i].getId()
            << " | Name= " << doctors.data[i].getName()
            << " | Spec= " << doctors.data[i].getSpecialization()
            << " | Fee= " << doctors.data[i].getFees() << endl;
    }

    int doctor_id;
    cout << "Enter Doctor ID to remove: ";
    cin >> doctor_id;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].doc_id == doctor_id && match_char(appointments.data[i].status, "pending"))
        {
            cout << "Cannot remove doctor with pending appointments. Cancel or reassign them first." << endl;
            return;
        }
    }

    doctors.removeById(doctor_id);
    fh.update_doctors(doctors);
    cout << "Doctor removed." << endl;
}

void view_all_patients()
{
    for (int i = 0; i < patients.size(); i++)
    {
        int unpaid_count = 0;
        for (int j = 0; j < bills.size(); j++)
        {
            if (bills.data[j].patient_id == patients.data[i].getId() && match_char(bills.data[j].status, "unpaid"))
            {
                unpaid_count++;
            }
        }
        cout << "ID= " << patients.data[i].getId()
            << " | Name= " << patients.data[i].getName()
            << " | Age= " << patients.data[i].getAge()
            << " | Contact= " << patients.data[i].getContact()
            << " | Balance= PKR " << patients.data[i].getBalance()
            << " | Unpaid Bills= " << unpaid_count << endl;
    }
}

void view_all_doctors()
{
    for (int i = 0; i < doctors.size(); i++)
    {
        cout << "ID= " << doctors.data[i].getId()
            << " | Name= " << doctors.data[i].getName()
            << " | Spec= " << doctors.data[i].getSpecialization()
            << " | Contact= " << doctors.data[i].getContact()
            << " | Fee= PKR " << doctors.data[i].getFees() << endl;
    }
}

void view_all_appointments()
{
    for (int i = 0; i < appointments.size(); i++)
    {
        Patient* p = patients.findById(appointments.data[i].patient_id);
        Doctor* doc = doctors.findById(appointments.data[i].doc_id);
        cout << "ID= " << appointments.data[i].appointment_id
            << " | Patient= " << (p ? p->getName() : "Unknown")
            << " | Doctor= " << (doc ? doc->getName() : "Unknown")
            << " | Date= " << appointments.data[i].date
            << " | Slot= " << appointments.data[i].timeslot
            << " | Status= " << appointments.data[i].status << endl;
    }
}

void view_unpaid_bills()
{
    for (int i = 0; i < bills.size(); i++)
    {
        if (match_char(bills.data[i].status, "unpaid"))
        {
            Patient* p = patients.findById(bills.data[i].patient_id);
            cout << "Bill ID= " << bills.data[i].bill_id
                << " | Patient= " << (p ? p->getName() : "Unknown")
                << " | Amount= PKR " << bills.data[i].amount
                << " | Date= " << bills.data[i].date << endl;
        }
    }
}

void discharge_patient()
{
    int patient_id;
    cout << "Enter Patient ID: ";
    cin >> patient_id;

    Patient* p = patients.findById(patient_id);
    if (p == nullptr)
    {
        cout << "Patient not found." << endl;
        return;
    }

    for (int i = 0; i < bills.size(); i++)
    {
        if (bills.data[i].patient_id == patient_id && match_char(bills.data[i].status, "unpaid"))
        {
            cout << "Cannot discharge patient with unpaid bills." << endl;
            return;
        }
    }

    for (int i = 0; i < appointments.size(); i++)
    {
        if (appointments.data[i].patient_id == patient_id && match_char(appointments.data[i].status, "pending"))
        {
            cout << "Cannot discharge patient with pending appointments." << endl;
            return;
        }
    }

    fh.append_discharged(*p);
    patients.removeById(patient_id);
    fh.update_patients(patients);

    cout << "Patient discharged and archived successfully." << endl;
}

void view_security_log()
{
    ifstream file("security_log.txt");
    if (!file.is_open())
    {
        cout << "No security events logged." << endl;
        return;
    }
    char line[300];
    bool empty = true;
    while (file.getline(line, 300))
    {
        cout << line << endl;
        empty = false;
    }
    if (empty)
    {
        cout << "No security events logged." << endl;
    }
    file.close();
}

void generate_daily_report()
{
    char today[11];
    get_current_date(today);

    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
    double revenue = 0;

    for (int i = 0; i < appointments.size(); i++)
    {
        if (match_char(appointments.data[i].date, today))
        {
            total++;
            if (match_char(appointments.data[i].status, "pending"))
            {
                pending++;
            }
            else if (match_char(appointments.data[i].status, "completed"))
            {
                completed++;
            }
            else if (match_char(appointments.data[i].status, "no-show"))
            {
                noshow++;
            }
            else if (match_char(appointments.data[i].status, "cancelled"))
            {
                cancelled++;
            }
        }
    }

    for (int i = 0; i < bills.size(); i++)
    {
        if (match_char(bills.data[i].status, "paid") && match_char(bills.data[i].date, today))
        {
            revenue = revenue + bills.data[i].amount;
        }
    }

    cout << "Total appointments today: " << total
        << " (Pending: " << pending
        << " Completed: " << completed
        << " No-show: " << noshow
        << " Cancelled: " << cancelled << ")" << endl;
    cout << "Revenue collected today (paid bills): PKR " << revenue << endl;

    cout << "\nPatients with outstanding unpaid bills:" << endl;
    for (int i = 0; i < patients.size(); i++)
    {
        double owed = 0;
        for (int j = 0; j < bills.size(); j++)
        {
            if (bills.data[j].patient_id == patients.data[i].getId() && match_char(bills.data[j].status, "unpaid"))
            {
                owed = owed + bills.data[j].amount;
            }
        }
        if (owed > 0)
        {
            cout << "Patient= " << patients.data[i].getName() << " | Total Owed= PKR " << owed << endl;
        }
    }

    cout << "\nDoctor-wise summary for today:" << endl;
    for (int i = 0; i < doctors.size(); i++)
    {
        int doc_completed = 0, doc_pending = 0, doc_noshow = 0;
        for (int j = 0; j < appointments.size(); j++)
        {
            if (appointments.data[j].doc_id == doctors.data[i].getId() && match_char(appointments.data[j].date, today))
            {
                if (match_char(appointments.data[j].status, "completed"))
                {
                    doc_completed++;
                }
                else if (match_char(appointments.data[j].status, "pending"))
                {
                    doc_pending++;
                }
                else if (match_char(appointments.data[j].status, "no-show"))
                {
                    doc_noshow++;
                }
            }
        }
        cout << "Doctor= " << doctors.data[i].getName()
            << " | Completed= " << doc_completed
            << " | Pending= " << doc_pending
            << " | No-show= " << doc_noshow << endl;
    }
}

void admin_menu()
{
    int choice = 0;
    while (true)
    {
        cout << "\nAdmin Panel - MediCore" << endl;
        cout << "======================" << endl;
        cout << "1. Add Doctor" << endl;
        cout << "2. Remove Doctor" << endl;
        cout << "3. View All Patients" << endl;
        cout << "4. View All Doctors" << endl;
        cout << "5. View All Appointments" << endl;
        cout << "6. View Unpaid Bills" << endl;
        cout << "7. Discharge Patient" << endl;
        cout << "8. View Security Log" << endl;
        cout << "9. Generate Daily Report" << endl;
        cout << "10. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            add_doctor();
        }
        else if (choice == 2)
        {
            remove_doctor();
        }
        else if (choice == 3)
        {
            view_all_patients();
        }
        else if (choice == 4)
        {
            view_all_doctors();
        }
        else if (choice == 5)
        {
            view_all_appointments();
        }
        else if (choice == 6)
        {
            view_unpaid_bills();
        }
        else if (choice == 7)
        {
            discharge_patient();
        }
        else if (choice == 8)
        {
            view_security_log();
        }
        else if (choice == 9)
        {
            generate_daily_report();
        }
        else if (choice == 10)
        {
            cout << "Logged out." << endl;
            return;
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }
}

void handle_login()
{
    Admin admin(0, "Admin", "admin123");
    try
    {
        fh.load_admin(admin);
    }
    catch (FileNotFoundException& e)
    {
        cout << e.what() << " (admin.txt)" << endl;
    }

    while (true)
    {
        cout << "Welcome to Z Hospital Management System" << endl;
        cout << "===============================================" << endl;
        cout << "Login as:" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Doctor" << endl;
        cout << "3. Admin" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            int id;
            char password[51];
            int attempts = 0;
            bool logged_in = false;

            while (attempts < 3)
            {
                cout << "Enter Patient ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> password;

                Patient* p = patients.findById(id);
                if (p != nullptr && match_char(p->getPassword(), password))
                {
                    logged_in = true;
                    patient_menu(p);
                    break;
                }
                else
                {
                    cout << "Invalid ID or password." << endl;
                    attempts++;
                    char today[11];
                    get_current_date(today);
                    char id_str[10];
                    int temp = id, idx = 0;
                    if (temp == 0)
                    {
                        id_str[idx++] = '0';
                    }
                    else
                    {
                        char rev[10];
                        int rev_idx = 0;
                        while (temp > 0)
                        {
                            rev[rev_idx++] = '0' + temp % 10;
                            temp /= 10;
                        }
                        for (int k = rev_idx - 1; k >= 0; k--)
                        {
                            id_str[idx++] = rev[k];
                        }
                    }
                    id_str[idx] = '\0';
                    fh.log_security(today, "Patient", id_str, "FAILED");
                }
            }
            if (!logged_in && attempts == 3)
            {
                cout << "Account locked. Contact admin." << endl;
            }
        }
        else if (choice == 2)
        {
            int id;
            char password[51];
            int attempts = 0;
            bool logged_in = false;

            while (attempts < 3)
            {
                cout << "Enter Doctor ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> password;

                Doctor* doc = doctors.findById(id);
                if (doc != nullptr && match_char(doc->getPassword(), password))
                {
                    logged_in = true;
                    doctor_menu(doc);
                    break;
                }
                else
                {
                    cout << "Invalid ID or password." << endl;
                    attempts++;
                    char today[11];
                    get_current_date(today);
                    char id_str[10];
                    int temp = id, idx = 0;
                    char rev[10];
                    int rev_idx = 0;
                    while (temp > 0)
                    {
                        rev[rev_idx++] = '0' + temp % 10;
                        temp /= 10;
                    }
                    for (int k = rev_idx - 1; k >= 0; k--)
                    {
                        id_str[idx++] = rev[k];
                    }
                    id_str[idx] = '\0';
                    fh.log_security(today, "Doctor", id_str, "FAILED");
                }
            }
            if (!logged_in && attempts == 3)
            {
                cout << "Account locked. Contact admin." << endl;
            }
        }
        else if (choice == 3)
        {
            int id;
            char password[51];
            int attempts = 0;
            bool logged_in = false;

            while (attempts < 3)
            {
                cout << "Enter Admin ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> password;

                if (admin.getId() == id && match_char(admin.getPassword(), password))
                {
                    logged_in = true;
                    admin_menu();
                    break;
                }
                else
                {
                    cout << "Invalid ID or password." << endl;
                    attempts++;
                    char today[11];
                    get_current_date(today);
                    char id_str[10];
                    int temp = id, idx = 0;
                    char rev[10];
                    int rev_idx = 0;
                    while (temp > 0)
                    {
                        rev[rev_idx++] = '0' + temp % 10;
                        temp /= 10;
                    }
                    for (int k = rev_idx - 1; k >= 0; k--)
                    {
                        id_str[idx++] = rev[k];
                    }
                    id_str[idx] = '\0';
                    fh.log_security(today, "Admin", id_str, "FAILED");
                }
            }
            if (!logged_in && attempts == 3)
            {
                cout << "Account locked. Contact admin." << endl;
            }
        }
        else if (choice == 4)
        {
            cout << "Goodbye!" << endl;
            return;
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }
}

void create_files_if_not_exist()
{
    const char* files[6] = {
        "patients.txt", "doctors.txt", "appointments.txt",
        "bills.txt", "prescriptions.txt", "security_log.txt"
    };
    for (int i = 0; i < 6; i++)
    {
        ifstream check(files[i]);
        if (!check.is_open())
        {
            ofstream create(files[i]);
            create.close();
        }
        else
        {
            check.close();
        }
    }
    ifstream check("admin.txt");
    if (!check.is_open())
    {
        ofstream create("admin.txt");
        create << "1,Admin,admin123\n";
        create.close();
    }
    else
    {
        check.close();
    }
}

int main()
{
    create_files_if_not_exist();
    load_all_files();
    ui();
    return 0;
}