#pragma once

template<typename KEY, typename VALUE>
class CBinaryTreeNode
{
	template<typename KEY, typename VALUE>
	friend class CBinaryTreeIterator;
	template<typename KEY, typename VALUE>
	friend class CBinaryTree;
public :
	CBinaryTreeNode() :m_Parent(nullptr), m_Right(nullptr), m_Left(nullptr), m_Prev(nullptr), m_Next(nullptr) {};
	~CBinaryTreeNode() {};
private :
	CBinaryTreeNode<KEY, VALUE>* m_Parent;
	CBinaryTreeNode<KEY, VALUE>* m_Right;
	CBinaryTreeNode<KEY, VALUE>* m_Left;
	CBinaryTreeNode<KEY, VALUE>* m_Prev;
	CBinaryTreeNode<KEY, VALUE>* m_Next;
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
	CBinaryTreeIterator() : m_Node(nullptr) {}
	~CBinaryTreeIterator(){}
private :
	CBinaryTreeNode<KEY, VALUE>* m_Node;
public :
	bool operator == (const CBinaryTreeNode<KEY, VALUE>* Node)
	{
		return m_Node == Node;
	}
	bool operator != (const CBinaryTreeNode<KEY, VALUE>* Node)
	{
		return m_Node != Node;
	}
	bool operator == (const CBinaryTreeIterator<KEY, VALUE> &iter )
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CBinaryTreeIterator<KEY, VALUE> &iter )
	{
		return m_Node != iter.m_Node;
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
	CBinaryTreeNode<KEY, VALUE>* operator ->()
	{
		return m_Node;
	}
};

template<typename KEY, typename VALUE>
class CBinaryTree
{
private :
	typedef CBinaryTreeNode<KEY, VALUE> NODE;
	typedef CBinaryTreeNode<KEY, VALUE>* PNODE;
public :
	typedef CBinaryTreeIterator<KEY, VALUE> iterator;
public :
	CBinaryTree()
	{
		m_Size = 0;
		m_Root = nullptr;

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
private :
	int m_Size;
	PNODE m_Begin;
	PNODE m_End;
	PNODE m_Root;
public :
	iterator begin() const {
		iterator iter;
		iter.m_Node = m_Begin->m_Next;
		return iter;
	}
	iterator end() const {
		iterator iter;
		iter.m_Node = m_End;
		return iter;
	}
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void insert(const KEY& key, const VALUE& value)
	{
		// ó���� �ƹ��͵� ���ٴ� ��
		if (!m_Root)
		{
			m_Root = new NODE;
			m_Root->first = key;
			m_Root->second = value;

			m_Root->m_Prev = m_Begin;
			m_Begin->m_Next = m_Root;

			m_Root->m_Next = m_End;
			m_End->m_Prev = m_Root;
		}
		insert(key, value, m_Root);
		++m_Size;
	}
	iterator Find(const KEY & key)
	{
		return Find(key, m_Root);
	}
private :
	PNODE insert(const KEY& key, const VALUE& value, PNODE Node)
	{
		// �ش� Node key���� �۴ٸ�, ���� // ũ�� ������
		if (Node->first > key)
		{
			// ���� �ڽ��� �ִٸ�, �ش� ���� �ڽĿ��� insert �Լ� ����
			if (Node->m_Left)
				return insert(key, value, Node->m_Left);
			else
			{
				// �θ�,�ڽ� ����
				PNODE NewNode = new NODE;
				NewNode->m_Parent = Node;
				Node->m_Left = NewNode;

				// ������ ����
				NewNode->first = key;
				NewNode->second = value;

				// ����Ʈ 
				PNODE Prev = Node->m_Prev;
				Prev->m_Next = NewNode;
				NewNode->m_Prev = Prev;

				Node->m_Prev = NewNode;
				NewNode->m_Next = Node;
				return NewNode ;
			}
		}
		// ������� ���� Node�� key���� ũ�ٴ� �� 
		if(Node->m_Right)
			return insert(key, value, Node->m_Right);
		else
		{
			// �θ�,�ڽ� ����
			PNODE NewNode = new NODE;
			NewNode->m_Parent = Node;
			Node->m_Right = NewNode;

			// ������ ����
			NewNode->first = key;
			NewNode->second = value;

			// ����Ʈ 
			PNODE Next = Node->m_Next;
			Next->m_Prev = NewNode;
			NewNode->m_Next = Next;

			NewNode->m_Prev = Node;
			Node->m_Next = NewNode;

			return NewNode;
		}
	}
	iterator Find(const KEY& key, PNODE Node)
	{
		if (!Node) { return end(); }
		if (key == Node->first)
		{
			iterator iter;
			iter.m_Node = Node;
			return iter;
		}
		if (key < Node->first)
			return Find(key, Node->m_Left);
		return Find(key, Node->m_Right);
	}

};