#pragma once

template <typename T>
class Storage {
public:
    T data[100];
    int count;

    Storage() {
        count = 0;
    }

    void add(T obj) {
        if (count < 100) {
            data[count] = obj;
            count = count + 1;
        }
    }

    void removeById(int id)
    {
        for (int i = 0; i < count; i++)
        {
            if (data[i].getId() == id)
            {
                for (int j = i; j < count - 1; j++)
                {
                    data[j] = data[j + 1];
                }
                count = count - 1;
                return;
            }
        }
    }

    T* findById(int id)
    {
        for (int i = 0; i < count; i++)
        {
           
            if (data[i].getId() == id)
            {
                
                return &data[i];
            }
        }
        return nullptr;
    }

    T* getAll() {
        return data;
    }

    int size() {
        return count;
    }
};