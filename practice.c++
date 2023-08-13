#pragma once

template<typename KEY, typename VALUE>
class CBinaryTreeNode
{
    template<typename KEY, typename VALUE>
    friend class CBinaryTree;
    template<typename KEY, typename VALUE>
    friend class CBinaryTreeIterator;

    private :
        CBinaryTreeNode<KEY, VALUE>* m_Left;
        CBinaryTreeNode<KEY, VALUE>* m_Right;
        CBinaryTreeNode<KEY, VALUE>* m_Parent;
        CBinaryTreeNode<KEY, VALUE>* m_Next;
        CBinaryTreeNode<KEY, VALUE>* m_Prev;
    public :
        KEY first;
        VALUE second;
};

template<typename KEY, typename VALUE>
class CBinaryTreeIterator
{
    template<typename KEY, typename VALUE>
    friend class CBinaryTree;

    public :
    CBinaryTreeIterator() : m_Node(nullptr){}
    ~CBinaryTreeIterator() {}

private :
    CBinaryTreeNode<KEY, VALUE>* m_Node;
    public :
    bool operator == (const CBinaryTreeIterator<KEY, VALUE>& iter) const{
        return iter.m_Node == m_Node;
    }
    bool operator != (const CBinaryTreeIterator<KEY, VALUE>& iter) const{
        return iter.m_Node != m_Node;
    }
    bool operator == (const CBinaryTreeNode<KEY, VALUE>* Node) const{
        return m_Node == Node;
    }
    bool operator == (const CBinaryTreeNode<KEY, VALUE>* Node) const{
        return m_Node != Node;
    }
    void operator ++ ()
    {
        m_Node = m_Node->m_Next;
    }
    void operator ++ (int)
    {
        m_Node = m_Node->m_Next;
    }
    void operator -- ()
    {
        m_Node = m_Node->m_Prev;
    }
    void operator -- (int)
    {
        m_Node = m_Node->m_Prev;
    }
    CBinaryTreeNode<KEY, VALUE>* operator -> ()
    {
        return m_Node;
    }
};

template<typename KEY, typename VALUE>
class CBinaryTree
{
public :
    CBinaryTree()
    {
        m_Root = nullptr;
        m_Size = 0;

        m_Begin = new NODE;
        m_End = new NODE;

        m_Begin->m_Next = m_End;
        m_End->m_Prev = m_Begin;
    }
    ~CBinaryTree()
    {
        PNODE DeleteNode = m_Begin;

        while (DeleteNode)
        {
            PNODE Next = DeleteNode->m_Next;
            delete DeleteNode;
            DeleteNode = Next;
        }
    }
private:
	typedef CBinaryTreeNode<KEY, VALUE>		NODE;
	typedef CBinaryTreeNode<KEY, VALUE>* PNODE;
public:
	typedef CBinaryTreeIterator<KEY, VALUE>	iterator;
private :
    PNODE* m_Root;
    PNODE* m_Begin;
    PNODE* m_End;
    int m_Size;
    public :
    void insert(const KEY& value, const VALUE& value)
    {
        if (!m_Root)
        {
            m_Root = new NODE;
            m_Root->first = key;
            m_Root->second = value;

            m_Begin->m_Next = m_Root;
            m_Root->m_Prev = m_Begin;

            m_Root->m_Next = m_End;
            m_End->m_Prev = m_Root;
        }
        else 
        {
            insert(key, value, m_Root);
        }
        ++m_Size;
    }
    int size()	const
	{
		return m_Size;
	}
	bool empty()	const
	{
		return m_Size == 0;
	}
    void clear()
    {
        PNODE Node = m_Begin->m_Next;

        while (Node != m_End)
        {
            PNODE Next = Node->m_Next;
            delete Node;
            Node = Next;        
        }

        m_Begin->m_Next = m_End;
        m_End->m_Prev = m_Begin;
        m_Size = 0;
        m_Root = nullptr;
    }
    iterator begin() const{
        iterator iter;
        iter.m_Node = m_Begin->m_Next;
        return iter;
    }
    iterator end() const{
        iterator iter;
        iter.m_Node = m_End;
        return iter;
    }
    iterator erase (const KEY& key)
    {
        iterator iter = Find(key);
        if (iter == end())
            return iter;
        
        return erase(iter);
    }
    iterator erase(const iterator& iter)
    {
        if (iter == end())
            return iter;
        
        else if (iter == m_Begin)
            return end();

        else if (iter.m_Node == nullptr)
            return end();

        // 리프 노드일 경우, 부모로부터 연결을 제거하고, 노드를 제거해준다.
        if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
        {
            // 부모 노드를 얻어온다.
            PNODE Parent = iter.m_Node->m_Parent;

            // 부모 노드가 없다면, 현재 지우려는 노드는 루트 노드라는 것이다
            // 루트노드가 리프노드라는 의미는, 해당 노드 1개만 남았다는 것이다.
            if (!Parent)
            {
                delete iter.m_Node;
                m_Root = nullptr;
                --m_Size;

                m_Begin->m_Next = m_End;
                m_End->m_Prev = m_Begin;

                return end();
            }

            // 지우려는 노드가 부모노드의 왼쪽인지, 오른쪽인지 판단한다
            // 이후 부모와의 연결을 끊어준다.
            if (Parent->m_Left == iter.m_Node)
                Parent->m_Left = nullptr;
            else 
                Parent->m_Right = nullptr;

            PNODE Prev = iter.m_Node->m_Prev;
            PNODE Next = iter.m_Node->m_Next;

            Prev->m_Next = Next;
            Next->m_Prev = Prev;

            delete iter.m_Node;

            --m_Size;

            // 지운 노드의 다음 노드를 넣어서 iterator 반환
            iterator result;
            result.m_Node = Next;
            return result;
        }

        // 만약 왼쪽 노드가 있을 경우
        // 왼쪽 노드에서 가장 큰 노드를 찾아서
        // 지울 노드의 값을 변경해주고
        // 찾아준 노드를 제거한다.
        if (iter.m_Node->m_Left)
        {
            // 왼쪽에 존재하는 가장 큰 노드를 찾아온다.
            PNODE MaxNode = FindMax(iter.m_Node->m_Left);

            iter.m_Node->first = MaxNode->first;
            iter.m_Node->second = MaxNode->second;

            // 찾아준 노드를 제거해야 하기 때문에
            // 부모로부터 연결을 끊고 제거해주도록 한다.
            // 단, 찾아준 노드가 왼쪽 자식노드가 있을 수도 있으므로
            PNODE LeftChild = MaxNode->m_Left;
            PNODE Parent = MaxNode->m_Parent;

            if (Parent->m_Left == MaxNode)
                Parent->m_Left = LeftChild; 
            else 
                Parent->m_Right = LeftChild;

            if (LeftChild)
                LeftChild->m_Parent = Parent;

            PNODE Prev = MaxNode->m_Prev;
            PNODE Next = MaxNode->m_Next;

            Prev->m_Next = Next;
            Next->m_Prev = Prev;

            delete MaxNode;

            --m_Size;

            iterator result;
            result.m_Node = Next;

            return result;
        }

        // 지울 노드의 오른쪽 노드만 존재할 경우
        // 오른쪽 노드에서 가장 작은 노드를 찾아온다.
        PNODE MinNode = FindMin(iter.m_Node->m_Right);

        // 찾아준 노드의 key, value 값으로 변경해준다.
        iter.m_Node->first = MinNode->first;
        iter.m_Node->second = MinNode->second;

        PNODE RightChild = MinNode->m_Right;
        PNODE Parent = MinNode->m_Parent;

        if (Parent->m_Left == MinNode)
            Parent->m_Left = RightChild;
        else 
            Parent->m_Right = RightChild;

        if (RightChild)
            RightChild->m_Parent = Parent;

        PNODE Prev = MinNode->m_Prev;
        PNODE Next = MinNode->m_Next;

        Prev->m_Next = Next;
        Next->m_Prev = Prev;

        delete MinNode;

        --m_Size;

        iterator result;
        result.m_Node = Next;

        return result;
    }
    iterator Find(const KEY& key) const{
        return Find(key, m_Root);
    }
    private :
    PNODE insert(const KEY& key, const VALUE& value, PNODE Node)
    {
        // 기준 노드보다 작다면 왼쪽이다
        if (Node->first > key)
        {
            // 만약 기준 노드의 왼쪽 자식노드가 있다면, 그 왼쪽 자식노드를 기준노드로 하여
            // 다시 탐색을 하게 한다.
            if (Node->m_Left)
                return insert(key, value, Node->m_Left);

            // 더이상 자식 노드가 없을 경우, 이 위치에 새로 노드를 생성하여
            // 추가해주어야 한다.
            PNODE NewNode = new NODE;

            NewNode->fist = key;
            NewNode->second = value;

            // 기준노드의 왼쪽 자식 노드로 지정한다
            Node->m_Left = NewNode;
            NewNode->m_Parent = Node;

            PNODE Prev = Node->m_Prev;
            Prev->m_Next = NewNode;
            NewNode->m_Prev = Prev;

            NewNode->m_Next = Node;
            Node->m_Prev = NewNode;

            return NewNode;
        }

        // 여기로 내려오면, 값이 더 카다는 의미
        if (Node->m_Right)
            return insert(key, value, Node->m_Right);

        // 더이상 자식 노드가 없을 경우, 이 위치에 새로 노드를 생성하여 추가해주어야 한다.
        PNODE NewNode = new NODE;
        NewNode->first = key;
        NewNode->second = value;

        Node->m_Right = NewNode;
        NewNode->m_Parent = Node;

        PNODE Next = Node->m_Next;

        Next->m_Prev = NewNode;
        NewNode->m_Next = Next;

        Node->m_Next = Next;
        Next->m_Prev = Node;

        return NewNode;
    }

    iterator Find(const KEY& key, PNODE Node) const{
        if (!Node)
            return end();

        if (Node->first == key)
        {
            iterator iter;
            iter.m_Node = Node;
            return iter;
        }

        if (Node->fist > key)
            return Find(key, Node->m_Left);

        return Find(key, Node->m_Right);
    }

    PNODE FindMax(PNODE Node)
    {
        if (Node->m_Right)
            return FindMax(Node->m_Right);
        return Node;
    }

    PNODE FindMin(PNODE Node)
    {
        if (Node->m_Left)
            return FindMin(Node->m_Left);
        return Node;
    }
};