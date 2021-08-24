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
		m_Next = nullptr;
		m_Prev = nullptr;
		m_Left = nullptr;
		m_Right = nullptr;
		m_Parent = nullptr;
	}
private :
	CAVLTreeNode<T>* m_Next;
	CAVLTreeNode<T>* m_Prev;
	CAVLTreeNode<T>* m_Left;
	CAVLTreeNode<T>* m_Right;
	CAVLTreeNode<T>* m_Parent;
public :
	KEY first;
	VALUE second;
public :
	bool IsParent() { return m_Parent != nullptr; }
	bool IsLeft() { return m_Left != nullptr; }
	bool IsRight() { return m_Right != nullptr; }
	KEY GetParentKey() { return m_Parent->first; }
	KEY GetLeftKey() { return m_Left->first; }
	KEY GetRightKey() { return m_Right->first; }
	VALUE GetParentValue() { return m_Parent->second ; }
	VALUE GetLeftValue() { return m_Left->second; }
	VALUE GetRightValue() { return m_Right->second; }
};

template<typename KEY, typename VALUE>
class CAVLTreeIterator
{
public :
	CAVLTreeIterator() : m_Node(nullptr){}
private :
	CAVLTree<KEY, VALUE>* m_Node;
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
	bool operator != (const CAVLTreeNode<KEY, VALUE>* Node) const
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
	typedef CAVLTreeNode<KEY, VALUE> NODE;
	typedef CAVLTreeNode<KEY, VALUE>* PNODE;
public :
	typedef CAVLTreeIterator<KEY, VALUE> iterator;
public :
	CAVLTree()
	{
		m_Begin = new NODE;
		m_End = new NODE;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
		m_Root = nullptr;
		m_Size = 0;
	}
	~CAVLTree()
	{
		PNODE Node = m_Begin;
		while (Node)
		{
			PNODE Next = Node->m_Next;
			delete Node;
			Node = Node->m_Next;
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
		PNODE Node = m_Begin->m_Next;
		while (Node != m_End)
		{
			PNODE Next = Node->m_Next;
			delete Node;
			Node = Node->m_Next;
		}
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
		m_Root = nullptr;
		m_Size = 0l
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
	// 순회
	void PreOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		PostOrder(pFunc, m_Root);
	}
	void InOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		InOrder(pFunc, m_Root);
	}
	void PostOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		PostOrder(pFun, m_Root);
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
		}
		else
			insert(key, value, m_Root);

		++m_Size;
	}
	// 삭제
	iterator Find(const KEY& key)
	{
		iterator iter = begin();
		itreator iterEnd = end();
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->first == key) return iter;
		}
		return end();
	}
	iterator erase(const KEY& key)
	{
		iterator iter = Find(key);
		if (iter == end()) return end();
		return erase(iter);
	}
	iterator erase(iterator& iter)
	{
		if (iter == end() || iter == m_Begin) return end();
		// 리프 노드
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			PNODE Parent = iter.m_Node->m_Parent;
			if (!Parent)
			{
				// 해당 노드가 루트 노드
				delete iter.m_Node;
				m_Root = nullptr;
				m_Begin->m_Next = m_End;
				m_End->m_Prev = m_Begin;
				--m_Size;
				return end();
			}
			PNODE Prev = iter.m_Node->m_Prev;
			PNODE Next = iter.m_Node->m_Next;
			Prev->m_Next = Next;
			Next->m_Prev = Prev;

			if (Parent->m_Left == iter.m_Node)
				Parent->m_Left = nullptr;
			else
				Parent->m_Right = nullptr;

			--m_Size;
			delete iter.m_Node;

			iterator result;
			result.m_Node = Next;
			return result;

		}

		// 왼쪽
		if (iter.m_Node->m_Left)
		{
			PNODE MaxNode = FindMax(iter.m_Node->m_Left);
			PNODE Parent = MaxNode->m_Parent;
			PNODE LeftChild = MaxNode->m_Left;

			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;


			
		}

		// 오른쪽
	}
private :
	void ReBalance(){}
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
	void insert(const KEY& key, const VALUE& value, PNODE Node)
	{
		if (key < Node->first)
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
		}
		if (Node->m_Right) insert(key, value, Node->m_Right);
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

	}
	void PreOrder(void (*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node) return;
		pFunc(Node->first, Node->second;);
		PreOrder(pFunc, Node->m_Left);
		PreOrder(pFunc, Node->m_Right);
	}
	void InOrder(void (*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if(!Node) return;
		InOrder(pFunc, Node->m_Left);
		pFunc(Node->first, Node->second;);
		InOrder(pFunc, Node->m_Right);
	}
	void PostOrder(void (*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if(!Node) return;
		PostOrder(pFunc, Node->m_Left);
		PostOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second;);
	}
};



