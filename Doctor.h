#pragma once
#include "Person.h"
#include <iostream>
using namespace std;


class Doctor :public Person {
protected:
	double fees;

	char specialization[51];
public:
	Doctor& operator=(const Doctor& obj)
	{
		Person::operator=(obj);
		fees = obj.fees;
		for (int i = 0; i <= 50; i++)
			specialization[i] = obj.specialization[i];
		return *this;
	}
	Doctor() : Person("", "", 0, "")
	{
		fees = 0;
		specialization[0] = '\0';
	}
	Doctor(int id, const char* name, const char* spec, const char* password, const char* contact, double fees) : Person(name, password, id, contact)
	{
		int i;
		for (i = 0; i < 50 && spec[i] != '\0'; i++)
			this->specialization[i] = spec[i];
		this->specialization[i] = '\0';
		this->fees = fees;
	}
	bool operator ==(const Doctor& obj) {
		if (id == obj.id) {
			return true;

		}
		else return false;
	}
	friend ostream& operator <<(ostream& out,Doctor& obj) {

		out << "Doctor Details" << endl << "Id= " << obj.id << endl << "Specialization" << obj.specialization << endl << "Name= " << obj.name << endl << "Contact= " << obj.contact << endl << "Fees= " << obj.fees;
		return out;
	}
	void login() override {}
	void display() override {}
	double getFees() { 
		return fees;
	}
	char* getSpecialization() {
		return specialization; 
	}
};
