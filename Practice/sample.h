#pragma
#include<assert.h>

template<typename T>
class CListNode
{
private :
	template<typename T>
	friend class CListIterator;
	template<typename T>
	friend class CList;
public :
	CListNode()
	{
		m_Next = nullptr;
		m_Prev = nullptr;
	}
private :
	CListNode<T>* m_Next;
	CListNode<T>* m_Prev;
	T m_Data;
};

template<typename T>
class CListIterator
{
	template<typename T>
	friend class CList;
public :
	CListIterator()
	{

	}
private :
	CListNode<T>* m_Node;
public :
	bool operator == (const CListIterator<T>& iter)
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CListIterator<T>&iter)
	{
		return m_Node != iter.m_Node;
	}
	bool operator == (const CListNode<T>* Node)
	{
		return m_Node == Node;
	}
	bool operator !=(const CListNode<T>* Node)
	{
		return m_Node != Node;
	}
	T& operator * ()
	{
		return m_Node->m_Data;
	}
};

template<typename T>
class CList
{
private :
	typedef CListNode<T>* PNODE;
	typedef CListNode<T> NODE;
public :
	typedef CListIterator<T> iterator;
public :
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
	CListNode<T>* m_Begin;
	CListNode<T>* m_End;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
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
		m_Size = 0;
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
	}
	T& front() const
	{
		if (empty()) assert(false);
		return m_Begin->m_Next->m_Data
	}
	T& back() const
	{
		if (empty()) assert(false);
		return m_End->m_Prev->m_Data;
	}
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

		Node->m_Prev    = m_Begin;
		m_Begin->m_Next = Node;

		++m_Size;
	}
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
	void pop_front()
	{
		if (empty()) assert(false);
		PNODE DeleteNode = m_Begin->m_Next;
		PNODE Next = DeleteNode->m_Next;
		delete DeleteNode;
		Next->m_Prev = m_Begin;
		m_Begin->m_Next = Next;
		--m_Size;
	}
	void operator = (const CList<T>& list)
	{
		clear();
		PNODE Node = list.m_Begin->m_Next;
		while (Node != list.m_End)
		{
			push_back(Node->m_Data);
			Node = Node->m_Next;
		}
		m_Size = list.m_Size;
	}
	iterator Find(const T& data)
	{
		iterator iter = begin();
		for (; iter != end(); ++iter)
		{
			if (*iter == data) return iter;
		}
		return nullptr;
	}
	iterator erase(const T& data)
	{
		iterator iter = Find(data);
		if (iter == end()) return end();
		return erase(iter);
	}
	iterator erase(iterator & iter)
	{
		if (iter == m_Begin() || iter == end()) return end();
		PNODE Next = iter.m_Node->m_Next;
		PNODE Prev = iter.m_Node->m_Prev;
		Next->m_Prev = Prev;
		Prev->m_Next = Next;
		delete iter.m_Node;
		--m_Size;
		iterator result;
		result.m_Node = Next;
		return result;
	}
};