#pragma once

#include <assert.h>

// friend : �Ʒ�ó�� Listnode�� List�� friend�� �����ϸ� List������ ListNode��
// private�� ������ �� �ִ�.

template <typename T>
class CListNode
{
	template <typename T>
	friend class CList;

	template <typename T>
	friend class CListIterator;

	template <typename T>
	friend class CListReverseIterator;

private:
	CListNode() :
		m_Next(nullptr),
		m_Prev(nullptr)
	{
	}

	~CListNode()
	{
	}

private:
	T		m_Data;
	CListNode<T>* m_Next;
	CListNode<T>* m_Prev;
};

/*
iterator : ��带 ���������� �湮�ϱ� ���ؼ�  iterator�� ���� �����Ѵ�.
iterator�� �̿��ؼ� �տ��� ���� Ȥ�� �ڿ������� ��带 ���������� ������ �� �ְ� ���ش�.
*/
template <typename T>
class CListIterator
{
	template <typename T>
	friend class CList;

public:
	CListIterator() :
		m_Node(nullptr)
	{
	}

	~CListIterator()
	{
	}

private:
	CListNode<T>* m_Node;

public:
	// iterator���� ���� ������ �ִ� ��尡 ���� ��� ���ٰ� �Ǵ��Ѵ�.
	bool operator == (const CListIterator<T>& iter)	const
	{
		return m_Node == iter.m_Node;
	}

	bool operator != (const CListIterator<T>& iter)	const
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

	T& operator * ()	const
	{
		return m_Node->m_Data;
	}
};

// ���� : reverseiterator Ŭ������ ��������.
// reverseiterator �� ���������� �����ϴ� iterator�̴�.

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
	// iterator���� ���� ������ �ִ� ��尡 ���� ��� ���ٰ� �Ǵ��Ѵ�.
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


template <typename T>
class CList
{
public:
	CList()
	{
		m_Size = 0;

		// Begin�� End��带 �����ϰ� �� ��带 ���� �����Ѵ�.
		m_Begin = new NODE;
		m_End = new NODE;

		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
	}

	~CList()
	{
		// Begin���� ������尡 nullptr�̴�.
		// End���� ������尡 nullptr�̴�.
		PNODE	DeleteNode = m_Begin;

		while (DeleteNode)
		{
			PNODE	Next = DeleteNode->m_Next;

			delete	DeleteNode;

			DeleteNode = Next;
		}
	}

private:
	typedef CListNode<T>	NODE;
	typedef CListNode<T>* PNODE;

public:
	typedef CListIterator<T>	iterator;
	typedef CListReverseIterator<T>	reverse_iterator;

private:
	// Begin�� End�� �����͸� �����ϱ� ���� ���� �ƴϴ�.
	// ���������� ���۰� ���� �ǹ��ϴ� ���� ����ϱ� ���� �Ҵ��صΰ� ����Ѵ�.
	// ���� �����͸� �����ϴ� ���� Begin�� End��� ���̿� ��ġ�ϰ� �ɰ��̴�.
	PNODE	m_Begin;
	PNODE	m_End;
	int		m_Size;

public:
	void push_back(const T& data)
	{
		// �����͸� �����ص� ��带 �����Ѵ�.
		PNODE	Node = new NODE;

		Node->m_Data = data;

		// End���� End����� ������� ���̿� ���� ������ ��带 �߰����ֵ��� �Ѵ�.
		PNODE	Prev = m_End->m_Prev;

		// End����� ��������� ������带  End���� ���� ������ ���� ��ü�Ѵ�.
		Prev->m_Next = Node;

		// ���� ������ ����� ������带 End�� �������� �����Ѵ�.
		Node->m_Prev = Prev;

		// ���� ������ ����� ���� ��带 End��帣 �����Ѵ�.
		Node->m_Next = m_End;

		// End����� ������带 ���� ������ ���� �����Ѵ�.
		m_End->m_Prev = Node;

		++m_Size;
	}

	void push_front(const T& data)
	{
		PNODE	Node = new NODE;

		Node->m_Data = data;

		// Begin���� Begin����� ������� ���̿� ���� ������ ��带 �߰��Ѵ�.
		PNODE	Next = m_Begin->m_Next;

		// ���� ������ ����� �������� Begin����� ������带 �����Ѵ�.
		Node->m_Next = Next;

		// Begin����� ��������� ������带 ���� ������ ���� �����Ѵ�.
		Next->m_Prev = Node;

		// Begin����� �������� ���� ������ ��带 �����Ѵ�.
		m_Begin->m_Next = Node;

		// ���� ������ ����� �������� Begin��带 �����Ѵ�.
		Node->m_Prev = m_Begin;

		++m_Size;
	}

	// ������ ��带 �����Ѵ�.
	void pop_back()
	{
		if (empty())
		{
			assert(false);
		}

		// End����� ���� ��带 �����ش�. �׷��Ƿ� End�� ������带 ������
		// End�� ���� ����� ������带 ���ͼ� End�� �������ش�.
		PNODE	DeleteNode = m_End->m_Prev;

		PNODE	Prev = DeleteNode->m_Prev;

		// �������� ��������� ������带 End���� �����Ѵ�.
		Prev->m_Next = m_End;

		// End����� �������� �������� ������带 �����Ѵ�.
		m_End->m_Prev = Prev;

		// ��带 �����Ѵ�.
		delete	DeleteNode;

		--m_Size;
	}

	void pop_front()
	{
		if (empty())
		{
			assert(false);
		}

		// Begin����� ���� ��带 �����ش�.
		PNODE	DeleteNode = m_Begin->m_Next;

		// ���� ����� ���� ��带 ���´�.
		PNODE	Next = DeleteNode->m_Next;

		// Begin����� �������� ���� ����� ������带 �����Ѵ�.
		m_Begin->m_Next = Next;

		// �������� ��������� �������� Begin��带 �����Ѵ�.
		Next->m_Prev = m_Begin;

		// ��带 �����Ѵ�.
		delete	DeleteNode;

		--m_Size;
	}

	const T& front()	const
	{
		if (empty())
			assert(false);

		return m_Begin->m_Next->m_Data;
	}

	const T& back()	const
	{
		if (empty())
			assert(false);

		return m_End->m_Prev->m_Data;
	}


	// Ŭ������ ����Լ��� �Լ��� �ڿ� const�� ���� �� �ִ�.
	// ����Լ��� �ڿ� const�� ������ �� �Լ������� ��������� ���� ������ �� ���� �ȴ�.
	// const ��ü�� ������� ����� �ڿ� const�� ���� ����Լ��� ȣ���� �����ϴ�.
	int size()	const
	{
		return m_Size;
	}

	bool empty()	const
	{
		return m_Size == 0;
	}

	// �߰��� ��� ��带 �����Ѵ�.
	void clear()
	{
		if (empty())
			return;

		PNODE	DeleteNode = m_Begin->m_Next;

		// End��尡 �ƴ϶�� while�� ���鼭 �������ֵ��� �Ѵ�.
		while (DeleteNode != m_End)
		{
			// ���� ����� ������带 �̸� �޾Ƶ־� �Ѵ�.
			// �ֳ��ϸ� ��带 ���� ���������ٸ� ������带 ���� �� ���� �����̴�.
			PNODE	Next = DeleteNode->m_Next;

			// ��带 �����Ѵ�.
			delete	DeleteNode;

			// ���� ��带 ���� ���� �����Ѵ�.
			DeleteNode = Next;
		}

		// Begin���� End��带 ���� �������ش�.
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;

		// ����� ������ 0���� �ʱ�ȭ���ش�.
		m_Size = 0;
	}

	iterator begin()	const
	{
		iterator	iter;
		iter.m_Node = m_Begin->m_Next;
		return iter;
	}

	iterator end()	const
	{
		iterator	iter;
		iter.m_Node = m_End;
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

	void operator = (const CList<T>& list)
	{
		// ���� ������ �����Ѵ�.
		clear();

		m_Size = list.m_Size;

		PNODE	Node = list.m_Begin->m_Next;

		while (Node != list.m_End)
		{
			push_back(Node->m_Data);
			Node = Node->m_Next;
		}
	}

	// ������ ����� ���� ��带 ������ �ִ� iterator�� ��ȯ���ش�.
	iterator erase(const T& data)
	{
		iterator	iter = Find(data);

		if (iter == end())
			return iter;

		return erase(iter);
	}

	iterator erase(const iterator& iter)
	{
		if (iter == end() || iter == m_Begin)
			return end();

		// ���� ����� �������� ������带 ���´�.
		// ���� ����� �������� ������带 ���� ��������ش�.
		PNODE	Prev = iter.m_Node->m_Prev;
		PNODE	Next = iter.m_Node->m_Next;

		Prev->m_Next = Next;
		Next->m_Prev = Prev;

		delete	iter.m_Node;

		--m_Size;

		iterator	result;
		result.m_Node = Next;

		return result;
	}

	iterator Find(const T& data)
	{
		iterator	iter = begin();
		iterator	iterEnd = end();

		for (; iter != iterEnd; ++iter)
		{
			if (*iter == data)
				return iter;//
		}

		// ��ã�� ��� end�� �����Ѵ�.
		return iterEnd;
	}

	// ����
	void sort(bool (*pFunc)(const T&, const T&))
	{
		iterator	iter1 = begin();
		iterator	iter1End = end();
		--iter1End;

		iterator	iter2;
		iterator	iter2End = end();

		// ���� ��� 10����� 0 ~ 8 ������ �ݺ��Ѵ�.
		// ������ 9���� �ݺ��� ���Ѵ�.
		for (; iter1 != iter1End; ++iter1)
		{
			iter2 = iter1;
			++iter2;
			for (; iter2 != iter2End; ++iter2)
			{
				if (pFunc(*iter1, *iter2))
				{
					T	data = *iter1;
					*iter1 = *iter2;
					*iter2 = data;
				}
			}
		}
	}
};

