#pragma once
#include<assert.h>

template<typename KEY, typename VALUE>
class CAVLTreeNode
{
	template<typename KEY, typename VALUE>
	friend class CAVLTreeIterator;
	template<typename KEY, typename VALUE>
	friend class CAVLTree;
public :
	CAVLTreeNode()
	{
		m_Next = nullptr;
		m_Prev = nullptr;
		m_Left = nullptr;
		m_Right = nullptr;
		m_Right = nullptr;
	}
	~CAVLTreeNode()
	{

	}
private :
	CAVLTreeNode<KEY, VALUE>* m_Next;
	CAVLTreeNode<KEY, VALUE>* m_Prev;
	CAVLTreeNode<KEY, VALUE>* m_Left;
	CAVLTreeNode<KEY, VALUE>* m_Right;
	CAVLTreeNode<KEY, VALUE>* m_Parent;
public :
	KEY first;
	VALUE second;
public :
	bool IsParent()
	{
		return m_Parent != nullptr;
	}
	KEY GetParentKey()
	{
		return m_Parent->first;
	}
	VALUE GetParentValue()
	{
		return m_Parent->second;
	}
	bool IsLeft()
	{
		return m_Left != nullptr;
	}
	KEY GetLeftKey()
	{
		return m_Left->first;
	}
	VALUE GetLeftValue()
	{
		return m_Left->second;
	}
	bool IsRight()
	{
		return m_Right != nullptr;
	}
	KEY GetRightKey()
	{
		return m_Right->first;
	}
	VALUE GetRightValue()
	{
		return m_Right->second;
	}

};

template<typename KEY, typename VALUE>
class CAVLTreeIterator
{
	template<typename KEY, typename VALUE>
	friend class CAVLTree;
public :
	CAVLTreeIterator()
	{
		m_Node = nullptr;
	}
	~CAVLTreeIterator()
	{

	}
private :
	CAVLTreeNode<KEY, VALUE>* m_Node;
public :
	bool operator == (const CAVLTreeNode<KEY, VALUE>* Node) const 
	{
		return m_Node == Node;
	}
	bool operator != (const CAVLTreeNode<KEY, VALUE>* Node) const 
	{
		return m_Node != Node;
	}
	bool operator == (const CAVLTreeIterator<KEY, VALUE>& iter) const 
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CAVLTreeIterator<KEY, VALUE>& iter) const 
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
	CAVLTreeNode<KEY, VALUE>* operator ->()
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
		m_Size = 0;
		m_Root = nullptr;
		m_Begin = new NODE;
		m_End = new NODE;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
	}
	~CAVLTree()
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
	CAVLTreeNode<KEY, VALUE>* m_Root;
	CAVLTreeNode<KEY, VALUE>* m_Begin;
	CAVLTreeNode<KEY, VALUE>* m_End;
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
		m_Size = 0;
		m_Root = nullptr;
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
	iterator Find(const KEY& key)
	{
		return Find(key, m_Root);
	}
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
		else insert(key, value, m_Root);
		++m_Size;
	}
	// 전위 순회
	void PreOrder(void (*pFunc)(const KEY&, const VALUE &))
	{
		PreOrder(pFunc,m_Root);
	}
	// 중위 순회
	void InOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		InOrder(pFunc, m_Root);
	}
	// 후위 순회 
	void PostOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		PostOrder(pFunc, m_Root);
	}
	iterator erase(const KEY& key)
	{
		iterator iter = Find(key);
		if (iter == end()) return end();
		return erase(iter);
	}
	iterator erase(iterator& iter)
	{
		if (iter == end() || iter == m_Begin || iter.m_Node == nullptr)
			return end();
		
		// 리프노드 일 때
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			PNODE Parent = iter.m_Node->m_Parent;

			// 부모가 없을 때 --> 해당 노드가 루트 노드
			if (!Parent)
			{
				m_Root = nullptr;
				m_Begin->m_Next = m_End;
				m_End->m_Prev = m_Begin;
				--m_Size;
				delete iter.m_Node;
				return end();
			}

			if (Parent->m_Left == iter.m_Node)
				Parent->m_Left = nullptr;
			else
				Parent->m_Right = nullptr;

			PNODE Next = iter.m_Node->m_Next;
			PNODE Prev = iter.m_Node->m_Prev;
			Next->m_Prev = Prev;
			Prev->m_Next = Next;

			delete iter.m_Node;
			--m_Size;
			ReBalance(Parent);
			iterator result;
			result.m_Node = Next;
			return result;
		}

		// 왼쪽 노드가 존재할 때
		if (iter.m_Node->m_Left)
		{
			PNODE MaxNode = FindMax(iter.m_Node->m_Left);
			PNODE Parent = MaxNode->m_Parent;
			PNODE LeftChild = MaxNode->m_Left;

			// 값 세팅
			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

			// 부모,자식 노드 세팅
			if (Parent->m_Left == MaxNode)
				Parent->m_Left = LeftChild;
			else
				Parent->m_Right = LeftChild;
			if (LeftChild)
				LeftChild->m_Parent = Parent;

			// 리스트 세팅
			PNODE Prev = MaxNode->m_Prev;
			PNODE Next = MaxNode->m_Next;
			Prev->m_Next = Next;
			Next->m_Prev = Prev;

			// 지워주기
			delete MaxNode;
			--m_Size;
			ReBalance(Parent);
			iterator result;
			result.m_Node = Next;
			return result;
		}

		// 오른쪽 노드가 존재할 때
		PNODE MinNode = FindMin(iter.m_Node->m_Right);
		PNODE Parent = MinNode->m_Parent;
		PNODE RightChild = MinNode->m_Right;

		// 값 세팅
		iter.m_Node->first = MinNode->first;
		iter.m_Node->second = MinNode->second;

		// 부모,자식 노드 세팅
		if (Parent->m_Left == MinNode)
			Parent->m_Left = RightChild;
		else
			Parent->m_Right = RightChild;
		if (RightChild)
			RightChild->m_Parent = Parent;

		// 리스트 세팅
		PNODE Prev = MinNode->m_Prev;
		PNODE Next = MinNode->m_Next;
		Prev->m_Next = Next;
		Next->m_Prev = Prev;

		// 지워주기
		delete MinNode;
		--m_Size;
		ReBalance(Parent);

		iterator result;
		result.m_Node = Next;
		return result;

	}
private :
	void ReBalance(PNODE Node)
	{
		if (!Node) return;
		int Factor = BalaceFactor(Node);
		// 오
		if (Factor <= -2)
		{
			int RightFactor = BalaceFactor(Node->m_Right);
			// 오오
			if (RightFactor <= 0)
			{
				Node = RotationLeft(Node);
			}
			// 오왼
			else
			{
				RotationRight(Node->m_Right);
				Node = RotationLeft(Node);
			}
		}
		// 왼
		else if (Factor >= 2)
		{
			int LeftFactor = BalaceFactor(Node->m_Left);
			// 오오
			if (LeftFactor >= 0)
			{
				Node = RotationRight(Node);
			}
			// 오왼
			else
			{
				RotationLeft(Node->m_Left);
				Node = RotationRight(Node);
			}
		}
		ReBalance(Node->m_Parent);
	}
	PNODE RotationLeft(PNODE Node)
	{
		PNODE Parent = Node->m_Parent;
		PNODE RightChild = Node->m_Right;
		PNODE RightLeftChild = RightChild->m_Left;

		// 기준-자식
		Node->m_Parent = RightChild;
		RightChild->m_Left = Node;

		// 자식-부모
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

		// 기준-자식자식
		Node->m_Right = RightLeftChild;
		if (RightLeftChild) RightLeftChild->m_Parent = Node;
		return RightChild;

	}
	PNODE RotationRight(PNODE Node)
	{
		PNODE Parent = Node->m_Parent;
		PNODE LeftChild = Node->m_Left;
		PNODE LeftRightChild = LeftChild->m_Right;

		// 기준-자식
		Node->m_Parent = LeftChild;
		LeftChild->m_Right = Node;

		// 자식-부모
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

		// 기준-자식자식
		Node->m_Left = LeftChild;
		if (LeftRightChild) LeftRightChild->m_Parent = Node;
		
		return LeftChild;
	}
	int BalaceFactor(PNODE Node)
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
		if (Node->m_Left) return FindMax(Node->m_Left);
		return Node;
	}
	void PreOrder(void (*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node) return;
		pFunc(Node->first, Node->second);
		PreOrder(pFunc, Node->m_Left);
		PreOrder(pFunc, Node->m_Right);
	}
	void InOrder(void (*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node) return;
		InOrder(pFunc, Node->m_Left);
		pFunc(Node->first, Node->second);
		InOrder(pFunc, Node->m_Right);
	}
	void PostOrder(void (*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node) return;
		PostOrder(pFunc, Node->m_Left);
		PostOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second);
	}
	PNODE insert(const KEY& key, const VALUE& value, PNODE Node)
	{
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

			ReBalance(Node);
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

		ReBalance(Node);

		return NewNode;
	}
	iterator Find(const KEY& key, PNODE Node)
	{
		if (!Node) return end();
		if (Node->first == key)
		{
			iterator iter;
			iter.m_Node = Node;
			return iter;
		}
		if (Node->first > key)
			return Find(key, Node->m_Left);
		return Find(key, Node->m_Right);
	}
};