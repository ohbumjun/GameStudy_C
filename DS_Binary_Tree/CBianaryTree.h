#pragma once
#include<assert.h>

template<typename KEY,typename VALUE>
class CBinaryTreeNode
{
	template<typename KEY, typename VALUE>
	friend class CBinaryTreeIterator;
	template<typename KEY, typename VALUE>
	friend class CBinaryTree;
public :
	CBinaryTreeNode(){
		m_Parent = nullptr;
		m_Prev = nullptr;
		m_Next = nullptr;
		m_Right = nullptr;
		m_Left = nullptr;
	}
	~CBinaryTreeNode(){}
private :
	CBinaryTreeNode<KEY, VALUE>* m_Parent;
	CBinaryTreeNode<KEY, VALUE>* m_Prev;
	CBinaryTreeNode<KEY, VALUE>* m_Next;
	CBinaryTreeNode<KEY, VALUE>* m_Right;
	CBinaryTreeNode<KEY, VALUE>* m_Left;
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
	CBinaryTreeIterator():m_Node(nullptr){}
	~CBinaryTreeIterator(){}
private : 
	CBinaryTreeNode<KEY, VALUE>* m_Node;
public :
	bool operator == (CBinaryTreeIterator<KEY, VALUE>& iter)
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (CBinaryTreeIterator<KEY, VALUE>& iter)
	{
		return m_Node != iter.m_Node;
	}
	bool operator == (CBinaryTreeNode<KEY, VALUE>* Node)
	{
		return m_Node == Node;
	}
	bool operator != (CBinaryTreeNode<KEY, VALUE>* Node)
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


template<typename KEY, typename VALUE>
class CBinaryTree
{
private :
	typedef CBinaryTreeNode<KEY, VALUE> NODE;
	typedef CBinaryTreeNode<KEY, VALUE>* PNODE;
public :
	typedef CBinaryTreeIterator<KEY, VALUE> iterator;
private :
	int m_Size;
	CBinaryTreeNode<KEY, VALUE>* m_Begin;
	CBinaryTreeNode<KEY, VALUE>* m_End;
	CBinaryTreeNode<KEY, VALUE>* m_Root;
public :
	CBinaryTree()
	{
		m_Size = 0;
		m_Begin = new NODE;
		m_End = new NODE;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
		m_Root = nullptr;
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
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	iterator begin()
	{
		iterator iter;
		iter.m_Node = m_Begin->m_Next;
		return iter;
	}
	iterator end()
	{
		iterator iter;
		iter.m_Node = m_End;
		return iter;
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

			m_End->m_Prev = m_Root;
			m_Root->m_Next = m_Begin;
		}
		else insert(key,value,m_Root);
		++m_Size;
	}
	// ���� : root -> left -> right 
	void PreOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		PreOrder(pFunc, m_Root);
	}
	// ���� : left -> root -> right
	void InOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		InOrder(pFunc, m_Root);
	}
	// ���� : left -> right -> root 
	void PostOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		PostOrder(pFunc, m_Root);
	}
	iterator erase(const KEY& key)
	{
		iterator iter = Find(key);
		if (iter == end()) return end();
		return iter;
	}
	iterator erase(const iterator& iter)
	{
		if (iter == end() || iter == m_Begin || iter.m_Node == nullptr) return end();
		// ��������� ���
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			PNODE Parent = iter.m_Node->m_Parent;

			// �θ� ���� ��� --> �ش� ��� �Ѱ��� ���Ҵٴ� �ǹ�
			if (!Parent)
			{
				m_Size = 0;
				m_Begin->m_Next = m_End;
				m_End->m_Prev = m_Begin;
				m_Root = nullptr;
				return end();
			}

			// �θ� ����� ��������, ���������� ���ؾ� 
			// �θ���� ���� ����
			// �ش� ��� �����ϱ� 
			if (Parent->m_Left == iter.m_Node)
				Parent->m_Left = nullptr;
			else
				Parent->m_Right = nullptr;

			PNODE Next = iter.m_Node->m_Next;
			PNODE Prev = iter.m_Node->m_Prev;
			Next->m_Prev = Prev;
			Prev->m_Next = Next;

			--m_Size;
			delete iter.m_Node;
			iterator result;
			result.m_Node = Next;
			return result;
		}
		// ���� ��尡 ���� 
		if (iter.m_Node->m_Left)
		{
			PNODE MaxNode = FindMax(iter.m_Node->m_Left);
			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

			PNODE LeftChild = MaxNode->m_Left;
			PNODE Parent = MaxNode->m_Parent;

			if (Parent->m_Left == MaxNode)
				Parent->m_Left = LeftChild;
			else
				Parent->m_Right = LeftChild;
			
			if (LeftChild) LeftChild->m_Parent = Parent;

			--m_Size;
			PNODE Next = MaxNode->m_Next;
			PNODE Prev = MaxNode->m_Prev;

			Next->m_Prev = Prev;
			Prev->m_Next = Next;

			delete MaxNode;

			iterator result;
			result.m_Node = Next;
			return result;

		}

		// ������ ��尡 ���� 
		if (iter.m_Node->m_Right)
		{
			PNODE MinNode = FindMin(iter.m_Node->m_Right);
			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

			PNODE Parent = MinNode->m_Parent;
			PNODE RightChild = MinNode->m_Right;

			if (Parent->m_Left == MinNode)
				Parent->m_Left = RightChild;
			else
				Parent->m_Right = RightChild;

			if (RightChild) RightChild->m_Parent = Parent;

			PNODE Next = iter.m_Node->m_Next;
			PNODE Prev = iter.m_Node->m_Prev;
			Next->m_Prev = Prev;
			Prev->m_Next = Next;

			delete MinNode;
			--m_Size;

			iterator result;
			result.m_Node = Next;
			return result;
		}

	}

private :
	iterator  Find(const KEY& key, PNODE Node)
	{
		if (!Node) return end();
		if (Node->first == key)
		{
			iterator result;
			result.m_Node = Node;
			return result;
		}
		if (Node->first > key)
			return Find(key, Node->m_Left);
		return Find(key, Node->m_Right);
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

			NewNode->m_Parent = Node;
			Node->m_Left = NewNode;

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

		NewNode->m_Parent = Node;
		Node->m_Right = NewNode;

		PNODE Next = Node->m_Next;
		Next->m_Prev = NewNode;
		NewNode->m_Next = Next;

		NewNode->m_Prev = Node;
		Node->m_Next = NewNode;
		return NewNode;
	}
	void PreOrder(void (*pFunc)(const KEY&, const VALUE&),PNODE Node)
	{
		// ���� : root -> left -> right 
		pFunc(Node->first, Node->second);
		PreOrder(pFunc, Node->m_Left);
		PreOrder(pFunc, Node->m_Right);
	}

	void InOrder(void (*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		// ���� : left -> root -> right
		InOrder(pFunc, Node->m_Left);
		pFunc(Node->first, Node->second);
		InOrder(pFunc, Node->m_Right);
	}
	void PostOrder(void (*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		// ���� : left -> right -> root 
		PostOrder(pFunc, Node->m_Left);
		PostOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second);
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
};