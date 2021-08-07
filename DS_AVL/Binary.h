#pragma once
#include<assert.h>

template<typename KEY, typename VALUE>
class CAVLTreeNode
{
	template<typename KEY, typename VALUE>
	friend class CAVLTreeIterator;
	template<typename KEY, typename VALUE>
	friend class CAVLTreeNode;
public :
	CAVLTreeNode()
	{
		m_Next = nullptr;
		m_Prev = nullptr;
		m_Left = nullptr;
		m_Right = nullptr;
		m_Parent = nullptr;
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
	T m_Data;
public :
	KEY first;
	VALUE second;
public :
	bool IsParent()
	{
		return m_Parent != nullptr;
	}
	int GetParentKey()
	{
		return m_Parent->first;
	}
	int GetParentValue()
	{
		return m_Parent->second;
	}
	bool IsLeft()
	{
		return m_Left != nullptr;
	}
	int GetLeftKey()
	{
		return m_Left->first;
	}
	int GetLeftValue()
	{
		return m_Left->second;
	}
	bool IsRight()
	{
		return m_Right != nullptr;
	}
	int GetRightKey()
	{
		return m_Right->first;
	}
	int GetRightValue()
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
	bool operator == (const CAVLTreeNode<KEY, VALUE>* Node)
	{
		return m_Node = Node;
	}
	bool operator != (const CAVLTreeNode<KEY, VALUE>* Node)
	{
		return m_Node != Node;
	}
	bool operator == (const CAVLTreeIterator<KEY, VALUE>& iter)
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CAVLTreeIterator<KEY, VALUE>& iter)
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
		m_Root = nullptr;
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
		while(DeleteNode != m_End)
		{
			PNODE Next = DeleteNode->m_Next;
			delete DeleteNode;
			DeleteNode = Next;
		}
		m_Root = nullptr;
		m_Size = 0;
	}
	// 전위 : root -> left -> right
	void PreOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		PreOrder(pFunc, m_Root);
	}
	// 중위 : left -> root -> right
	void InOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		InOrder(pFunc, m_Root);
	}
	// 후위 : left -> right -> root
	void PostOrder(void (*pFunc)(const KEY&, const VALUE&))
	{
		PostOrder(pFunc, m_Root);
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
	void erase(const KEY& key)
	{
		iterator iter = Find(key);
		if (iter == end()) return end();
		return erase(iter);
	}
	void erase(const iterator& iter)
	{

	}
	void insert(const KEY& key, const VALUE& value)
	{
		if (!m_Root)
		{
			m_Root = new NODE;
			m_Root->first = key;
			m_Root->second = value;
		}
	}
private :
	iterator Find(const KEY& key, PNODE Node)
	{
		if (!Node) return end();
		if (Node->first == key)
		{
			iterator result;
			result.m_Node = Node;
			return result;
		}
		if (Node->first > key) return Find(key, Node->m_Left);
		return Find(key, Node->m_Right);
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
};