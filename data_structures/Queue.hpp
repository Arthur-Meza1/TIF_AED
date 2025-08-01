// src/Estructuras_datos/queue.h
#pragma once

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* frontNode;
    Node* backNode;
    int count;

public:
    Queue() : frontNode(nullptr), backNode(nullptr), count(0) {}

    ~Queue() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            frontNode = backNode = newNode;
        } else {
            backNode->next = newNode;
            backNode = newNode;
        }
        ++count;
    }

    void pop() {
        if (empty()) return;
        Node* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
        --count;
        if (frontNode == nullptr) backNode = nullptr;
    }

    T& front() {
        return frontNode->data;
    }

    const T& front() const {
        return frontNode->data;
    }

    bool empty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }
};
