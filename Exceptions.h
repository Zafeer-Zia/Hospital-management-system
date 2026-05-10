#pragma once

class HospitalException {
protected:
    char message[200];
public:
    HospitalException(const char* msg) {
        int i = 0;
        for (i = 0; i < 199 && msg[i] != '\0'; i++)
            this->message[i] = msg[i];
        this->message[i] = '\0';
    }
    virtual const char* what() {
        return message;
    }
    virtual ~HospitalException() {
    }
};

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException() : HospitalException("Error: Required file could not be opened.") {
    }
};

class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException() : HospitalException("Error: Insufficient balance.") {
    }
};

class InvalidInputException : public HospitalException {
public:
    InvalidInputException() : HospitalException("Error: Invalid input.") {
    }
};

class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException() : HospitalException("Error: Time slot is already taken.") {
    }
};