#include <iostream>
using namespace std;

int get_arr_len(const char* str)
{

    int count = 0;
    while (str[count] != '\0')
    {
        count++;
    }

    return count;
}


bool is_empty(const char* s)
{
    return s == nullptr || s[0] == '\0';
}
bool checkPassword(const char* password, const char* other)
{
    if (password == nullptr || other == nullptr)
        return false;

    int len = get_arr_len(password);

    if (len != get_arr_len(other))
        return false;

    for (int i = 0; i < len; i++)
    {
        if (password[i] != other[i])
        {
            cout << "Passwords do not match." << endl;
            return false;
        }
    }

    return true;
}