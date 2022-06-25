#pragma once

template <typename KEY, typename VALUE>
class CBinaryTreeNode
{
	template <typename KEY, typename VALUE>
	friend class CBinaryTree;
	template <typename KEY, typename VALUE>
	friend class CBinaryTreeIterator;

private:
	CBinaryTreeNode() :
		m_Left(nullptr),
		m_Right(nullptr),
		m_Parent(nullptr),
		m_Next(nullptr),
		m_Prev(nullptr)
	{
	}

	~CBinaryTreeNode()
	{
	}

private:
	CBinaryTreeNode<KEY, VALUE>* m_Left;
	CBinaryTreeNode<KEY, VALUE>* m_Right;
	CBinaryTreeNode<KEY, VALUE>* m_Parent;
	CBinaryTreeNode<KEY, VALUE>* m_Next;
	CBinaryTreeNode<KEY, VALUE>* m_Prev;

public:
	KEY		first;
	VALUE	second;
};

template <typename KEY, typename VALUE>
class CBinaryTreeIterator
{
	template <typename KEY, typename VALUE>
	friend class CBinaryTree;

public:
	CBinaryTreeIterator() :
		m_Node(nullptr)
	{
	}

	~CBinaryTreeIterator()
	{
	}

private:
	CBinaryTreeNode<KEY, VALUE>* m_Node;

public:
	// iterator���� ���� ������ �ִ� ��尡 ���� ��� ���ٰ� �Ǵ��Ѵ�.
	bool operator == (const CBinaryTreeIterator<KEY, VALUE>& iter)	const
	{
		return m_Node == iter.m_Node;
	}

	bool operator != (const CBinaryTreeIterator<KEY, VALUE>& iter)	const
	{
		return m_Node != iter.m_Node;
	}

	bool operator == (const CBinaryTreeNode<KEY, VALUE>* Node)	const
	{
		return m_Node == Node;
	}

	bool operator != (const CBinaryTreeNode<KEY, VALUE>* Node)	const
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

	CBinaryTreeNode<KEY, VALUE>* operator -> ()
	{
		return m_Node;
	}
};

// Key�� Ž���� �ϱ� ���� Ÿ���̴�.
// Value�� ���� �����ϱ� ���� ������ Ÿ���̴�.
// �׷��� Ž���� �Ҷ��� KeyŸ������ �Ѵ�.
// ���� KeyŸ���� ���ڿ��̶�� ���ڿ��� Ž���� �� �� �ִ� ���̴�.
template <typename KEY, typename VALUE>
class CBinaryTree
{
public:
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
		PNODE	DeleteNode = m_Begin;

		while (DeleteNode)
		{
			PNODE	Next = DeleteNode->m_Next;

			delete	DeleteNode;

			DeleteNode = Next;
		}
	}

private:
	typedef CBinaryTreeNode<KEY, VALUE>		NODE;
	typedef CBinaryTreeNode<KEY, VALUE>* PNODE;

public:
	typedef CBinaryTreeIterator<KEY, VALUE>	iterator;

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
			insert(key, value, m_Root); //
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

		// ��������� ��� �θ�κ��� ������ �����ϰ�
		// ��带 �������ش�.
		if (!iter.m_Node->m_Left && 
		!iter.m_Node->m_Right)
		{
			// �θ��带 ���´�.
			PNODE	Parent = iter.m_Node->m_Parent;

			// ���� �θ��尡 ���ٸ� 
			// ���� ������� ���� ��Ʈ����� ���̴�.
			// �׷��� ��Ʈ��尡 ��������� ���� 
			// �� ��� 1���� ���Ҵٴ�
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

			// ������� ��尡 �θ������ 
			// ���� ������� ������ ���������
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


			// ���� ����� ������带 �־ 
			// iterator�� ��ȯ���ش�.
			iterator	result;
			result.m_Node = Next;

			return result;
		}

		// ���� ���� ��尡 ���� ��� 
		// ���� ��忡�� ���� ū ��带 ã�Ƽ�
		// ���� ����� ���� �������ְ� 
		// ã���� ��带 �����Ѵ�.
		if (iter.m_Node->m_Left)
		{
			// ���ʿ� �����ϴ� ������ ���� ū ��带 ã�ƿ´�.
			PNODE	MaxNode = FindMax(iter.m_Node->m_Left);

			// ã���� ����� Key, Value ������ �������ش�.
			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

			// ã���� ��带 �����ؾ��ϱ� ������ 
			// �θ�κ��� ������ ����
			// �������ֵ��� �Ѵ�.
			// ��, ã���� ��尡 ���� �ڽĳ�尡 
			// �������� �����Ƿ�
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

			iterator	result;
			result.m_Node = Next;

			return result;
		}

		// ���� ����� ������ ��常 ������ ��� 
		// ������ ��忡�� ���� ���� ��带
		// ã�ƿ´�.
		PNODE	MinNode = FindMin(iter.m_Node->m_Right);

		// ã���� ����� Key, Value ������ �������ش�.
		iter.m_Node->first = MinNode->first;
		iter.m_Node->second = MinNode->second;

		// ã���� ��带 �����ؾ��ϱ� ������ 
		// �θ�κ��� ������ ����
		// �������ֵ��� �Ѵ�.
		// ��, ã���� ��尡 ������ �ڽĳ�尡 
		// �������� �����Ƿ�
		PNODE	RightChild = MinNode->m_Right;
		PNODE	Parent = MinNode->m_Parent;

		if (Parent->m_Left == MinNode)
			Parent->m_Left = RightChild;

		else
			Parent->m_Right = RightChild;

		// ������ �ڽĳ�尡 ���� ����� 
		// �θ�� MinNode�� �θ�
		// �������ֵ��� �Ѵ�.
		if (RightChild)
			RightChild->m_Parent = Parent;

		PNODE	Prev = MinNode->m_Prev;
		PNODE	Next = MinNode->m_Next;

		Prev->m_Next = Next;
		Next->m_Prev = Prev;

		delete	MinNode;

		--m_Size;

		iterator	result;
		result.m_Node = Next;

		return result;
	}

private:
	PNODE insert(const KEY& key, 
	const VALUE& value, PNODE Node)
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

	void PreOrder(void(*pFunc)(const KEY&, 
	const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		pFunc(Node->first, Node->second);
		PreOrder(pFunc, Node->m_Left);
		PreOrder(pFunc, Node->m_Right);
	}

	void InOrder(void(*pFunc)(const KEY&, 
	const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		InOrder(pFunc, Node->m_Left);
		pFunc(Node->first, Node->second);
		InOrder(pFunc, Node->m_Right);
	}

	void PostOrder(void(*pFunc)(const KEY&, 
	const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		PostOrder(pFunc, Node->m_Left);
		PostOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second); //
	}

	PNODE FindMax(PNODE Node)
	{
		if (Node->m_Right)
			return FindMax(Node->m_Right); //

		return Node;
	}

	PNODE FindMin(PNODE Node)
	{
		if (Node->m_Left)
			return FindMin(Node->m_Left);

		return Node;
	}
};

