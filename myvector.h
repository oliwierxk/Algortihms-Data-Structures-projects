//
// Created by Oliwier Komorowski on 03/04/2024.
//

#ifndef ONP1_0_MYVECTOR_H
#define ONP1_0_MYVECTOR_H
#include <iostream>

template<typename T>
class MyVector {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* head;
    size_t sz;

public:
    MyVector() : head(nullptr), sz(0) {}

    void push_back(const T& val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        sz++;
    }

    void pop_back() {
        if (head) {
            if (head->next) {
                Node* temp = head;
                while (temp->next->next) {
                    temp = temp->next;
                }
                delete temp->next;
                temp->next = nullptr;
            } else {
                delete head;
                head = nullptr;
            }
            sz--;
        }
    }

    Node* begin() const {
        return head;
    }

    Node* end() const {
        if (!head) {
            return nullptr;
        }
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        return temp->next;
    }

    const T& back() const {
        if (!head) {
            throw std::out_of_range("Vector is empty");
        }
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        return temp->data;
    }

    bool empty() const {
        return sz == 0;
    }


    size_t size() const {
        return sz;
    }

    const T& operator[](size_t index) const {
        Node* temp = head;
        for (size_t i = 0; i < index; ++i) {
            temp = temp->next;
        }
        return temp->data;
    }

    ~MyVector() {
       /* while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }*/
    }
};



#endif //ONP1_0_MYVECTOR_H
