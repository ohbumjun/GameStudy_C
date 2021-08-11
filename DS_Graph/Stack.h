#pragma once

#include <assert.h>

template <typename T>
class CStackNode
{
	template <typename T>
	friend class CStack;

private:
	CStackNode() :
		m_Next(nullptr)
	{
	}

	~CStackNode()
	{
	}

private:
	CStackNode<T>* m_Next;
	T				m_Data;
};

template <typename T>
class CStack
{
public:
	CStack()
	{
		m_LastNode = nullptr;
		m_Size = 0;
	}

	~CStack()
	{
		clear();
	}

private:
	// ���� �������� �߰��� ����� �ּҸ� ��´�.
	CStackNode<T>* m_LastNode;
	int				m_Size;

public:
	void push(const T& data)
	{
		// �����͸� �����ϱ� ���� ��带 �����Ѵ�.
		CStackNode<T>* Node = new CStackNode<T>;

		Node->m_Data = data;

		// ���� ������ ����� ������带 ������ �������� �߰��� ���� �������ش�.
		// ���� ó�� �߰��ϴ� ����� m_LastNode�� nullptr�� �� �����Ƿ�
		// ���λ����� ����� �������δ� nullptr�� ������ ���̴�.
		// ��, ���� ��尡 nullptr�̶�� ���̻� ��尡 ���ٴ� �ǹ��̴�.
		Node->m_Next = m_LastNode;

		// ���������� �߰��� ��带 �������ֵ��� �Ѵ�.
		m_LastNode = Node;

		++m_Size;
	}

	T& top()	const
	{
		if (empty())
			assert(false);

		return m_LastNode->m_Data;
	}

	void pop()
	{
		if (empty())
			assert(false);

		CStackNode<T>* Next = m_LastNode->m_Next;

		delete	m_LastNode;

		m_LastNode = Next;

		--m_Size;
	}

	int size()	const
	{
		return m_Size;
	}

	bool empty()	const
	{
		return m_Size == 0;
	}

	void clear()
	{
		while (m_LastNode)
		{
			CStackNode<T>* Next = m_LastNode->m_Next;
			delete	m_LastNode;
			m_LastNode = Next;
		}

		m_Size = 0;
	}
};

