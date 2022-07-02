#ifndef STACKLINKEDLIST_H
#define STACKLINKEDLIST_H

template<typename T>
class StackLinkedList
{
public:
    struct Node {
        T data;
        Node* next;
    };

    Node* head;
public:
    StackLinkedList() = default;
    StackLinkedList(StackLinkedList & stackLinkedList) = delete;
    void push(Node * newNode);
    Node* pop();
};
};

template <class T>
void StackLinkedList<T>::push(Node* newNode) {
    newNode->next = head;
    head = newNode; // 앞쪽에 새로 할당, Head 를 계속 앞쪽으로 
}

template <class T>
typename StackLinkedList<T>::Node* StackLinkedList<T>::pop() {
    Node* top = head;
    head = head->next; // 앞쪽에서 뽑아서 사용, Head 를 계속 뒤쪽으로
    return top;
}

#endif /* STACKLINKEDLIST_H */