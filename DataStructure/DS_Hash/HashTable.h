#pragma once
#include<iostream>
#include"Hash.h"
#include"List.h"

template <typename Key, typename Value>
class CHashNode
{
	template <typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;

	template <typename Key, typename Value>
	friend class CHashTableIterator;

private:
	CHashNode()
	{
	}

	~CHashNode()
	{
		typename CList<CHashNode<Key, Value>*>::iterator	iter = m_Chain->begin();
		typename CList<CHashNode<Key, Value>*>::iterator	iterEnd = m_Chain->end();

		for (; iter != iterEnd; ++iter)
		{
			delete* iter;
		}
	}

public:
	Key		first;
	Value	second;

private:
	CList<CHashNode<Key, Value>*>*	m_Chain;

private:
	void Add(const Key& key, const Value& value)
	{
		CHashNode<Key, Value>* Node = new CHashNode<Key, Value>;

		Node->first = key;
		Node->second = value;
		m_Chain->push_back(Node);
	}

	CHashNode<Key, Value>* Get(const Key& key)
	{
		std::cout << "hello" << std::endl;
		typename CList<CHashNode<Key, Value>*>::iterator	iter = m_Chain->begin();
		typename CList<CHashNode<Key, Value>*>::iterator	iterEnd = m_Chain->end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->first == key)
				return *iter;
		}
		return nullptr;
	}
};

template<typename Key, typename Value>
class CHashIterator
{
	template<typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;
public :
	CHashIterator()
	{
		m_Node = nullptr;
	}
	~CHashIterator()
	{
	}
private :
	CHashNode<Key, Value>* m_Node;
public :
	bool operator == (const CHashIterator<Key,Value>&iter) const
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CHashIterator<Key, Value>& iter) const
	{
		return m_Node != iter.m_Node;
	}
	CHashNode<Key, Value>* operator *()
	{
		return m_Node;
	}
};


template<typename Key, typename Value, int HASHSIZE = 1223>
class CHashTable
{
private :
	typedef CHashNode<Key, Value> NODE;
	typedef CHashNode<Key, Value>* PNODE;
public :
	typedef CHashIterator<Key, Value> iterator;
public :
	CHashTable() { m_Size = 0; }
	~CHashTable(){}
private :
	NODE m_Hash[HASHSIZE];
	int m_Size;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void insert(const Key& key, const Value& value)
	{
		CHash hash;
		unsigned __int64 NewKey = hash.GetHash<Key>(key);
		int Index = (int)(NewKey % HASHSIZE);
		m_Hash[Index].Add(key, value);
		++m_Size;
	}
	Value& operator [] (const Key& key)
	{
		CHash hash;
		unsigned __int64 NewKey = hash.GetHash<Key>(key);
		int Index = (int)(NewKey % HASHSIZE);
		PNODE Node = m_Hash[Index].Get(key);
		if (!Node)
		{
			Value addValue;
			m_Hash[Index].Add(key, addValue);
			Node = m_Hash[Index].Get(key);
		}
		return Node->second;
	}
	iterator Find(const Key& key)
	{
		CHash hash;
		unsigned __int64 NewKey = hash.GetHash<Key>(key);
		int Index = (int)(NewKey % HASHSIZE);
		PNODE Node = m_Hash[Index].Get(key);
		iterator iter;
		iter.m_Node = Node;
		return iter;
	}
	bool IsValid(iterator& iter)
	{
		return iter.m_Node != nullptr;
	}
};