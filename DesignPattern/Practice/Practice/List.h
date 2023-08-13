#pragma once

#include<assert.h>

template<typename T>
class CListNode
{
	template<typename T>
	friend class CListIterator;
	template<typename T>
	friend class CList;
public :
	CListNode() : 
		m_Next(nullptr), m_Prev(nullptr){}
	~CListNode()
	{
	}
private :
	T m_Data;
	CListNode<T>* m_Next;
	CListNode<T>* m_Prev;
};

template<typename T>
class CListIterator
{
	template<typename T>
	friend class CList;
public :
	CListIterator() :  m_Node(nullptr)
	{
	}
	~CListIterator()
	{
	}
private :
	CListNode<T>* m_Node;
public :
	bool operator == (const CListNode<T>* Node) const 
	{
		return m_Node == Node;
	}
	bool operator != (const CListNode<T>* Node) const 
	{
		return m_Node != Node;
	}
	bool operator == (const CListIterator<T>& iter) const 
	{
		return m_Node == iter.Node;
	}
	bool operator != (const CListIterator<T>& iter) const 
	{
		return m_Node != iter.Node;
	}
	void operator ++()
	{
		m_Node = m_Node->m_Next;
	}
	void operator --()
	{
		m_Node = m_Node->m_Prev;
	}
	T& operator*() const 
	{ 
		return iter.m_Node->m_Data; 
	}
};


template<typename T>
class CList
{
private :
	typedef CListNode<T> NODE;
	typedef CListNode<T>* PNODE;
public :
	typedef CListIterator<T> iterator;
public :
	CList()
	{
		m_Begin = new NODE;
		m_End = new NODE;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
		m_Size = 0;
	}
	~CList()
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
public :
	// push_back
	void push_back(const T& data)
	{
		PNODE Node;
		Node->m_Data = data;

		PNODE Prev = m_End->m_Prev;
		Prev->m_Next = Node;
		Node->m_Prev = Prev;

		Node->m_Next = m_End;
		m_End->m_Prev = Node;

		++m_Size;
	}
	// push_front
	void push_front(const T& data)
	{
		PNODE Node = new NODE;
		Node->m_Data = data;

		PNODE Next = m_Begin->m_Next;
		Next->m_Prev = Node;
		Node->m_Next = Next;

		Node->m_Prev = m_Begin;
		m_Begin->m_Next = Node;

		++m_Size;
	}
	bool empty() const { return m_Size == 0; }
	// pop_back
	void pop_back()
	{
		if (empty()) assert(false);
		PNODE DeleteNode = m_End->m_Prev;
		PNODE Prev = DeleteNode->m_Prev;
		delete DeleteNode;
		Prev->m_Next = m_End;
		m_End->m_Prev = Prev;
		--m_Size;
	}

	// pop_front
	void pop_front()
	{
		if (empty()) assert(false);
		PNODE DeleteNode = m_Begin->m_Next;
		PNODE Next = DeleteNode->m_Next;
		delete DeleteNode;
		m_Begin->m_Next = Next;
		Next->m_Prev = m_Begin;
		--m_Size;
	}
	// front, back
	const T& front() const 
	{
		if (empty()) assert(false);
		return m_Begin->m_Next->m_Data;
	}
	const T& back() const 
	{
		if (empty()) assert(false);
		return m_End->m_Prev->m_Data;
	}
	// size. empty, clear 
	int size() const { return m_Size; }
	void clear()
	{
		if (empty()) assert(false);
		PNODE DeleteNode = m_Begin->m_Next;
		while (DeleteNode != m_End)
		{
			PNODE Next = DeleteNode->m_Next;
			delete DeleteNode;
			DeleteNode = Next;
		}
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
		m_Size = 0;
	}
	// begin, end, = 
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
	void operator = (const CList<T>& list)
	{
		clear();
		m_Size = list.m_Size;

		PNODE Node = list.m_Begin->m_Next;
		while (Node != list.m_End)
		{
			push_back(Node->m_Data);
			Node = Node->m_Next;
		}
	}
	iterator Find(const T& data)
	{
		iterator iter = begin();
		iterator iterEnd = end();
		for (; iter != iterEnd; ++iter)
		{
			if (*iter == data) return iter;
		}
		return iterEnd;
	}
	iterator erase(const T& data)
	{
		iterator iter = Find(data);
		if (iter == end()) return end();
		return erase(iter);
	}
	iterator erase(const iterator& iter)
	{
		if (iter == end() || iter == m_Begin) return false;
		PNODE Prev = iter.m_Node->m_Prev;
		PNODE Next = iter.m_Node->m_Next;
		delete iter.m_Node;
		Prev->m_Next = Next;
		Next->m_Prev = Prev;
		--m_Size;
		iterator result;
		result.m_Node = Next;
		return result;
 	}
	// erase , Find, sort 
	void sort(bool (*pFunc)(const T&, const T&))
	{
		iterator iter1 = begin();
		iterator iter1End = end();
		--iter1End;

		iterator iter2;
		iterator iter2End = end();
		for (; iter1 != iter1End; ++iter1)
		{
			iter2 = iter1;
			++iter2;;
			for (; iter2 != iter2End; ++iter2)
			{
				if (pFunc(*iter1, *iter2))
				{
					T data = *iter1;
					*iter1 = *iter2;
					*iter2 = data;
				}
			}
		}
	}
};
