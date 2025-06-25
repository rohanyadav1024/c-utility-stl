#ifndef SLINKEDLIST_H
#define SLINKEDLIST_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <typeinfo>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& data, Node* next = nullptr) : data(data), next(next) {}
    };

    Node* head;
    Node* tail;
    int size;

    // void insertAt(int index, const T& data);
    void insertNode(Node* prev, Node* node);
    void removeNode(Node* prev, Node* node);
    Node* mergeSort(Node* head);
    Node* merge(Node* left, Node* right);

public:
    LinkedList();
    LinkedList(const T& data);
    LinkedList(const std::vector<T>& data);
    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);
    ~LinkedList();

    static Node* createNode(const T& data);

    void printList();
    void append(const T& data);
    void append(Node* node);
    void appendAt(int index, const T& data);
    void appendAtMid(const T& data);
    void remove(const T& data);
    void remove(Node* node);
    void removeAt(int index);
    void reverse();
    void clear();
    Node* search(const T& data);
    void merge(LinkedList<T>& other);
    void sortList();
    void removeDuplicates();
    void rotate(int k);
    void swapNodes(const T& x, const T& y);

    int length();
    typename LinkedList<T>::Node* getMidNode();
    typename LinkedList<T>::Node* getHead();
    typename LinkedList<T>::Node* getTail();
    typename LinkedList<T>::Node* getAt(const int index);
};

// Implementation of the class methods
template <typename T>
void LinkedList<T>::insertNode(Node* prev, Node* node) {
    if(node == nullptr) {
        throw std::out_of_range("Node is null");
    }
    if(prev == nullptr) {
        node->next = head;
        head = node;
        if(tail == nullptr) {
            tail = head;
        }
    } else {
        node->next = prev->next;
        prev->next = node;
        if(node->next == nullptr) {
            tail = node;
        }
    }
    size++;
}

template <typename T>
void LinkedList<T>::removeNode(Node* prev, Node* node) {
    if(node == nullptr){
        throw std::out_of_range("Node is null");
    }
    if(prev == nullptr){
        // remove head
        head = head->next;
        delete node;
    }
    else{
        prev->next = node->next;
        delete node;
    }
    size--;
}


template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
LinkedList<T>::LinkedList(const T& data) : head(new Node(data)), tail(head), size(1) {}


template <typename T>
LinkedList<T>::LinkedList(const std::vector<T>& data) : head(nullptr), tail(nullptr), size(0) {
    for (const T& d : data) {
        append(d);
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size(0) {
    Node* temp = other.head;
    while (temp != nullptr) {
        append(temp->data);
        temp = temp->next;
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this == &other) {
        return *this;
    }
    clear();
    Node* temp = other.head;
    while (temp != nullptr) {
        append(temp->data);
        temp = temp->next;
    }
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// Static method to create a node
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::createNode(const T& data) {
    return new Node(data);
}

template<typename T>
void LinkedList<T>::printList() {
    Node* temp = head;
    while(temp != nullptr) {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

template <typename T>
void LinkedList<T>::append(const T& data) {
    insertNode(tail, new Node(data));
}

template <typename T>
void LinkedList<T>::append(Node* node) {
    insertNode(tail, node);
}

template <typename T>
void LinkedList<T>::appendAt(int index, const T& data) {
    insertNode(getAt(index - 1), new Node(data));
}

template <typename T>
void LinkedList<T>::appendAtMid(const T&data) {
    if(size == 0){
        insertNode(tail, new Node(data));
    }
    else{
    insertNode(getAt(size/2), new Node(data));
    }
}


template <typename T>
void LinkedList<T>::remove(const T& data) {
    Node* node = head, *prev = nullptr;
    while (node != nullptr) {
        if (node->data == data) {
            removeNode(prev, node);
            return;
        }
        prev = node;
        node = node->next;
    }
}

template <typename T>
void LinkedList<T>::remove(Node* node) {
    Node* prev = head;
    while (prev != nullptr) {
        if (prev->next == node) {
            removeNode(prev, node);
            return;
        }
        prev = prev->next;
    }
}


template <typename T>
void LinkedList<T>::removeAt(int index) {
    Node* prev = getAt(index - 1);
    removeNode(prev, prev->next);
}

template <typename T>
void LinkedList<T>::reverse() {
    if (head == nullptr) return;

    Node* temp = head;
    Node* prev = nullptr;
    while (temp != nullptr) {
        Node *nextptr = temp->next;
        temp->next = prev;
        prev = temp;
        temp = nextptr;
    }
    tail = head;
    head = prev;
}

template<typename T>
typename LinkedList<T>:: Node* LinkedList<T>:: search(const T& data) {
    Node* temp = head;
    while (temp != nullptr && index < size) {
        if (temp->data == data) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}


template<typename T>
void LinkedList<T>::merge(LinkedList<T>& other){
    if(head == nullptr){
        head = other.head;
        tail = other.tail;
        size = other.size;
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
        return;
    }

    else{
        tail->next = other.head;
        tail = other.tail;
        size += other.size;
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }
}


template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::mergeSort(Node* head) {
    // Base case: if head is nullptr or only one element, return head
    if (!head || !head->next) {
        return head;
    }

    // Split the list into two halves
    Node* slow = head;
    Node* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node* mid = slow->next;
    slow->next = nullptr;

    // Recursively sort the two halves
    Node* left = mergeSort(head);
    Node* right = mergeSort(mid);

    // Merge the two sorted halves
    return merge(left, right);
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::merge(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    Node* mergedHead = nullptr;

    if (left->data < right->data) {
        mergedHead = left;
        mergedHead->next = merge(left->next, right);
    } else {
        mergedHead = right;
        mergedHead->next = merge(left, right->next);
    }

    return mergedHead;
}

template <typename T>
void LinkedList<T>::sortList() {
    head = mergeSort(head);

    // Update tail after sorting
    if (head == nullptr) {
        tail = nullptr;
    } else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        tail = temp;
    }
}


template<typename T>
void LinkedList<T>::removeDuplicates(){
    // std::unordered_map<T, bool> seen;
    std::unordered_set<T> seen;
    Node* temp = head, *prev = nullptr;

    while(temp != nullptr){
        if(seen.find(temp->data) != seen.end()){
            Node* todelete = temp;
            temp = prev;
            removeNode(prev, todelete);
            temp = prev->next;
        }
        else{
            // seen[temp->data]  = true;
            seen.insert(temp->data);
            prev = temp;
            temp = temp->next;
        }
    }

    // free up memory
    seen.clear();
}

template<typename T>
void LinkedList<T>::rotate(int k){
    if(k == size || k == 0){
        return;
    }
    Node* prev = getAt(size - k - 1);
    Node* newHead = prev->next;
    prev->next = nullptr;
    tail->next = head;
    head = newHead;
    tail = prev;
}

template<typename T>
void LinkedList<T>::swapNodes(const T& x, const T& y){
    if(x == y){
        return;
    }

    Node* node1 = nullptr, *node2 = nullptr;
    Node* prev1 = nullptr, *prev2 = nullptr;
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data == x) {
            node1 = temp;
            break;
        }
        prev1 = temp;
        temp = temp->next;
    }
    temp = head;
    while (temp != nullptr) {
        if (temp->data == y) {
            node2 = temp;
            break;
        }
        prev2 = temp;
        temp = temp->next;
    }
    
    if(node1 == nullptr || node2 == nullptr){
        return;
    }
    Node* nextprt = node2->next;
    if(prev1 == nullptr){
        head = node2;
    }
    else{
        prev1->next = node2;
    }
    if(prev2 == nullptr){
        head = node1;
    }
    else{
        prev2->next = node1;
    }

    Node* temp1 = node1->next;
    node1->next = node2->next;
    node2->next = temp1;
}

template <typename T>
void LinkedList<T>::clear() {
    Node* temp = head;
    while (temp != nullptr) {
        Node* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

template <typename T>
int LinkedList<T>::length() {
    return size;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::getHead() {
    return head;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::getTail() {
    return tail;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::getMidNode() {
    return getAt(size / 2);
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::getAt(const int index) {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of bound range");

    Node* node = head;
    for (int i = 0; i < index; ++i) {
        node = node->next;
    }

    return node;
}

#endif // SLINKEDLIST_H