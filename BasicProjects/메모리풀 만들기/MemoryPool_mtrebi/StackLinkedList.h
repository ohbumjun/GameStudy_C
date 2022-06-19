#pragma once

template<typename T>
class StackLinkedList {
public :
	struct Node {
		T data;
		Node* Next;
	};
	Node* head;
public :
	StackLinkedList() = default;
	StackLinkedList(StackLinkedList& list) = delete;
	void push(Node* newNode);
	Node* pop();
};


template <typename T>
void StackLinkedList<T>::push(Node* newNode)
{
	newNode->Next = head;
	head = newNode;
}

template <typename T>
typename StackLinkedList<T>::Node* StackLinkedList<T>::pop()
{
	Node* PopNode = head;
	head = head->Next;
	return PopNode;
}
