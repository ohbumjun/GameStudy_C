#pragma once

template <typename KEY, typename VALUE>
class CAVLTreeNode
{
	template <typename KEY, typename VALUE>
	friend class CAVLTree;
	template <typename KEY, typename VALUE>
	friend class CAVLTreeIterator;

private:
	CAVLTreeNode() :
		m_Left(nullptr),
		m_Right(nullptr),
		m_Parent(nullptr),
		m_Next(nullptr),
		m_Prev(nullptr)
	{
	}

	~CAVLTreeNode()
	{
	}

private:
	CAVLTreeNode<KEY, VALUE>* m_Left;
	CAVLTreeNode<KEY, VALUE>* m_Right;
	CAVLTreeNode<KEY, VALUE>* m_Parent;
	CAVLTreeNode<KEY, VALUE>* m_Next;
	CAVLTreeNode<KEY, VALUE>* m_Prev;

public:
	KEY		first;
	VALUE	second;
public:
	bool IsParent() const
	{
		return m_Parent != nullptr;
	}
	KEY GetParentKey() const
	{
		return m_Parent->first;
	}
	VALUE GetParentValue() const
	{
		return m_Parent->second;
	}
	bool IsLeft() const
	{
		return m_Left != nullptr;
	}
	KEY GetLeftKey() const
	{
		return m_Left->first;
	}
	VALUE GetLeftValue() const
	{
		return m_Left->second;
	}
	bool IsRight() const
	{
		return m_Right != nullptr;
	}
	KEY GetRightKey() const
	{
		return m_Right->first;
	}
	VALUE GetRightValue() const
	{
		return m_Right->second;
	}
};

template <typename KEY, typename VALUE>
class CAVLTreeIterator
{
	template <typename KEY, typename VALUE>
	friend class CAVLTree;

public:
	CAVLTreeIterator() :
		m_Node(nullptr)
	{
	}

	~CAVLTreeIterator()
	{
	}

private:
	CAVLTreeNode<KEY, VALUE>* m_Node;

public:
	// iterator끼리 서로 가지고 있는 노드가 같을 경우 같다고 판단한다.
	bool operator == (const CAVLTreeIterator<KEY, VALUE>& iter)	const
	{
		return m_Node == iter.m_Node;
	}

	bool operator != (const CAVLTreeIterator<KEY, VALUE>& iter)	const
	{
		return m_Node != iter.m_Node;
	}

	bool operator == (const CAVLTreeNode<KEY, VALUE>* Node)	const
	{
		return m_Node == Node;
	}

	bool operator != (const CAVLTreeNode<KEY, VALUE>* Node)	const
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

// Key는 탐색을 하기 위한 타입이다.
// Value는 실제 저장하기 위한 데이터 타입이다.
// 그래서 탐색을 할때는 Key타입으로 한다.
// 만약 Key타입이 문자열이라면 문자열로 탐색을 할 수 있는 것이다.
template <typename KEY, typename VALUE>
class CAVLTree
{
public:
	CAVLTree()
	{
		m_Root = nullptr;
		m_Size = 0;

		m_Begin = new NODE;
		m_End = new NODE;

		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
	}

	~CAVLTree()
	{
		PNODE	DeleteNode = m_Begin;

		while (DeleteNode)
		{
			PNODE	Next = DeleteNode->m_Next;

			delete	DeleteNode;

			DeleteNode = Next;
		}
	}

private:
	typedef CAVLTreeNode<KEY, VALUE>		NODE;
	typedef CAVLTreeNode<KEY, VALUE>* PNODE;

public:
	typedef CAVLTreeIterator<KEY, VALUE>	iterator;

private:
	PNODE	m_Root;
	PNODE	m_Begin;
	PNODE	m_End;
	int		m_Size;

public:
	void insert(const KEY& key, const VALUE& value)
	{
		// 처음 데이터를 추가할 경우
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

	iterator begin()	const
	{
		iterator	iter;
		iter.m_Node = m_Begin->m_Next;
		return iter;
	}

	iterator end()	const
	{
		iterator	iter;
		iter.m_Node = m_End;
		return iter;
	}

	iterator Find(const KEY& key)	const
	{
		return Find(key, m_Root);
	}

	void clear()
	{
		PNODE	Node = m_Begin->m_Next;

		while (Node != m_End)
		{
			PNODE	Next = Node->m_Next;

			delete	Node;

			Node = Next;
		}

		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;

		m_Size = 0;

		m_Root = nullptr;
	}

	/*
	이진트리는 3가지의 순회방법이 있다.
	전위순회, 중위순회, 후위순회
	전위순회 : Root -> Left -> Right
	중위순회 : Left -> Root -> Right
	후위순회 : Left -> Right -> Root
	*/
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

		// 리프노드일 경우 부모로부터 연결을 제거하고
		// 노드를 제거해준다.
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			// 부모노드를 얻어온다.
			PNODE	Parent = iter.m_Node->m_Parent;

			// 만약 부모노드가 없다면 
			// 현재 지우려는 노드는 루트노드라는 것이다.
			// 그런데 루트노드가 리프노드라는 말은 
			// 이 노드 1개만 남았다는
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

			// 지우려는 노드가 부모느드의 
			// 왼쪽 노드인지 오른쪽 노드인지를
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

			ReBalance(Parent);

			// 지운 노드의 다음노드를 넣어서 
			// iterator를 반환해준다.
			iterator	result;
			result.m_Node = Next;

			return result;
		}

		// 만약 왼쪽 노드가 있을 경우 
		// 왼쪽 노드에서 가장 큰 노드를 찾아서
		// 지울 노드의 값을 변경해주고 
		// 찾아준 노드를 제거한다.
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
			// 단, 찾아준 노드가 왼쪽 자식노드가 
			// 있을수도 있으므로
			PNODE	LeftChild = MaxNode->m_Left;
			PNODE	Parent = MaxNode->m_Parent;

			if (Parent->m_Left == MaxNode)
				Parent->m_Left = LeftChild;

			else
				Parent->m_Right = LeftChild;

			// 왼쪽 자식노드가 있을 경우라면 부모로 MaxNode의 부모를
			// 지정해주도록 한다.
			if (LeftChild)
				LeftChild->m_Parent = Parent;

			PNODE	Prev = MaxNode->m_Prev;
			PNODE	Next = MaxNode->m_Next;

			Prev->m_Next = Next;
			Next->m_Prev = Prev;

			delete	MaxNode;

			--m_Size;
			ReBalance(Parent);

			iterator	result;
			result.m_Node = Next;

			return result;
		}

		// 지울 노드의 오른쪽 노드만 존재할 경우 
		// 오른쪽 노드에서 가장 작은 노드를
		// 찾아온다.
		PNODE	MinNode = FindMin(iter.m_Node->m_Right);

		// 찾아준 노드의 Key, Value 값으로 변경해준다.
		iter.m_Node->first = MinNode->first;
		iter.m_Node->second = MinNode->second;

		// 찾아준 노드를 제거해야하기 때문에 
		// 부모로부터 연결을 끊고
		// 제거해주도록 한다.
		// 단, 찾아준 노드가 오른쪽 자식노드가 
		// 있을수도 있으므로
		PNODE	RightChild = MinNode->m_Right;
		PNODE	Parent = MinNode->m_Parent;

		if (Parent->m_Left == MinNode)
			Parent->m_Left = RightChild;

		else
			Parent->m_Right = RightChild;

		// 오른쪽 자식노드가 있을 경우라면 
		// 부모로 MinNode의 부모를
		// 지정해주도록 한다.
		if (RightChild)
			RightChild->m_Parent = Parent;

		PNODE	Prev = MinNode->m_Prev;
		PNODE	Next = MinNode->m_Next;

		Prev->m_Next = Next;
		Next->m_Prev = Prev;

		delete	MinNode;

		--m_Size;
		ReBalance(Parent);

		iterator	result;
		result.m_Node = Next;

		return result;
	}
	// 높이 차이 구해주는 함수
	
	

private:
	PNODE insert(const KEY& key, const VALUE& value, PNODE Node)
	{
		// 기준노드보다 작다면 왼쪽이다.
		if (Node->first > key)
		{
			// 만약 기준노드의 왼쪽 자식노드가 있다면 그 왼쪽 자식노드를
			// 기준노드로 하여 다시 탐색을 하게 한다.
			if (Node->m_Left)
				return insert(key, value, Node->m_Left);

			// 더이상 왼쪽 자식노드가 없을 경우 이 위치에 새로 노드를 생성하여
			// 추가해주어야 한다.
			PNODE	NewNode = new NODE;

			NewNode->first = key;
			NewNode->second = value;

			// 기준노드의 왼쪽 자식노드로 지정한다.
			Node->m_Left = NewNode;
			NewNode->m_Parent = Node;

			// 왼쪽으로 배치가 된다는것은 부모노드보다 작다는 것이다.
			// 그러므로 부모노드의 이전노드와 부모노드 사이에 새로 생성된
			// 노드를 리스트로 연결해주도록 한다.
			PNODE	Prev = Node->m_Prev;

			Prev->m_Next = NewNode;
			NewNode->m_Prev = Prev;

			NewNode->m_Next = Node;
			Node->m_Prev = NewNode;

			ReBalance(NewNode);

			return NewNode;
		}

		// 여기로 내려오면 값이 크다는 것이므로 오른쪽으로 탐색을 해봐야 한다.
		// 만약 오른쪽 자식노드가 있을 경우 기준노드를 오른쪽 자식노드로 하여
		// 탐색하게 한다.
		if (Node->m_Right)
			return insert(key, value, Node->m_Right);

		// 더이상 오른쪽 자식노드가 없을 경우 이 위치에 새로 노드를 생성하여
		// 추가해주어야 한다.
		PNODE	NewNode = new NODE;

		NewNode->first = key;
		NewNode->second = value;

		// 기준노드의 오른쪽 자식노드로 지정한다.
		Node->m_Right = NewNode;
		NewNode->m_Parent = Node;

		PNODE	Next = Node->m_Next;

		Node->m_Next = NewNode;
		NewNode->m_Prev = Node;

		NewNode->m_Next = Next;
		Next->m_Prev = NewNode;

		ReBalance(NewNode);

		return NewNode;
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

	// 노드의 높이를 구해주는 함수
	int GetHeight(PNODE Node)
	{
		if (!Node) return 0;
		// 들어온 노드의 왼쪽, 오른쪽으로
		// 타고 들어갈 것이다
		int Left = GetHeight(Node->m_Left);
		int Right = GetHeight(Node->m_Right);

		int Height = Left > Right ? Left : Right;
		
		// 왜 + 1 ?
		// 만약 해당 노드가 자식 노드들이 없으면, 0이 Height 값으로 들어간다
		// 자기 자신하나가 높이를 구성하므로
		// 높이는 1 이라고 할 수 있다. 
		return Height + 1;
	}

	int BalanceFactor(PNODE Node)
	{
		return GetHeight(Node->m_Left) - GetHeight(Node->m_Right);
		// 왼쪽에서 오른쪽을 뺀다
		// +2 이상 : 왼쪽으로 균형 무너짐
		// -2 이하 : 오른쪽으로 균형 무너짐
	
	}

	// 균형 맞춰주는 함수
	void ReBalance(PNODE Node)
	{
		// 부모를 타고 올라가면서
		// 무너진 곳을 찾아야 한다
		// 왜냐하면, 어디가 무너졌는지 모르기 때문이다


		// 부모를 타고 올라가는데 
		// Node가 null이 나오는 경우가 있다.
		// 무슨 의미일까 ? 
		// root의 부모노드로 타고 올라가는 경우
		// 모든 노드는 root 노드를 제외
		// 부모노드를 가지고 있기 때문이다
		// root 노드의 부모노드가 Node에 들어온것
		// 즉, 모든 노드에 대한 탐색이 끝난 것
		// 따라서 return
		// 더이상 할 게 없다는 것이기 때문이다 
		if (!Node) return;

		// 왼,오 의 높이 차이를 구해준다 
		int Factor = BalanceFactor(Node);

		// 오른쪽으로 균형이 무너졌을 경우
		if (Factor <= -2)
		{
			// 1. R-R ( 오른쪽, 오른쪽 )
			// 1. R-L ( 오른쪽, 왼쪽 )
			
			// 바로 오른쪽 노드의 왼,오 높이를 본다
			// R-R일 경우 : -1 ( 0보다 작거나 같거나 )
			// R-L일 경우 : 1 ( 0보다 크거나 ) 
			int RightFactor = BalanceFactor(Node->m_Right);

			// 음수라면, 오른쪽, 오른쪽으로 균형이 무너졌을 경우
			if (RightFactor <= 0)
			{
				// 기준노드를 중심으로 왼쪽으로 회전
				Node = RotationLeft(Node);
			}
			// 오른쪽 왼쪽으로 균형이 무너졌을 경우이다.
			else
			{
				// 오른쪽 자식노드를 중심으로
				// 오른쪽 회전을 한다
				RotationRight(Node->m_Right);

				// 기준노드를 중심으로 
				// 왼쪽 회전을 시켜준다 
				Node = RotationLeft(Node);
			}

		}
		// 왼쪽으로 균형이 무너졌을 경우 	
		else if (Factor >= 2)
		{
			// 군형을 가지고 온다 
			int LeftFactor = BalanceFactor(Node->m_Left);
		
			// 왼쪽 왼쪽으로 균형이
			// 무너졌을 경우 
			if (LeftFactor >= 0)
			{
				// 기준노드를 중심으로
				// 오른쪽으로 회전한다 
				Node = RotationRight(Node);
			}
			// 왼쪽 오른쪽으로
			// 균형이 무너졌을 경우 
			else
			{
				// 기준노드의 왼쪽 자식노드를 중심으로
				// 왼쪽으로 회전한다
				RotationLeft(Node->m_Left);

				// 기준노드를 중심으로
				// 오른쪽으로 회전한다 
				Node = RotationRight(Node);
			}
		}

		// 부모노드로 찾아간다
		// 더이상 부모가 없을 때까지
		ReBalance(Node->m_Parent);

	}

	// 왼쪽 회전 
	// 기준 노드를 중심으로 오른쪽 자식노드가
	// 해당 기준 노드 위치로 온다
	// 즉, 바뀐 기준노드가 어떤 노드인지를
	// 반환하는 식으로 작업할 것이다.
	PNODE RotationLeft(PNODE Node)
	{
		// 기준 노드의 부모노드를 얻어온다
		PNODE Parent = Node->m_Parent;
		// 기준 노드의 오른쪽 자식 노드를 얻어온다
		PNODE RightChild = Node->m_Right;
		// Right의 left child가 있으면
		// 기준노드 오른쪽 child로

		// (오른쪽 회전은 )
		// ( Left의 RightChild가 있으면 )
		// (기준노드가 Right으로 내려가고 나서)
		// (기준노드의 왼쪽 Left Child로 달아준다 )
		
		// 기준노드의 오른쪽 자식노드의 
		// 왼쪽 자식 노드를 얻어온다
		PNODE RightLeftChild = RightChild->m_Left;

		// 기준노드의 오른쪽 자식노드로
		// 오른쪽 자식노드의 왼쪽 자식노드를
		// 붙여준다
		Node->m_Right = RightLeftChild;

		// 오른쪽 자식노드의 왼쪽 자식노드가 있을 경우
		// 그 부모를 기준노드로 지정한다
		if (RightLeftChild)
			RightLeftChild->m_Parent = m_Node;

		// 기준노드를 오른쪽 자식노드의
		// 왼쪽 자식으로 붙여준다
		RightChild->m_Left = Node;

		// 기준노드의 부모로 오른쪽
		// 자식노드를 지정한다
		Node->m_Parent = RightChild;

		//오른쪽 자식노드의 부모노드를
		//기준노드의 부모노드로 지정한다
		RightChild->m_Parent = Parent;

		// 기준노드의 부모노드가 있을 경우
		// 기준노드가 부모노드의 왼쪽 자식인지, 오른쪽 자식인지를 판단하여
		// 그 방향으로 오른쪽 자식노드를
		// 붙여준다
		if (Parent)
		{
			if (Parent->m_Left == Node)
				Parent->m_Left = RightChild;
			else
				Parent->m_Right = RightChild;
		}

		// 기준노드의 부모노드가 없다면
		// 기준노드는 루트노드라는 의미이다
		// 그러므로, 기준노드의 자리에
		// RightChild가 오게 되므로
		// 루트노드를 RightChild로
		// 교체해주어야 한다
		else
			m_Root = RightChild;

		// 바뀐 기준노드를 반환한다
		return RightChild;

	}

	// 오른쪽 회전 
	// 기준 노드를 중심으로 왼쪽 자식노드가
	// 해당 기준 노드 위치로 온다 
	PNODE RotationRight(PNODE Node)
	{
		// 왼쪽 자식노드가 기준노드로 올라오고
		// 왼쪽 자식노드가 지니던 오른쪽 자식은
		// 바뀐 기준노드의 왼쪽으로 놓는다

		// 기준노드의 부모노드를 얻어온다
		PNODE Parent = Node->m_Parent;

		// 기준노드의 왼쪽 자식노드를 얻어온다
		PNODE LeftChild = Node->m_Left;

		// 기준노드의 왼쪽 자식노드의
		// 오른쪽 자식노드를 얻어온다
		PNODE LeftRightChild = LeftChild->m_Right;
		
		Node->m_Left = LeftRightChild;

		// 오른쪽 자식노드의 왼쪽 자식노드가 있을 경우
		// 그 부모를 기준노드로 지정한다
		if (LeftRightChild)
			LeftRightChild->m_Parent = Node;

		// 왼쪽 자식노드의 오른쪽 자식으로
		// 기준노드를 붙여준다.
		LeftChild->m_Right = Node;

		// 기준노드의 부모노드로 왼쪽 자식노드를 지정한다
		Node->m_Parent = LeftChild;

		// 왼쪽 자식 노드의 부모로 
		// 기준노드의 부모노드를 지정한다
		LeftChild->m_Parent = Node;

		// 기준노드의 부모노드가 있을 경우
		// 부모노드의 왼쪽,오른쪽 자식인지 판단
		// 그 방향으로, 기준노드의 왼쪽 자식으로 붙여준다 
		if (Parent)
		{
			if (Parent->m_Left == Node)
				Parent->m_Left = LeftChild;
			else
				Parent->m_Right = LeftChild;
		}
		// 기준노드의 부모노드가 없다면
		// 기준노드는 루트노드라는 의미이다
		// 그러므로, 기준노드의 자리에
		// LeftChild가 오게 되므로
		// 루트노드를 LeftChild로
		// 교체해주어야 한다
		else
			m_Root = LeftChild;

		return LeftChild;
	}

};

/*
좌좌 : 우회전
우우 : 좌회전

(왼 무너짐 )좌우 : 
1) 1을 2의 자식
2) 우회전 

(우 무너짐 )우좌 
1) 3을 2의 자식
2) 좌회전 

*/