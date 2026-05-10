#pragma once
#include <iostream>
using namespace std;

class Validators {
public:
    bool validateId(int id) {
        if (id > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool validateDate(const char* date) {
        int len = 0;
        for (int i = 0; date[i] != '\0'; i++)
        {
            len++;
        }
        if (len != 10)
        {
            return false;
        }
        if (date[2] != '-' || date[5] != '-')
        {
            return false;
        }
        int day = (date[0] - '0') * 10 + (date[1] - '0');//char num ki ascii se minus 0 ki ascii actual number ke liye 
        int month = (date[3] - '0') * 10 + (date[4] - '0');//multiply by 10 to shift its place 
        int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');
        if (day < 1 || day > 31)
        {
            return false;
        }
        if (month < 1 || month > 12)
        {
            return false;
        }
        if (year < 2025)
        {
            return false;
        }
        return true;
    }

    bool validateTimeSlot(const char* slot) {
        const char* validSlots[8] = {
            "09:00", "10:00", "11:00", "12:00","13:00", "14:00", "15:00", "16:00"
        };
        for (int i = 0; i < 8; i++)
        {
            bool match = true;
            for (int j = 0; j < 5; j++)
            {
                if (slot[j] != validSlots[i][j])
                {
                    match =  false;
                    break;
                }
            }
            if (match)
            {
                return true;
            }
        }
        return false;
    }

    bool validateContact(const char* contact) {
        int len = 0;
        for (int i = 0; contact[i] != '\0'; i++)
        {
            if (contact[i] < '0' || contact[i] > '9')
            {
                return false;
            }
            len++;
        }
        if (len == 11)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool validatePassword(const char* password) {
        int len = 0;
        for (int i = 0; password[i] != '\0'; i++)
        {
            len++;
        }
        if (len >= 6)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool validatePositiveFloat(double amount) {
        if (amount > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool validateMenuChoice(int choice, int min, int max) {
        if (choice >= min && choice <= max)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};