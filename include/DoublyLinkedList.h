#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <typeinfo>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& data, Node* next = nullptr, Node* prev = nullptr) : data(data), next(next), prev(prev) {}
    };

    Node* head;
    Node* tail;
    int size;

    // void insertAt(int index, const T& data);
    void insertNode(Node* prev, Node* node);
    void removeNode(Node* node);
    Node* mergeSort(Node* head);
    Node* merge(Node* left, Node* right);

public:
    DoublyLinkedList();
    DoublyLinkedList(const T& data);
    DoublyLinkedList(const std::vector<T>& data);
    DoublyLinkedList(const DoublyLinkedList& other);
    DoublyLinkedList& operator=(const DoublyLinkedList& other);
    ~DoublyLinkedList();

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
    void merge(DoublyLinkedList<T>& other);
    void sortList();
    void removeDuplicates();
    void rotate(int k);
    void swapNodes(const T& x, const T& y);

    int length();
    typename DoublyLinkedList<T>::Node* getMidNode();
    typename DoublyLinkedList<T>::Node* getHead();
    typename DoublyLinkedList<T>::Node* getTail();
    typename DoublyLinkedList<T>::Node* getAt(const int index);
};

// Implementation of the class methods
template <typename T>
void DoublyLinkedList<T>::insertNode(Node* prev, Node* node) {
    if(node == nullptr) {
        throw std::out_of_range("Node is null");
    }
    if(prev == nullptr) {
        node->next = head;
        node->prev = nullptr;
        if(head != nullptr){
            head->prev = node;
        }
        head = node;
        if(tail == nullptr) {
            tail = head;
        }
    } else {
        Node* nextptr = prev->next;
        node->next = nextptr;
        node->prev = prev;
        prev->next = node;
        if(nextptr != nullptr){
            nextptr->prev = node;
        }
        if(node->next == nullptr) {
            tail = node;
        }
    }
    size++;
}

template <typename T>
void DoublyLinkedList<T>::removeNode(Node* node) {
    if(node == nullptr){
        throw std::out_of_range("Node is null");
    }
    if(node->prev == nullptr){
        // remove head
        head = head->next;
        if(head != nullptr){
            head->prev = nullptr;
        }
        // handle tail also
        if(node == tail){
            tail = nullptr;
        }
        delete node;
    }
    else{
        Node* prev = node->prev;
        Node* nextptr = node->next;
        prev->next = nextptr;
        if(nextptr != nullptr){
            nextptr->prev = prev;
        }
        delete node;
    }
    size--;
}


template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const T& data) : head(new Node(data)), tail(head), size(1) {}


template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const std::vector<T>& data) : head(nullptr), tail(nullptr), size(0) {
    for (const T& d : data) {
        append(d);
    }
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
    Node* temp = other.head;
    while (temp != nullptr) {
        append(temp->data);
        temp = temp->next;
    }
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other) {
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
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

// Static method to create a node
template <typename T>
typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::createNode(const T& data) {
    return new Node(data);
}

template<typename T>
void DoublyLinkedList<T>::printList() {
    Node* temp = head;
    while(temp != nullptr) {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

template <typename T>
void DoublyLinkedList<T>::append(const T& data) {
    insertNode(tail, new Node(data));
}

template <typename T>
void DoublyLinkedList<T>::append(Node* node) {
    insertNode(tail, node);
}

template <typename T>
void DoublyLinkedList<T>::appendAt(int index, const T& data) {
    insertNode(getAt(index - 1), new Node(data));
}

template <typename T>
void DoublyLinkedList<T>::appendAtMid(const T&data) {
    if (size == 0){
        insertNode(tail, new Node(data));
    }else{
    insertNode(getAt(size/2), new Node(data));
    }
}


template <typename T>
void DoublyLinkedList<T>::remove(const T& data) {
    Node* node = head;
    while (node != nullptr) {
        if (node->data == data) {
            removeNode(node);
            return;
        }
        node = node->next;
    }
}

template <typename T>
void DoublyLinkedList<T>::remove(Node* node) {
    removeNode(node);
}


template <typename T>
void DoublyLinkedList<T>::removeAt(int index) {
    removeNode(getAt(index));
}



template<typename T>
void DoublyLinkedList<T>::reverse(){
    Node* temp = head;
    Node* prev = nullptr;
    Node* nextptr = nullptr;
    while(temp != nullptr){
        nextptr = temp->next;
        temp->next = prev;
        temp->prev = nextptr;
        prev = temp;
        temp = nextptr;
    }
    tail = head;
    head = prev;
}

template<typename T>
typename DoublyLinkedList<T>:: Node* DoublyLinkedList<T>:: search(const T& data) {
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
void DoublyLinkedList<T>::merge(DoublyLinkedList<T>& other){
    if(head == nullptr){
        head = other.head;
        tail = other.tail;
        size = other.size;
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }
    else if(other.head != nullptr){
        tail->next = other.head;
        other.head->prev = tail;
        tail = other.tail;
        size += other.size;
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }
}


template <typename T>
typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::mergeSort(Node* head) {
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
typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::merge(Node* left, Node* right) {
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
void DoublyLinkedList<T>::sortList() {
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
void DoublyLinkedList<T>::removeDuplicates(){
    // std::unordered_map<T, bool> seen;
    std::unordered_set<T> seen;
    Node* temp = head;

    while(temp != nullptr){
        if(seen.find(temp->data) != seen.end()){
            Node* todelete = temp;
            temp = temp->next;
            removeNode(todelete);
        }
        else{
            seen.insert(temp->data);
            temp = temp->next;
        }
    }

    // free up memory
    seen.clear();
}

template<typename T>
void DoublyLinkedList<T>::rotate(int k){
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
void DoublyLinkedList<T>::swapNodes(const T& x, const T& y){
    if(x == y){
        return;
    }
    Node* prevX = nullptr;
    Node* currX = head;
    while(currX != nullptr && currX->data != x){
        prevX = currX;
        currX = currX->next;
    }

    Node* prevY = nullptr;
    Node* currY = head;
    while(currY != nullptr && currY->data != y){
        prevY = currY;
        currY = currY->next;
    }

    if(currX == nullptr || currY == nullptr){
        return;
    }

    if(prevX != nullptr){
        prevX->next = currY;
    }
    else{
        head = currY;
    }

    if(prevY != nullptr){
        prevY->next = currX;
    }
    else{
        head = currX;
    }

    Node* temp = currX->next;
    currX->next = currY->next;
    currY->next = temp;
}

template <typename T>
void DoublyLinkedList<T>::clear() {
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
int DoublyLinkedList<T>::length() {
    return size;
}

template <typename T>
typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::getHead() {
    return head;
}

template <typename T>
typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::getTail() {
    return tail;
}

template <typename T>
typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::getMidNode() {
    return getAt(size / 2);
}

template <typename T>
typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::getAt(const int index) {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of bound range");

    Node* node = head;
    for (int i = 0; i < index; ++i) {
        node = node->next;
    }

    return node;
}

#endif // SLINKEDLIST_H