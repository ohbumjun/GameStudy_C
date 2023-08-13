#pragma once

template<typename T>
class CTreeNode {
	template<typename T>
	friend class CTree;
private :
	T Data;
	CTreeNode<T>* Next;
	CTreeNode<T>* Prev;
	CTreeNode<T>* Left;
	CTreeNode<T>* Right;
	CTreeNode<T>* Parent;
};

template<typename T>
class CTree {
	typedef CTreeNode<T> NODE;
	typedef CTreeNode<T>* PNODE;
public :
	CTree()
{
	m_Begin = new NODE;
	m_End = new NODE;

	m_Begin->Next = m_End;
	m_End->Prev = m_Begin;

	m_Root = nullptr;
	m_Size = 0;
}
	~CTree()
{
	PNODE DeleteNode = m_Begin;

	while (DeleteNode)
	{
		PNODE Next = DeleteNode->Next;
		delete DeleteNode;
		DeleteNode = Next;
	}

	m_Size = 0;
}
private :
	PNODE m_Begin;
	PNODE m_End;
	PNODE m_Root;
	int m_Size;
public :
	void insert(T& Data)
{
	if (!m_Root)
	{
		m_Root = new NODE;
		m_Root->Data = Data;

		m_Begin->Next = m_Root;
		m_Root->Prev = m_Begin;

		m_Root->Next = m_End;
		m_End->Prev = m_Root;
	}
	else
	{
		insert(Data, m_Root);
	}
	++m_Size;
}
	void PreOrder(void(*pFunc)(const T& Data))
{
		PreOrder(pFunc, m_Root);
}
	void InOrder(void(*pFunc)(const T& Data))
{
		InOrder(pFunc, m_Root);
}
	void PostOrder(void(*pFunc)(const T& Data))
{
		PostOrder(pFunc, m_Root);
}
	void Delete(const T& Data)
{
		PNODE Node = Find(Data);

		if (!Node)
			return;

		Delete(Node);

		--m_Size;
}
	PNODE Find(const T& Data)
{
		Find(Data, m_Root);
}
private :
	void Delete(PNODE Node)
{
		// 자식이 없을 때
		if (!Node->Left && !Node->Right)
		{
			PNODE Parent = Node->Parent;

			if (!Parent)
			{
				// 현재 제거하는 노드가 Root Node 라는 의미이다
				delete m_Root;
				m_Root = nullptr;
				m_Size = 0;
				m_Begin->Next = m_End;
				m_End->Prev = m_Begin;

				return;
			}

			if (Parent->Left == Node)
				Parent->Left = nullptr;
			else
				Parent->Right = nullptr;

			PNODE Prev = Node->Prev;
			PNODE Next = Node->Next;

			Prev->Next = Next;
			Next->Prev = Prev;

			delete Node;

			return;
		}

		// 왼쪽이 존재
		if (Node->Left)
		{
			PNODE MaxNode = FindMax(Node);
			PNODE LeftChild = MaxNode->Left;
			PNODE ParentNode = MaxNode->Parent;

			Node->Data = MaxNode->Data;

			if (ParentNode->Right == MaxNode)
				ParentNode->Right = LeftChild;
			else
				ParentNode->Left = LeftChild;

			if (LeftChild)
				LeftChild->Parent = ParentNode;

			PNODE Prev = MaxNode->Prev;
			PNODE Next = MaxNode->Next;
			Prev->Next = Next;
			Next->Prev = Prev;

			delete MaxNode;

			return;
		}

		// 오른쪽이 존재
		if (!Node->Right)
		{
			PNODE MinNode = FindMax(Node);
			PNODE RightChild = MinNode->Right;
			PNODE ParentNode = MinNode->Parent;

			Node->Data = MinNode->Data;

			if (ParentNode->Right == MinNode)
				ParentNode->Right = RightChild;
			else
				ParentNode->Left = RightChild;

			if (RightChild)
				RightChild->Parent = ParentNode;

			PNODE Prev = MinNode->Prev;
			PNODE Next = MinNode->Next;
			Prev->Next = Next;
			Next->Prev = Prev;

			delete MinNode;

			return;
		}
}
	PNODE insert(T& Data, NODE* Node)
{
	// 말단 노드
	if (!Node->Left && !Node->Right)
	{
		PNODE NewNode = new NODE;
		NewNode->Data = Data;

		Node->Left = NewNode;
		NewNode->Parent = Node;

		PNODE Prev = Node->Left;
		Prev->Next = NewNode;
		NewNode->Prev = Prev;

		return NewNode;
	}

	// 왼쪽 조사 
	if (Node->Data > Data)
	{
		if (Node->Left)
			return insert(Data, Node->Left);

		PNODE NewNode = new NODE;
		NewNode->Data = Data;

		Node->Left = NewNode;
		NewNode->Parent = Node;

		PNODE Prev = Node->Prev;
		Prev->Next = NewNode;
		NewNode->Prev = Prev;

		return NewNode;
	}

	// 오른쪽 조사
	if (Node->Right)
		return insert(Data, Node->Right);

	PNODE NewNode = new NODE;
	NewNode->Data = Data;

	Node->Right = NewNode;
	NewNode->Parent = Node;

	PNODE Next = Node->Next;
	Next->Prev = NewNode;
	NewNode->Next = Next;

	return NewNode;
}
	PNODE FindMax(PNODE Node)
{
		if (Node->Right)
			return FindMax(Node->Right);

		return Node;
}
	PNODE FindMin(PNODE Node)
{
		if (Node->Left)
			return FindMin(Node->Left);

		return Node;
}
	void PreOrder(void(*pFunc)(const T& Data), NODE* Node)
	{
		if (!Node)
			return;
		pFunc(Node->Data);
		PreOrder(Node->Left);
		PreOrder(Node->Right);
	}
	void InOrder(void(*pFunc)(const T& Data), NODE* Node)
	{
		if (!Node)
			return;
		InOrder(Node->Left);
		pFunc(Node->Data);
		InOrder(Node->Right);
	}
	void PostOrder(void(*pFunc)(const T& Data), NODE* Node)
	{
		if (!Node)
			return;
		PostOrder(Node->Left);
		PostOrder(Node->Right);
		pFunc(Node->Data);
	}
	PNODE Find(const T& Data, PNODE Node)
{
		if (!Node)
			return nullptr;

		if (Node->Data == Data)
			return Node;

		if (Node->Data > Data)
			return Find(Node->Left);

		if (Node->Right)
			return Find(Node->Right);

		return nullptr;
}
};