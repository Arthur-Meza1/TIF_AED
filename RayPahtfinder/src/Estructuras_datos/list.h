#pragma once

template <typename T>
class SimpleList {
private:
    struct Node {
        T data;
        Node* sgte;
        Node(const T& v) : data(v), sgte(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    SimpleList() : head(nullptr), tail(nullptr) {}

    ~SimpleList() {
        clear();
    }

    void push_back(const T& v) {
        Node* newNode = new Node(v);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->sgte = newNode;
            tail = newNode;
        }
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* tmp = current;
            current = current->sgte;
            delete tmp;
        }
        head = tail = nullptr;
    }

    bool empty() const {
        return head == nullptr;
    }

    class Iterator {
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        T& operator*() const {
            return current->data;
        }

        Iterator& operator++() {
            if (current) current = current->sgte;
            return *this;
        }
    };

    Iterator begin() const {
        return Iterator(head);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
};
