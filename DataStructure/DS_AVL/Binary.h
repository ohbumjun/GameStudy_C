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
	bool IsParent()	const
	{
		return m_Parent != nullptr;
	}

	KEY GetParentKey()	const
	{
		return m_Parent->first;
	}

	VALUE GetParentValue()	const
	{
		return m_Parent->second;
	}

	bool IsLeft()	const
	{
		return m_Left != nullptr;
	}

	KEY GetLeftKey()	const
	{
		return m_Left->first;
	}

	VALUE GetLeftValue()	const
	{
		return m_Left->second;
	}

	bool IsRight()	const
	{
		return m_Right != nullptr;
	}

	KEY GetRightKey()	const
	{
		return m_Right->first;
	}

	VALUE GetRightValue()	const
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
	// iterator���� ���� ������ �ִ� ��尡 ���� ��� ���ٰ� �Ǵ��Ѵ�.
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

// Key�� Ž���� �ϱ� ���� Ÿ���̴�.
// Value�� ���� �����ϱ� ���� ������ Ÿ���̴�.
// �׷��� Ž���� �Ҷ��� KeyŸ������ �Ѵ�.
// ���� KeyŸ���� ���ڿ��̶�� ���ڿ��� Ž���� �� �� �ִ� ���̴�.
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
		// ó�� �����͸� �߰��� ���
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
	����Ʈ���� 3������ ��ȸ����� �ִ�.
	������ȸ, ������ȸ, ������ȸ
	������ȸ : Root -> Left -> Right
	������ȸ : Left -> Root -> Right
	������ȸ : Left -> Right -> Root
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

		// ��������� ��� �θ�κ��� ������ �����ϰ� ��带 �������ش�.
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			// �θ��带 ���´�.
			PNODE	Parent = iter.m_Node->m_Parent;

			// ���� �θ��尡 ���ٸ� ���� ������� ���� ��Ʈ����� ���̴�.
			// �׷��� ��Ʈ��尡 ��������� ���� �� ��� 1���� ���Ҵٴ�
			// ���̴�.
			if (!Parent)
			{
				delete	iter.m_Node;

				m_Root = nullptr;
				--m_Size;

				// �� �������� Begin�� End���� �����Ѵ�.
				m_Begin->m_Next = m_End;
				m_End->m_Prev = m_Begin;

				return end();
			}

			// ������� ��尡 �θ������ ���� ������� ������ ���������
			// �Ǵ��Ͽ� �θ���� ������ �����ش�.
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


			// ���� ����� ������带 �־ iterator�� ��ȯ���ش�.
			iterator	result;
			result.m_Node = Next;

			return result;
		}

		// ���� ���� ��尡 ���� ��� ���� ��忡�� ���� ū ��带 ã�Ƽ�
		// ���� ����� ���� �������ְ� ã���� ��带 �����Ѵ�.
		if (iter.m_Node->m_Left)
		{
			// ���ʿ� �����ϴ� ������ ���� ū ��带 ã�ƿ´�.
			PNODE	MaxNode = FindMax(iter.m_Node->m_Left);

			// ã���� ����� Key, Value ������ �������ش�.
			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

			// ã���� ��带 �����ؾ��ϱ� ������ �θ�κ��� ������ ����
			// �������ֵ��� �Ѵ�.
			// ��, ã���� ��尡 ���� �ڽĳ�尡 �������� �����Ƿ�
			PNODE	LeftChild = MaxNode->m_Left;
			PNODE	Parent = MaxNode->m_Parent;

			if (Parent->m_Left == MaxNode)
				Parent->m_Left = LeftChild;

			else
				Parent->m_Right = LeftChild;

			// ���� �ڽĳ�尡 ���� ����� �θ�� MaxNode�� �θ�
			// �������ֵ��� �Ѵ�.
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

		// ���� ����� ������ ��常 ������ ��� ������ ��忡�� ���� ���� ��带
		// ã�ƿ´�.
		PNODE	MinNode = FindMin(iter.m_Node->m_Right);

		// ã���� ����� Key, Value ������ �������ش�.
		iter.m_Node->first = MinNode->first;
		iter.m_Node->second = MinNode->second;

		// ã���� ��带 �����ؾ��ϱ� ������ �θ�κ��� ������ ����
		// �������ֵ��� �Ѵ�.
		// ��, ã���� ��尡 ������ �ڽĳ�尡 �������� �����Ƿ�
		PNODE	RightChild = MinNode->m_Right;
		PNODE	Parent = MinNode->m_Parent;

		if (Parent->m_Left == MinNode)
			Parent->m_Left = RightChild;

		else
			Parent->m_Right = RightChild;

		// ������ �ڽĳ�尡 ���� ����� �θ�� MinNode�� �θ�
		// �������ֵ��� �Ѵ�.
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

private:
	PNODE insert(const KEY& key, const VALUE& value, PNODE Node)
	{
		// ���س�庸�� �۴ٸ� �����̴�.
		if (Node->first > key)
		{
			// ���� ���س���� ���� �ڽĳ�尡 �ִٸ� �� ���� �ڽĳ�带
			// ���س��� �Ͽ� �ٽ� Ž���� �ϰ� �Ѵ�.
			if (Node->m_Left)
				return insert(key, value, Node->m_Left);

			// ���̻� ���� �ڽĳ�尡 ���� ��� �� ��ġ�� ���� ��带 �����Ͽ�
			// �߰����־�� �Ѵ�.
			PNODE	NewNode = new NODE;

			NewNode->first = key;
			NewNode->second = value;

			// ���س���� ���� �ڽĳ��� �����Ѵ�.
			Node->m_Left = NewNode;
			NewNode->m_Parent = Node;

			// �������� ��ġ�� �ȴٴ°��� �θ��庸�� �۴ٴ� ���̴�.
			// �׷��Ƿ� �θ����� �������� �θ��� ���̿� ���� ������
			// ��带 ����Ʈ�� �������ֵ��� �Ѵ�.
			PNODE	Prev = Node->m_Prev;

			Prev->m_Next = NewNode;
			NewNode->m_Prev = Prev;

			NewNode->m_Next = Node;
			Node->m_Prev = NewNode;

			ReBalance(NewNode);

			return NewNode;
		}

		// ����� �������� ���� ũ�ٴ� ���̹Ƿ� ���������� Ž���� �غ��� �Ѵ�.
		// ���� ������ �ڽĳ�尡 ���� ��� ���س�带 ������ �ڽĳ��� �Ͽ�
		// Ž���ϰ� �Ѵ�.
		if (Node->m_Right)
			return insert(key, value, Node->m_Right);

		// ���̻� ������ �ڽĳ�尡 ���� ��� �� ��ġ�� ���� ��带 �����Ͽ�
		// �߰����־�� �Ѵ�.
		PNODE	NewNode = new NODE;

		NewNode->first = key;
		NewNode->second = value;

		// ���س���� ������ �ڽĳ��� �����Ѵ�.
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
		// ���س�尡 nullptr�� ��� ���̻� Ž���� ��尡 �����Ƿ�
		// end�� �����Ѵ�. ��ã�Ҵٴ� ���̴�.
		if (!Node)
			return end();

		if (Node->first == key)
		{
			iterator	iter;
			iter.m_Node = Node;
			return iter;
		}

		// Ű�� ���Ͽ� ������ ����, ũ�� ���������� Ž���ؼ� ����.
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

	PNODE RotationLeft(PNODE Node)
	{
		// ���س���� �θ��带 ���´�.
		PNODE	Parent = Node->m_Parent;

		// ���س���� ������ �ڽĳ�带 ���´�.
		PNODE	RightChild = Node->m_Right;

		// ���س���� ������ �ڽĳ���� ���� �ڽĳ�带 ���´�.
		PNODE	RightLeftChild = RightChild->m_Left;

		// ���س���� ������ �ڽĳ��� �������ڽĳ���� �����ڽĳ�带 �ٿ��ش�.
		Node->m_Right = RightLeftChild;

		// ������ �ڽĳ���� ���� �ڽĳ�尡 ���� ��� �� �θ� ���س��� �����Ѵ�.
		if (RightLeftChild)
			RightLeftChild->m_Parent = Node;

		// ���س�带 ������ �ڽĳ���� ���� �ڽ����� �ٿ��ش�.
		RightChild->m_Left = Node;

		// ���س���� �θ�� ������ �ڽĳ�带 �����Ѵ�.
		Node->m_Parent = RightChild;

		// ������ �ڽĳ���� �θ��带 ���س���� �θ���� �����Ѵ�.
		RightChild->m_Parent = Parent;

		// ���س���� �θ��尡 ������� ���س�尡 �θ����� �����ڽ����� ������ �ڽ�������
		// �Ǵ��Ͽ� �� �������� ���س���� ������ �ڽĳ�带 �ٿ��ش�.
		if (Parent)
		{
			if (Parent->m_Left == Node)
				Parent->m_Left = RightChild;

			else
				Parent->m_Right = RightChild;
		}

		// ���س���� �θ��尡 ���ٸ� ���س��� ��Ʈ����� �ǹ��̴�.
		// �׷��Ƿ� ���س���� �ڸ��� RightChild�� ���ԵǹǷ� ��Ʈ��带
		// RightChild�� ��ü���־�� �Ѵ�.
		else
			m_Root = RightChild;

		// �ٲ� ���س�带 ��ȯ�Ѵ�.
		return RightChild;
	}

	PNODE RotationRight(PNODE Node)
	{
		// ���س���� �θ��带 ���´�.
		PNODE	Parent = Node->m_Parent;

		// ���س���� ���� �ڽĳ�带 ���´�.
		PNODE	LeftChild = Node->m_Left;

		// ���س���� ���� �ڽĳ���� ������ �ڽĳ�带 ���´�.
		PNODE	LeftRightChild = LeftChild->m_Right;

		// ���س���� ���� �ڽĳ��� �����ڽĳ���� �������ڽĳ�带 �ٿ��ش�.
		Node->m_Left = LeftRightChild;

		// ���� �ڽĳ���� ������ �ڽĳ�尡 ���� ��� �� �θ� ���س��� �����Ѵ�.
		if (LeftRightChild)
			LeftRightChild->m_Parent = Node;

		// ���� �ڽĳ���� ������ �ڽ����� ���س�带 �ٿ��ش�.
		LeftChild->m_Right = Node;

		// ���س���� �θ���� ���� �ڽĳ�带 �����Ѵ�.
		Node->m_Parent = LeftChild;

		// �����ڽĳ���� �θ�� ���س���� �θ��带 �����Ѵ�.
		LeftChild->m_Parent = Parent;

		// ���س���� �θ��尡 ������� ���س�尡 �θ����� �����ڽ����� ������ �ڽ�������
		// �Ǵ��Ͽ� �� �������� ���س���� ���� �ڽĳ�带 �ٿ��ش�.
		if (Parent)
		{
			if (Parent->m_Left == Node)
				Parent->m_Left = LeftChild;

			else
				Parent->m_Right = LeftChild;
		}

		// ���س���� �θ��尡 ���ٸ� ���س��� ��Ʈ����� �ǹ��̴�.
		// �׷��Ƿ� ���س���� �ڸ��� LeftChild�� ���ԵǹǷ� ��Ʈ��带
		// LeftChild�� ��ü���־�� �Ѵ�.
		else
			m_Root = LeftChild;

		// �ٲ� ���س�带 ��ȯ�Ѵ�.
		return LeftChild;
	}

	PNODE FindMin(PNODE Node)
	{
		if (Node->m_Left)
			return FindMin(Node->m_Left);

		return Node;
	}

	int GetHeight(PNODE Node)
	{
		if (!Node)
			return 0;

		int	Left = GetHeight(Node->m_Left);
		int	Right = GetHeight(Node->m_Right);

		int	Height = Left > Right ? Left : Right;

		return Height + 1;
	}

	int BalanceFactor(PNODE Node)
	{
		return GetHeight(Node->m_Left) - GetHeight(Node->m_Right);
	}

	void ReBalance(PNODE Node)
	{
		if (!Node)
			return;

		// ���ʰ� �������� �������̸� �����ش�.
		int	Factor = BalanceFactor(Node);

		// ���������� ������ �������� ���
		if (Factor <= -2)
		{
			int	RightFactor = BalanceFactor(Node->m_Right);

			// ������� ������ ���������� ������ �������� ����̴�.
			if (RightFactor <= 0)
			{
				// ���س�带 �߽����� �������� ȸ���� �����ش�.
				Node = RotationLeft(Node);
			}

			// ������ �������� ������ �������� ����̴�.
			else
			{
				// ������ �ڽĳ�带 �߽����� ������ ȸ���� �Ѵ�.
				RotationRight(Node->m_Right);

				// ���س�鸦 �߽����� ���� ȸ���� �����ش�.
				Node = RotationLeft(Node);
			}
		}

		// �������� ������ �������� ���
		else if (Factor >= 2)
		{
			int	LeftFactor = BalanceFactor(Node->m_Left);

			// ���� �������� ������ �������� ���
			if (LeftFactor >= 0)
			{
				// ���س�带 �߽����� ���������� ȸ���Ѵ�.
				Node = RotationRight(Node);
			}

			// ���� ���������� ������ �������� ���
			else
			{
				// ���س���� ���� �ڽĳ�带 �߽����� �������� ȸ���Ѵ�.
				RotationLeft(Node->m_Left);

				// ���س�带 �߽����� ���������� ȸ���Ѵ�.
				Node = RotationRight(Node);
			}
		}

		ReBalance(Node->m_Parent);
	}
};

