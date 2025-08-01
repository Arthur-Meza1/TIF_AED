#ifndef QUEUE_H
#define QUEUE_H

#include "MyVector.hpp"
#include <utility> 
#include <limits>
#include <stdexcept> 

template <typename T> 
class PriorityQueue {
private:
   
    MyVector<T>data;

    bool less(int i, int j) const {
        if (i >= data.size() || j >= data.size()) {
            return false;
        }
        return data[i].first < data[j].first;
    }

    void swap(int i, int j) {
        T temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }

    void siftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (less(index, parent)) {
                swap(index, parent);
                index = parent;
            } else {
                break;
            }
        }
    }

    void siftDown(int index) {
        while (2 * index + 1 < data.size()) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = left;

            if (right < data.size() && less(right, left)) {
                smallest = right;
            }

            if (less(smallest, index)) {
                swap(index, smallest);
                index = smallest;
            } else {
                break;
            }
        }
    }


public:
    PriorityQueue() = default;

    bool empty() const {
        return data.empty();
    }

    bool full() const {
        return data.size();
    }


    void push(const T& value) {
        data.push_back(value);
        siftUp(data.size() - 1);
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Pop en cola de prioridad vacia.");
        }
        data[0] = data.back();
        data.pop_back();
        if (!empty()) {
            siftDown(0);
        }
    }

    T& top() {
        if (empty()) {
            throw std::out_of_range("Top en cola de prioridad vacia.");
        }
        return data[0];
    }

    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Top en cola de prioridad vacia.");
        }
        return data[0];
    }

    void clear() {
        data.clear();
    }
};

#endif // PRIORITY_QUEUE_H
