#pragma once
#include "Person.h"
#include <iostream>
using namespace std;

class Patient :public Person {
protected:
	int age;
	bool gender;

public:
	Patient& operator=(const Patient& obj)
	{
		Person::operator=(obj);
		age = obj.age;
		gender = obj.gender;
		balance = obj.balance;
		return *this;
	}
	double balance;
	Patient() : Person("", "", 0, "")
	{
		age = 0;
		gender = false;
		balance = 0;
	}
	Patient(int id, const char* name, int age, bool gender, const char* password, const char* contact, double balance) :Person(name, password, id, contact) {
		this->age = age;
		this->gender = gender;
		this->balance = balance;
	}
	Patient& operator +=(double amount) {
		balance = balance + amount;
		return *this;
	}
	Patient& operator -=(double amount) {
		balance = balance - amount;
		if (balance < 0) {
			balance = 0;
		}
		return *this;
	}
	bool operator ==(const Patient& obj) {
		if (id == obj.id) {
			return true;

		}
		else return false;
	}
	friend ostream& operator <<(ostream& out, Patient& obj) {

		out << "Patient Details" << endl << "Id= " << obj.id << endl << "Gender= " << obj.gender << endl << "Name= " << obj.name << endl << "Contact= " << obj.contact << endl << "Balance= " << obj.balance;
		return out;
	}
	void login() override {}
	void display() override {}
	int getAge() { //for the filehander to access  these
		return age;
	}
	bool getGender() {
		return gender;
	}
	double getBalance() {
		return balance;
	}
};