#pragma once

template<typename Key, typename Value>
class CBinaryTreeNode {
	template<typename Key, typename Value>
	friend class CBinaryTree;
	template<typename Key, typename Value>
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
	CBinaryTreeNode<Key, Value>* m_Left;
	CBinaryTreeNode<Key, Value>* m_Right;
	CBinaryTreeNode<Key, Value>* m_Parent;
	CBinaryTreeNode<Key, Value>* m_Next;
	CBinaryTreeNode<Key, Value>* m_Prev;
private :
	Key first;
	Value second;
};

template<typename Key, typename Value>
class CBinaryTreeIterator {
	template<typename Key, typename Value>
	friend class CBinaryTree;
private :
	CBinaryTreeNode<Key, Value>* m_Node;
public :
	bool operator == (const CBinaryTreeIterator<Key, Value>& iter)
{
		return m_Node == iter.m_Node;
}
	bool operator != (const CBinaryTreeIterator<Key, Value>& iter)
	{
		return m_Node != iter.m_Node;
	}
	bool operator == (const CBinaryTreeNode<Key, Value>* Node)
	{
		return m_Node == Node;
	}
	bool operator != (const CBinaryTreeNode<Key, Value>* Node)
	{
		return Node != Node;
	}
	void operator ++ ()
{
		m_Node = m_Node->m_Next;
}
	void operator -- ()
{
		m_Node = m_Node->m_Prev;
}
	CBinaryTreeNode<Key, Value>* operator -> ()
{
		return m_Node;
}
};

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
	typedef CBinaryTreeNode<KEY, VALUE>*    PNODE;

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
		if (!m_Root)
		{
			m_Root = new NODE;
			m_Root->first = key;
			m_Root->second = value;

			m_Begin->m_Next = m_Root;
			m_Root->m_Prev = m_Begin;

			m_Root->m_Next = m_End;
			m_End->m_Prev = m_Root;

			return;
		}
		else
			insert(key, value, m_Root);

		++m_Size;
	};
	int size()	const
	{
		return m_Size;
	}

	bool empty()	const
	{
		return m_Size == 0;
	}
	iterator begin() const
	{
		iterator result;
		result.m_Node = m_Begin->m_Next;
		return result;
	}
	iterator end() const
	{
		iterator result;
		result.m_Node = m_End;
		return result;
	}
	iterator Find(const KEY& key) const
	{
		return Find(key, m_Root);
	}
	void clear()
	{
		PNODE DeleteNode = m_Begin->m_Next;

		while (DeleteNode)
		{
			PNODE Next = DeleteNode->m_Next;
			delete DeleteNode;
			DeleteNode = Next;
		}

		m_Size = 0;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
	}
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
		iterator result = Find(key);

		if (result == end())
			return result;

		return erase(result);
	}
	iterator erase(iterator& iter)
	{
		if (iter == end() || iter.m_Node == m_Begin)
			return end();
		if (iter.m_Node == nullptr)
			return end();

		// 자식이 없을 때

		// 
	}
private :
	iterator Find(const KEY& key, PNODE Node) const
	{
		if (!Node)
			return end();

		if (key == Node->first)
		{
			iterator result;
			result.m_Node = Node;
			return result;
		}

		if (key < Node->first)
			return Find(key, Node->first);

		return Find(key, Node->m_Right);
	}
	void insert(const KEY& key, const VALUE& value,  PNODE Node)
{
	if (Node->first > key)
	{
		// 왼쪽
		if (Node->m_Left)
			return insert(key, value, Node->m_Left);

		PNODE NewNode = new NODE;
		NewNode->first = key;
		NewNode->second = value;

		Node->m_Left = NewNode;
		NewNode->m_Parent = Node;

		PNODE PrevNode = Node->m_Prev;
		PrevNode->m_Next = NewNode;
		NewNode->m_Prev = PrevNode;

		NewNode->m_Next = Node;
		Node->m_Prev = NewNode;

		return;
	}

		// 오른쪽
	if (Node->m_Right)
		return insert(key, value, Node->m_Right);


	PNODE NewNode = new NODE;
	NewNode->first = key;
	NewNode->second = value;

	Node->m_Right = NewNode;
	NewNode->m_Parent = Node;

	PNODE NextNode = Node->m_Next;
	NextNode->m_Prev = NewNode;
	NewNode->m_Next = NextNode;

	NewNode->m_Prev = Node;
	Node->m_Next = NewNode;

	return;
}
};