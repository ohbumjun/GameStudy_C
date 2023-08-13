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
    head = newNode; // ���ʿ� ���� �Ҵ�, Head �� ��� �������� 
}

template <class T>
typename StackLinkedList<T>::Node* StackLinkedList<T>::pop() {
    Node* top = head;
    head = head->next; // ���ʿ��� �̾Ƽ� ���, Head �� ��� ��������
    return top;
}

#endif /* STACKLINKEDLIST_H */