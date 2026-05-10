#pragma once
class Person
{
protected:
    char name[51];
    char password[51];
    int id;
    char contact[12];

    Person(const char* name, const char* password, int id, const char* contact)
    {
        int i;
        for (i = 0; i < 50 && name[i] != '\0'; i++)
            this->name[i] = name[i];
        this->name[i] = '\0';
        for (i = 0; i < 50 && password[i] != '\0'; i++)
            this->password[i] = password[i];
        this->password[i] = '\0';
        this->id = id;
        for (i = 0; i < 11 && contact[i] != '\0'; i++)
            this->contact[i] = contact[i];
        this->contact[i] = '\0';
    }

    Person(const char* name, const char* password, int id)
    {
        int i;
        for (i = 0; i < 50 && name[i] != '\0'; i++)
            this->name[i] = name[i];
        this->name[i] = '\0';
        for (i = 0; i < 50 && password[i] != '\0'; i++)
            this->password[i] = password[i];
        this->password[i] = '\0';
        this->id = id;
        this->contact[0] = '\0';
    }

    virtual void login() = 0;
    virtual void display() = 0;
    virtual ~Person() {}

public:
    int getId() {
        return id;
    }
    char* getName() 
    { 
        return name;
    }
    char* getContact() {
        return contact; 
    }
    char* getPassword() { 
        return password; 
    }

    Person(const Person& obj) : id(obj.id)
    {
        for (int i = 0; i < 50; i++)
            name[i] = obj.name[i];
        name[50] = '\0';
        for (int i = 0; i < 50; i++)
            password[i] = obj.password[i];
        password[50] = '\0';
        for (int i = 0; i < 11; i++)
            contact[i] = obj.contact[i];
        contact[11] = '\0';
    }

    Person& operator=(const Person& obj)
    {
        id = obj.id;
        for (int i = 0; i < 50; i++)
            name[i] = obj.name[i];
        name[50] = '\0';
        for (int i = 0; i < 50; i++)
            password[i] = obj.password[i];
        password[50] = '\0';
        for (int i = 0; i < 11; i++)
            contact[i] = obj.contact[i];
        contact[11] = '\0';
        return *this;
    }
};