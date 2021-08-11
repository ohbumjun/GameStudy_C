#pragma once
#include"Hash.h"
#include"List.h"

template<typename Key, typename Value>
class CHashNode
{
	template<typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;
	template<typename Key, typename Value, int HASHSIZE>
	friend class CHashIterator;

private :
	CHashNode()
	{

	}
	~CHashNode()
	{
		typename CList<CHashNode><Key, Value>* ::iterator iter;
		typename CList<CHashNode><Key, Value>* ::iterator iter;

		for (; iter != iterEnd; ++iter)
		{
			delete iter;
		}
	}
public :
	Key fir/st;
	Value second;
private :
	CList<CHashNode<Key, Value>*> m_Chain;
private :
	void Add(const Key& key, const VAlue& value)
	{
		CHashNode<Key, Value>* Node = new CHashNode<Key, Value>;
		Node->first = key;
		Node->second = value;
		m_Chain.push_back(Node);
	}
	CHashNode<Key, Value>* Get(Value& result, const Key& key)
	{
		// ���� ����� ����
		// �� typename ?
		// �ĺ��ڴ� ����ġ���� ��ū�� �ʿ��մϴ�.

		// iterfator�� CList�� �������� ����
		// �� �����, ���������� typename�� �ٿ��ش�
		// �ٿ��ش�
		// template�� ����� �� !! 
		typename CList<CHashNode><Key, Value>* ::iterator iter = m_Chain.begin() ;
		typename CList<CHashNode><Key, Value>* ::iterator iterEnd = m_Chain.end();
		
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->first == key)
				return iter;
		}
			
			
			
		return nullptr;
	}
};

template<typename Key, typename Value, int HASHSIZE>
class CHashIterator
{
	template<typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;
public :
	CHashIterator()
	{

	}
	~CHashIterator()
	{

	}
private :
	CHashNode<Key, Value>* m_Node;
public :
	bool operator == (const CHashTableIteror < Key, Value& iter) const
	{
		return m_Node == Node;
	}
};

template<typename Key, typename Value, int HASHSIZE>
class CHashTable
{
	// HashSize�� "�Ҽ�"�� ����ִ� ���� ?
	// �츮�� %, ������.�� key�� ���� ���ε�
	// "�Ҽ�"���� ���, 1�� �ڱ� �ڽ����θ� �������Ƿ�
	// �ٸ� ���� ����, �ߺ��� Ȯ���� ���� .
public :
	CHashTable()
	{

	}
	~CHashTable()
	{

	}
private :
	typedef CHashNode<Key, Value> NODE;
	typedef CHashNode<Key, Value>* PNODE;

private :
	NODE m_Array[HASHSIZE];
	int m_Size;
public :
	void insert(const Key& key, const Value& value)
	{
		CHash hash;
		unsigned __int64 HashKey = hash.GetHash<Key>(key);
		int Index = (int)(HashKey % HASHSIZE);

		// �ؽ� �浹
		m_Array[Index].Add(key, value);
		++m_Size;

	}
	int size() const
	{
		return m_Size;
	}
	bool empty() const
	{
		return m_Size == 0;
	}

	// &�� ���̸� lvalue�� �Ǿ, �������°� �����ϰ� �Ǵ� ���̴� 
	Value& operator [] (const Key& key)
	{
		CHash hash;

		unsigned __int64 HashKey = hash.GetHash<Key>(key);
		int Index = (int)(HashKey % HASHSIZE);

		PNODE Node = m_Array[Index];

		// ��ã�Ҵٸ� 
		// key�� �̿��ؼ�, �߰��� �ع��� ���̴� 
		if (!Node)
		{
			Value addValue;
			m_Array[Index].Add(key, addValue);
			Node = m_Array[Index].Get(key);
		}
		return Node->second;
	}

	iterator Find(Value& result, const Key& key)
	{
		CHash hash;

		unsigned __int64 HashKey = hash.GetHash<Key>(key);
		int Index = (int)(HashKey % HASHSIZE);

		PNODE Node = m_Array[Index];

		
		iterator iter;
		iter.mNode = Node;
		return iter;
	}
	bool IsValid(const iterator& iter) const
	{
		// ��ȿ�� üũ 
		return iter.m_Node != nullptr;
	}

};

