#pragma once
#include "MyVector.hpp"
#include "pair.hpp"
#include "list.hpp"
#include <functional>
#include <string>
#include <utility>
#include <stdexcept>

template <typename K, typename V>
class SimpleMap {
private:
  struct Pair {
    K key;
    V value;
  };

  MyVector<Pair> data;
  size_t count = 0;
  size_t capacity = 8;
  const double LOAD_FACTOR = 0.75;

  void resize() {
    size_t newCapacity = capacity * 2;
    MyVector<Pair> newData;
    newData.resize(newCapacity);

    for (size_t i = 0; i < count; ++i) {
      newData[i] = std::move(data[i]); 
    }
    
    data = std::move(newData);
    capacity = newCapacity;
  }

public:
  SimpleMap() { data.resize(capacity); }

  using iterator = typename MyVector<Pair>::iterator;
  using const_iterator = typename MyVector<Pair>::const_iterator;
  
  iterator begin() { return data.begin(); }
  iterator end() { return data.begin() + count; }
  const_iterator begin() const { return data.begin(); }
  const_iterator end() const { return data.begin() + count; }

  iterator find(const K& key) {
    for (iterator it = data.begin(); it != data.begin() + count; ++it) {
      if (it->key == key) {
        return it;
      }
    }
    return end();
  }

  const_iterator find(const K& key) const {
    for (const_iterator it = data.begin(); it != data.begin() + count; ++it) {
      if (it->key == key) {
        return it;
      }
    }
    return end();
  }

  // Método insert() mejorado
  void insert(const K& key, V value) {
    if (find(key) == end()) {
      if ((double)count / capacity >= LOAD_FACTOR) {
        resize();
      }
      data[count].key = key;
      data[count].value = std::move(value);
      count++;
    } else {
        // Opcional: manejar el caso de que la clave ya exista,
        // por ejemplo, lanzando una excepción o no haciendo nada.
        // En esta implementación, simplemente no se inserta.
    }
  }

  // operador [] mejorado para crear un nuevo par si la clave no existe
  V& operator[](const K& key) {
    iterator it = find(key);
    if (it != end()) {
      return it->value;
    }
    
    // Si no se encuentra, inserta un nuevo par con un valor por defecto
    if ((double)count / capacity >= LOAD_FACTOR) {
      resize();
    }
    data[count].key = key;
    data[count].value = V(); // V() llama al constructor por defecto de V
    count++;
    return data[count - 1].value;
  }

  // get() no constante que usa find()
  V& get(const K& key) {
    iterator it = find(key);
    if (it != end()) {
      return it->value;
    }
    throw std::out_of_range("Key not found");
  }

  // get() constante que usa find()
  const V& get(const K& key) const {
    const_iterator it = find(key);
    if (it != end()) {
      return it->value;
    }
    throw std::out_of_range("Key not found");
  } 
  
  // set() mejorado que usa find()
  void set(const K& key, V value) {
    iterator it = find(key);
    if (it != end()) {
      it->value = std::move(value);
    } else {
      insert(key, std::move(value));
    }
  }

  // contains() mejorado que usa find()
  bool contains(const K& key) const {
    return find(key) != end();
  }

  size_t size() const { return count; }
};