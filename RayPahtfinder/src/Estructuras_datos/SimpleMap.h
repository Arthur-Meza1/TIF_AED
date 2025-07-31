#pragma once

template <typename K, typename V>
class SimpleMap {
private:
    struct Pair {
        K key;
        V value;
    };

    Pair* data;
    int capacity;
    int count;

    void resize() {
        int newCapacity = capacity * 2;
        Pair* newData = new Pair[newCapacity];
        for (int i = 0; i < count; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    SimpleMap(int initialCap = 10) : capacity(initialCap), count(0) {
        data = new Pair[capacity];
    }

    ~SimpleMap() {
        delete[] data;
    }

    void insert(const K& key, const V& value) {
        for (int i = 0; i < count; ++i) {
            if (data[i].key == key) {
                data[i].value = value;
                return;
            }
        }
        if (count == capacity) resize();
        data[count++] = {key, value};
    }

    // Nuevo: reemplazo directo (alias de insert)
    void set(const K& key, const V& value) {
        insert(key, value);
    }

    // Nuevo: limpiar el mapa
    void clear() {
        count = 0;
    }

    bool contains(const K& key) const {
        for (int i = 0; i < count; ++i) {
            if (data[i].key == key) return true;
        }
        return false;
    }

    V get(const K& key) const {
        for (int i = 0; i < count; ++i) {
            if (data[i].key == key) return data[i].value;
        }
        return V();  // o lanza excepción si prefieres
    }

    // Opcional: acceso como en std::map
    V& operator[](const K& key) {
        for (int i = 0; i < count; ++i) {
            if (data[i].key == key) return data[i].value;
        }
        // Si no existe, lo crea con valor por defecto
        if (count == capacity) resize();
        data[count++] = {key, V()};
        return data[count - 1].value;
    }

    // Versión const del operador[]
    const V& operator[](const K& key) const {
        for (int i = 0; i < count; ++i) {
            if (data[i].key == key) return data[i].value;
        }
        static V defaultValue = V();
        return defaultValue;
    }
};
