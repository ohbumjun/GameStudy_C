
template<typename T>
class SinglyLinkedList
{
public :
    struct Node{
        T data;
        Node* next;
    };
    Node* head;
public :
    SinglyLinkedList();
    void insert(Node* prevNode, Node* newNode);
    void remove(Node* prevNode, Node* deleteNode);
};

template <class T>
SinglyLinkedList<T>::SinglyLinkedList(){
    
}

template <class T>
void SinglyLinkedList<T>::insert(Node* previousNode, Node* newNode)
{
    if (previousNode == nullptr)
    {
        // Is The First Node
        if (head != nullptr)
        {
            newNode->next = head;
        }
        else 
        {
            newNode->next = nullptr;
        }
        // 앞쪽에 삽입
        head = newNode;
    }
    else 
    {
        // 앞쪽 prevNode가 존재한다면, next Node 를 현재로 세팅해야 하는데
        if (previousNode->next == nullptr)
        {
            // Is The Last Node
            previousNode->next = newNode;
            newNode->next = nullptr;
        }
        else 
        {
            // 중간에 들어가게 하기
            // Is a Middle Node
            newNode->next = previousNode->next;
            previousNode->next = newNode;
        }
    }
}

template <class T>
void SinglyLinkedList<T>::remove(Node* previousNode, Node* deleteNode)
{
    if (previousNode == nullptr)
    {
        // Is The First Node
        if (deleteNode->next == nullptr)
        {
            // List Only Has One Element
            head = nullptr;
        }
        else 
        {
            // List Had More Elements (앞에 있는 것을 빼온다.)
            head = deleteNode->next;
        }
    }
    else 
    {
        previousNode->next = deleteNode->next;
    }
}