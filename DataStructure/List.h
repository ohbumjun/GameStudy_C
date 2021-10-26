#pragma once
#include<assert.h>

template<typename T>
class CListNode
{
	template<typename T>
	friend class CList;
	template<typename T>
	friend class CListIterator;
	template <typename T>
	friend class CListReverseIterator;

public:
	CListNode() :
		m_Next(nullptr),
		m_Prev(nullptr)
	{
	}
	~CListNode() {}
private:
	CListNode<T>* m_Next;
	CListNode<T>* m_Prev;
	T m_Data;
};

template<typename T>
class CListIterator
{
	template<typename T>
	friend class CList;
public:
	CListIterator() {}
	~CListIterator() {}
private:
	CListNode<T>* m_Node;
public:
	bool operator == (const CListIterator<T>& iter) const
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CListIterator<T>& iter) const
	{
		return m_Node != iter.m_Node;
	}
	bool operator == (const CListNode<T>* Node) const
	{
		return m_Node == Node;
	}
	bool operator != (const CListNode<T>* Node) const
	{
		return m_Node != Node;
	}
	void operator++()
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
	T& operator * ()
	{
		return m_Node->m_Data;
	}
};


template <typename T>
class CListReverseIterator
{
	template <typename T>
	friend class CList;

public:
	CListReverseIterator() :
		m_Node(nullptr)
	{
	}

	~CListReverseIterator()
	{
	}

private:
	CListNode<T>* m_Node;

public:
	// iterator끼리 서로 가지고 있는 노드가 같을 경우 같다고 판단한다.
	bool operator == (const CListReverseIterator<T>& iter)	const
	{
		return m_Node == iter.m_Node;
	}

	bool operator != (const CListReverseIterator<T>& iter)	const
	{
		return m_Node != iter.m_Node;
	}

	bool operator == (const CListNode<T>* Node)	const
	{
		return m_Node == Node;
	}

	bool operator != (const CListNode<T>* Node)	const
	{
		return m_Node != Node;
	}

	void operator ++ ()
	{
		m_Node = m_Node->m_Prev;
	}

	void operator ++ (int)
	{
		m_Node = m_Node->m_Prev;
	}

	void operator -- ()
	{
		m_Node = m_Node->m_Next;
	}

	void operator -- (int)
	{
		m_Node = m_Node->m_Next;
	}

	T& operator * ()	const
	{
		return m_Node->m_Data;
	}
};


template<typename T>
class CList
{
private:
	typedef CListNode<T> NODE;
	typedef CListNode<T>* PNODE;
public:
	typedef CListIterator<T> iterator;
	typedef CListReverseIterator<T>	reverse_iterator;

public:
	CList()
	{
		m_Size = 0;
		m_Begin = new NODE;
		m_End = new NODE;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
	}
	~CList()
	{
		PNODE DeleteNode = m_Begin->m_Next;
		while (DeleteNode)
		{
			PNODE Next = DeleteNode->m_Next;
			delete DeleteNode;
			DeleteNode = Next;
		}
	}
private:
	PNODE m_Begin;
	PNODE m_End;
	int m_Size;
public:
	void push_back(const T& data)
	{
		PNODE Node = new NODE;
		Node->m_Data = data;

		PNODE Prev = m_End->m_Prev;
		Prev->m_Next = Node;
		Node->m_Prev = Prev;

		Node->m_Next = m_End;
		m_End->m_Prev = Node;
		++m_Size;
	}
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
	T& front() const
	{
		if (empty()) assert(false);
		return m_Begin->m_Next->m_Data;
	}
	T& back() const
	{
		if (empty()) assert(false);
		return m_End->m_Prev->m_Data;
	}
	bool empty() const
	{
		return m_Size == 0;
	}
	int size() const
	{
		return m_Size;
	}
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
		m_End->m_Prev = m_Begin;
		m_Begin->m_Next = m_End;
	}
	void operator = (const CList<T> list)
	{
		clear();
		PNODE Node = list.m_Begin->m_Next;
		while (Node)
		{
			push_back(Node->m_Data);
			Node = Node->m_Next;
		}
	}
	void sort(bool (*pFunc)(const T&, const T&))
	{
		iterator iter1 = begin();
		iterator iter1End = end();
		--iter1End;

		iterator iter2 = begin();
		iterator iter2End = end();

		for (; iter1 != iter1End; ++iter1)
		{
			iter2 = iter1;
			++iter2;
			for (; iter2 != iter2End; ++iter2)
			{
				if (pFunc(*iter1, *iter2))
				{
					T Temp = *iter1;
					*iter1 = *iter2;
					*iter2 = Temp;
				}
			}
		}
	}
	iterator begin()
	{
		iterator iter;
		iter.m_Node = m_Begin->m_Next;
		return iter;
	}
	iterator end()
	{
		iterator iter;
		iter.m_Node = m_End->m_Prev;
		return iter;
	}
	reverse_iterator rbegin()	const
	{
		reverse_iterator	iter;
		iter.m_Node = m_End->m_Prev;
		return iter;
	}

	reverse_iterator rend()	const
	{
		reverse_iterator	iter;
		iter.m_Node = m_Begin;
		return iter;
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
		return eraseIterator(iter);
	}
	iterator eraseIterator(iterator& iter)
	{
		if (iter == end() || iter == m_Begin) return end();
		PNODE Prev = iter.m_Node->m_Prev;
		PNODE Next = iter.m_Node->m_Next;
		Prev->m_Next = Next;
		Next->m_Prev = Prev;
		delete iter.m_Node;
		iterator result;
		result.m_Node = Next;
		return result;
	}
};