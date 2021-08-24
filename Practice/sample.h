#pragma once

template<typename KEY, typename VALUE>
class CAVLTreeNode
{
	template<typename KEY, typename VALUE>
	friend class CAVLTree;
	template<typename KEY, typename VALUE>
	friend class CAVLTreeIterator;
public :
	CAVLTreeNode()
	{

	}
private :
	CAVLTreeNode<KEY, VALUE>* m_Parent;
	CAVLTreeNode<KEY, VALUE>* m_Next;
	CAVLTreeNode<KEY, VALUE>* m_Prev;
	CAVLTreeNode<KEY, VALUE>* m_Left;
	CAVLTreeNode<KEY, VALUE>* m_Right;

public :
	bool IsParent() { return m_Parent != nullptr; }
	KEY GetParentKey() { return m_Parent->first; }
	VALUE GetParentValue() { return m_Parent->second; }
	bool IsLeft() { return m_Left != nullptr; }
	KEY GetLeftKey() { return m_Left->first; }
	VALUE GetLeftValue() { return m_Left->second; }
	bool IsRight() { return m_Right != nullptr; }
	KEY GetRightKey() { return m_Right->first; }
	VALUE GetRightValue() { return m_Right->second; }
public :
	KEY first;
	VALUE second;
};


template<typename KEY, typename VALUE>
class CAVLTreeIterator
{
	template<typename KEY, typename VALUE>
	friend class CAVLTree;
private :
	CAVLTreeNode<KEY, VALUE>* m_Node;
public :
	bool operator == (const CAVLTreeIterator<KEY, VALUE>& iter) const 
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CAVLTreeIterator<KEY, VALUE>& iter) const 
	{
		return m_Node != iter.m_Node;
	}
	bool operator == (const CAVLTreeNode<KEY, VALUE>* Node) const 
	{
		return m_Node == Node;
	}
	bool operator != (const CAVLTreeIterator<KEY, VALUE>* Node) const 
	{
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

	CAVLTreeNode<KEY, VALUE>* operator -> ()
	{
		return m_Node;
	}
};


template<typename KEY, typename VALUE>
class CAVLTree
{
private :
	typedef CAVLTreeNode<KEY, VALUE>* PNODE;
	typedef CAVLTreeNode<KEY, VALUE> NODE;
public :
	typedef CAVLTreeIterator<KEY, VALUE> iterator;
public :
	CAVLTree()
	{
		m_Size = 0;
		m_Begin = new NODE;
		m_End = new NODE;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
		m_Root = nullptr;
	}
	~CAVLTree()
	{
		PNODE Node = m_Begin;
		while (Node)
		{
			PNODE Next = Node->m_Next;
			delete Node;;
			Node = Next;
		}
	}
private :
	int m_Size;
	PNODE m_Begin;
	PNODE m_End;
	PNODE m_Root;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	iterator begin() const
	{
		iterator iter;
		iter.m_Node = m_Begin->m_Next;
		return iter;
	}
	iterator end() const
	{
		iterator iter;
		iter.m_Node = m_End;
		return iter;
	}
	void clear()
	{
		PNODE Node = begin();
		while (Node != end())
		{
			PNODE Next = Node->m_Next;
			delete Node;
			Node = Next;
		}
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
		m_Root = nullptr;
		m_Size = 0;
	}
	// 순회
	void PreOrder(void(*pFunc)(const KEY&, const VALUE&))
	{
		PreOrder(pFunc, m_Root);
	}

	void InOrder(void(*pFunc)(const KEY&, const VALUE&))
	{
		InOrder(pFunc, m_Root);
	}

	void PostOrder(void(*pFunc)(const KEY&, const VALUE&))
	{
		PostOrder(pFunc, m_Root);
	}
	// 삽입
	void insert(const KEY& key, const VALUE& value)
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
			std::cout << "add root" << std::endl;
		}
		else
			insert(key, value, m_Root);
		++m_Size;
	}

	// 삭제
	iterator  Find(const KEY& key)
	{
		return Find(key, m_Root);
	}

	iterator erase(const KEY& key)
	{
		iterator	iter = Find(key);

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

		// 리프노드일 경우 부모로부터 연결을 제거하고 노드를 제거해준다.
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			// 부모노드를 얻어온다.
			PNODE	Parent = iter.m_Node->m_Parent;

			// 만약 부모노드가 없다면 현재 지우려는 노드는 루트노드라는 것이다.
			// 그런데 루트노드가 리프노드라는 말은 이 노드 1개만 남았다는
			// 것이다.
			if (!Parent)
			{
				delete	iter.m_Node;

				m_Root = nullptr;
				--m_Size;

				// 다 지웠으니 Begin과 End끼리 연결한다.
				m_Begin->m_Next = m_End;
				m_End->m_Prev = m_Begin;

				return end();
			}

			// 지우려는 노드가 부모느드의 왼쪽 노드인지 오른쪽 노드인지를
			// 판단하여 부모와의 연결을 끊어준다.
			if (Parent->m_Left == iter.m_Node)
				Parent->m_Left = nullptr;

			else
				Parent->m_Right = nullptr;

			PNODE	Prev = iter.m_Node->m_Prev;
			PNODE	Next = iter.m_Node->m_Next;

			Prev->m_Next = Next;
			Next->m_Prev = Prev;

			delete	iter.m_Node;

			--m_Size;

			lance(Parent);


			// 지운 노드의 다음노드를 넣어서 iterator를 반환해준다.
			iterator	result;
			result.m_Node = Next;

			return result;
		}

		// 만약 왼쪽 노드가 있을 경우 왼쪽 노드에서 가장 큰 노드를 찾아서
		// 지울 노드의 값을 변경해주고 찾아준 노드를 제거한다.
		if (iter.m_Node->m_Left)
		{
			// 왼쪽에 존재하는 노드들중 가장 큰 노드를 찾아온다.
			PNODE	MaxNode = FindMax(iter.m_Node->m_Left);

			// 찾아준 노드의 Key, Value 값으로 변경해준다.
			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

			// 찾아준 노드를 제거해야하기 때문에 
			// 부모로부터 연결을 끊고
			// 제거해주도록 한다.
			// 단, 찾아준 노드가 왼쪽 자식노드가 있을수도 있으므로
			PNODE	LeftChild = MaxNode->m_Left;
			PNODE	Parent = MaxNode->m_Parent;

			if (Parent->m_Left == MaxNode)
				Parent->m_Left = LeftChild;

			else
				Parent->m_Right = LeftChild;

			// 왼쪽 자식노드가 있을 경우라면 
			// 부모로 MaxNode의 부모를
			// 지정해주도록 한다.
			if (LeftChild)
				LeftChild->m_Parent = Parent;

			PNODE	Prev = MaxNode->m_Prev;
			PNODE	Next = MaxNode->m_Next;

			Prev->m_Next = Next;
			Next->m_Prev = Prev;

			delete	MaxNode;

			--m_Size;

			lance(Parent);

			iterator	result;
			result.m_Node = Next;

			return result;
		}

		// 지울 노드의 오른쪽 노드만 존재할 경우 오른쪽 노드에서 가장 작은 노드를
		// 찾아온다.
		PNODE	MinNode = FindMin(iter.m_Node->m_Right);

		// 찾아준 노드의 Key, Value 값으로 변경해준다.
		iter.m_Node->first = MinNode->first;
		iter.m_Node->second = MinNode->second;

		// 찾아준 노드를 제거해야하기 때문에 부모로부터 연결을 끊고
		// 제거해주도록 한다.
		// 단, 찾아준 노드가 오른쪽 자식노드가 있을수도 있으므로
		PNODE	RightChild = MinNode->m_Right;
		PNODE	Parent = MinNode->m_Parent;

		if (Parent->m_Left == MinNode)
			Parent->m_Left = RightChild;

		else
			Parent->m_Right = RightChild;

		// 오른쪽 자식노드가 있을 경우라면 부모로 MinNode의 부모를
		// 지정해주도록 한다.
		if (RightChild)
			RightChild->m_Parent = Parent;

		PNODE	Prev = MinNode->m_Prev;
		PNODE	Next = MinNode->m_Next;

		Prev->m_Next = Next;
		Next->m_Prev = Prev;

		delete	MinNode;

		--m_Size;

		lance(Parent);

		iterator	result;
		result.m_Node = Next;

		return result;
	}
private :
	void lance(PNODE Node)
	{
		if (!Node) return;
		int Factor = BalanceFactor(Node);

		// 오른쪽
		if (Factor <= -2)
		{
			std::cout << "right unb" << std::endl;
			int RightFactor = BalanceFactor(Node->m_Right);
			if (RightFactor <= 0)
			{
				Node = RotationLeft(Node);
			}
			else
			{
				RotationRight(Node->m_Right);
				Node = RotationLeft(Node);
			}
		}
		// 왼쪽
		else if (Factor >= 2)
		{
			int LeftFactor = BalanceFactor(Node->m_Left);
			if (LeftFactor >= 0)
			{
				Node = RotationRight(Node);
			}
			else
			{
				RotationLeft(Node->m_Left);
				Node = RotationRight(Node);
			}
		}
		lance(Node->m_Parent);
	}
	PNODE RotationLeft(PNODE Node)
	{
		PNODE Parent = Node->m_Parent;
		PNODE RightChild = Node->m_Right;
		PNODE RightLeftChild = RightChild->m_Left;

		RightChild->m_Left = Node;
		Node->m_Parent = RightChild;

		Node->m_Right = RightLeftChild;
		if (RightLeftChild) 
			RightLeftChild->m_Parent = Node;

		RightChild->m_Parent = Parent;
		if (Parent)
		{
			if (Parent->m_Left == Node)
				Parent->m_Left = RightChild;
			else
				Parent->m_Right = RightChild;
		}
		else
			m_Root = RightChild;
		std::cout << "rotate left" << std::endl;
		return RightChild;

	}
	PNODE RotationRight(PNODE Node)
	{
		PNODE Parent = Node->m_Parent;
		PNODE LeftChild = Node->m_Left;
		PNODE LeftRightChild = LeftChild->m_Right;

		LeftChild->m_Right = Node;
		Node->m_Parent = LeftChild;

		Node->m_Left = LeftRightChild;
		if (LeftRightChild) 
			LeftRightChild->m_Parent = Node;

		LeftChild->m_Parent = Parent;
		if (Parent)
		{
			if (Parent->m_Left == Node)
				Parent->m_Left = LeftChild;
			else
				Parent->m_Right = LeftChild;
		}
		else
			m_Root = LeftChild;
		return LeftChild;
	}
	int BalanceFactor(PNODE Node)
	{
		return GetHeight(Node->m_Left) - GetHeight(Node->m_Right);
	}
	int GetHeight(PNODE Node)
	{
		if (!Node) return 0;
		int Left = GetHeight(Node->m_Left);
		int Right = GetHeight(Node->m_Right);
		int Height = Left > Right ? Left : Right;
		return Height + 1;
	}
	PNODE FindMax(PNODE Node)
	{
		if (Node->m_Right) return FindMax(Node->m_Right);
		return Node;
	}
	PNODE FindMin(PNODE Node)
	{
		if (Node->m_Left) return FindMin(Node->m_Left);
		return Node;
	}

	iterator Find(const KEY& key, PNODE Node)	const
	{
		// 기준노드가 nullptr일 경우 더이상 탐색할 노드가 없으므로
		// end를 리턴한다. 못찾았다는 것이다.
		if (!Node)
			return end();

		if (Node->first == key)
		{
			iterator	iter;
			iter.m_Node = Node;
			return iter;
		}

		// 키를 비교하여 작으면 왼쪽, 크면 오른쪽으로 탐색해서 들어간다.
		if (Node->first > key)
			return Find(key, Node->m_Left);

		return Find(key, Node->m_Right);
	}

	void insert(const KEY& key, const VALUE& value, PNODE Node)
	{
		if (Node->first > key)
		{
			if (Node->m_Left) insert(key, value, Node->m_Left);
			PNODE NewNode = new NODE;
			NewNode->first = key;
			NewNode->second = value;

			Node->m_Left = NewNode;
			NewNode->m_Parent = Node;

			PNODE Prev = Node->m_Prev;
			Prev->m_Next = NewNode;
			NewNode->m_Prev = Prev;

			NewNode->m_Next = Node;
			Node->m_Prev = NewNode;
			lance(NewNode);
		}

		if (Node->m_Right) 
			insert(key, value, Node->m_Right);
		PNODE NewNode = new NODE;
		NewNode->first = key;
		NewNode->second = value;

		Node->m_Right = NewNode;
		NewNode->m_Parent = Node;

		PNODE Next = Node->m_Next;
		Next->m_Next = NewNode;
		NewNode->m_Prev = Next;

		NewN->m_Next = Next;
		Next->m_Prev = Node;

		lance(NewNode);
	}
	void PreOrder(void(*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		pFunc(Node->first, Node->second);
		PreOrder(pFunc, Node->m_Left);
		PreOrder(pFunc, Node->m_Right);
	}

	void InOrder(void(*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		InOrder(pFunc, Node->m_Left);
		pFunc(Node->first, Node->second);
		InOrder(pFunc, Node->m_Right);
	}

	void PostOrder(void(*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		PostOrder(pFunc, Node->m_Left);
		PostOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second);
	}



};