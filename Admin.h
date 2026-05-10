#pragma once
#include "Person.h"

class Admin:public Person  {

public:
	Admin(int id,const char*name,const char*password) : Person( name, password, id) {

	}
	void login() override {}
	void display() override {}
};