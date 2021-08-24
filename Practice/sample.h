#pragma once

template<typename Key,typename Value>
class CBinaryTreeNode
{
	template<typename Key, typename Value>
	class CBinaryTree;
	template<typename Key, typename Value>
	class CBinaryTreeIterator;
public :
	CBinaryTreeNode()
	{
		m_Next   = nullptr;
		m_Prev   = nullptr;
		m_Right  = nullptr;
		m_Left   = nullptr;
		m_Parent = nullptr;
	}
private :
	CBinaryTreeNode<Key, Value>* m_Next;
	CBinaryTreeNode<Key, Value>* m_Prev;
	CBinaryTreeNode<Key, Value>* m_Right;
	CBinaryTreeNode<Key, Value>* m_Left;
	CBinaryTreeNode<Key, Value>* m_Parent;
public :
	Key first;
	Value second;
};

template<typename Key,typename Value>
class CBinaryTreeIterator
{
	template<typename Key, typename Value>
	class CBinaryTree;
public :
	CBinaryTreeIterator():m_Node(nullptr){}
private :
	CBinaryTreeNode<Key, Value>* m_Node;
public :
	bool operator == (const CBinaryTreeNode<Key, Value>* Node) const
	{
		return m_Node == Node;
	}
	bool operator != (const CBinaryTreeNode<Key, Value>* Node) const
	{
		return m_Node != Node;
	}
	bool operator == (const CBinaryTreeIterator<Key, Value>& iter) const
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CBinaryTreeIterator<Key, Value>& iter) const
	{
		return m_Node != iter.m_Node;
	}
	void operator ++ () const
	{
		m_Node = m_Node->m_Next;
	}
	void operator ++ (int) const
	{
		m_Node = m_Node->m_Next;
	}
	void operator -- () const
	{
		m_Node = m_Node->m_Prev;
	}
	void operator -- (int) const
	{
		m_Node = m_Node->m_Prev;
	}
	CBinaryTreeNode<Key, Value>* operator -> () const
	{
		return m_Node;
	}
};

template<typename Key, typename Value>
class CBinaryTree
{
private :
	typedef CBinaryTreeNode<Key, Value> NODE;
	typedef CBinaryTreeNode<Key, Value>* PNODE;
public :
	typedef CBinaryTreeIterator<Key, Value> iterator;
public :
	CBinaryTree()
	{
		m_Root = nullptr;
		m_Begin = new NODE;
		m_End = new NODE;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
		m_Size = 0;
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
	PNODE m_Root;
	PNODE m_Begin;
	PNODE m_End;
	int m_Size;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void clear()
	{
		PNODE DeleteNode = m_Begin->m_Next;
		while (DeleteNode != m_End)
		{
			PNODE Next = DeleteNode->m_Next;
			delete DeleteNode;
			DeleteNode = Next;
		}
	}
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
	// ¼øÈ¸
	void PreOrder(void (*pFunc)(const Key&, const Value&))
	{
		PreOrder(pFunc, m_Root);
	}
	void InOrder(void (*pFunc)(const Key&, const Value&))
	{
		InOrder(pFunc, m_Root);
	}
	void PostOrder(void (*pFunc)(const Key&, const Value&))
	{
		PostOrder(pFunc, m_Root);
	}
	// »ðÀÔ
	PNODE insert(const Key& key, const Value& value)
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
			
			return m_Root;
		}
		else
			return insert(key, value, m_Root);
		++m_Size;
	}
	iterator erase(const Key& key)
	{
		iterator iter = Find(key);
		if (iter == end()) return end();
		return erase(iter);
	}
	iterator erase(iterator& iter)
	{
		if (iter == end() || iter == m_Begin) return end();
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			PNODE Parent = iter.m_Node->m_Parent;
			if (!Parent)
			{
				delete iter.m_Node;

				m_Begin->m_Next = m_End;
				m_End->m_Prev = m_Begin;

				m_Size = 0;
				m_Root = nullptr;
			}

			iter.m_Node->m_Parent = nullptr;
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

			iterator result;
			result.m_Node = Next;
			return result;
		}
		// ¿ÞÂÊ --> ¿ÞÂÊ¿¡¼­ °¡Àå Å«¾Ö Ã£±â
		if (iter.m_Node->m_Left)
		{
			PNODE MaxNode = FindMax(iter.m_Node->m_Left);
			PNODE Parent = MaxNode->m_Parent;
			PNODE LeftChild = MaxNode->m_Left;

			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

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
			retun result;
		}
		if (iter.m_Node->m_Right)
		{
			PNODE MinNode = FindMin(iter.m_Node->m_Right);
			PNODE Parent = MaxNode->m_Parent;
			PNODE LeftChild = MaxNode->m_Left;

			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

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
			retun result;
		}
	}
	iterator Find(const Key& key)
	{
		return Find(key, m_Root);
	}
private :
	iterator Find(const Key& key, PNODE Node)
	{
		if (!Node) return end();
		if (key < Node->first)
		{
			return Find(key, Node->m_Left);
		}
		if (key > Node->first)
		{
			return Find(key, Node->m_Right);
		}
		if (key == Node->first)
		{
			iterator result;
			result.m_Node = Node;
			return result;
		}
	}
	PNODE insert(const Key& key, const Value& value, PNODE Node)
	{
		// ¿ÞÂÊ ÀÖÀ» ¶§
		if (Node->first > key)
		{
			if (Node->m_Left)
				return insert(key, value, Node->m_Left);
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
			return NewNode;
		}
		if (Node->m_Right)
			return insert(key, value, Node->m_Right);
		PNODE NewNode = new NODE;
		NewNode->first = key;
		NewNode->second = value;

		Node->m_Right = NewNode;
		NewNode->m_Parent = Node;

		PNODE Next = Node->m_Next;
		Next->m_Prev = NewNode;
		NewNode->m_Next = Next;

		NewNode->m_Prev = Node;
		Node->m_Next = NewNode;
		return NewNode;
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
	void PreOrder(void (*pFunc)(const Key&, const Value&), PNODE Node)
	{
		if (!Node) return;
		// root - l - r
		pFunc(Node->first, Node->second);
		PreOrder(pFunc, Node->m_Left);
		PreOrder(pFunc, Node->m_Right);
	}
	void InOrder(void (*pFunc)(const Key&, const Value&), PNODE Node)
	{
		if (!Node) return;
		// l - root - r
		InOrder(pFunc, Node->m_Left);
		pFunc(Node->first, Node->second);
		InOrder(pFunc, Node->m_Right);
	}
	void PostOrder(void (*pFunc)(const Key&, const Value&), PNODE Node)
	{
		if (!Node) return;
		// l - r - root
		PostOrder(pFunc, Node->m_Left);
		PostOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second);
	}
};