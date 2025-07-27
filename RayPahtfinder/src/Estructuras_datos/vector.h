#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

template <typename T>
class Vector {
private:
    T* data;
    int tam;
    int capacidad;

    void resize(int n_capacidad) {
        T* data_nueva = new T[n_capacidad];
        for (int i = 0; i < tam; i++)
            data_nueva[i] = data[i];
        delete[] data;
        data = data_nueva;
        capacidad = n_capacidad;
    }

public:
    Vector(int n_tam, const T& value) {
        tam = n_tam;
        capacidad = n_tam;
        data = new T[capacidad];
        for (int i = 0; i < tam; i++)
            data[i] = value;
    }

    Vector() {
        tam = 0;
        capacidad = 0;
        data = nullptr;
    }

    void ren_tam(int n_tam, const T& value) {
        if (n_tam > capacidad)
            resize(n_tam);

        for (int i = tam; i < n_tam; i++)
            data[i] = value;

        tam = n_tam;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int n_tam() const {
        return tam;
    }

    ~Vector() {
        delete[] data;
    }
};

#endif // VECTOR_H
