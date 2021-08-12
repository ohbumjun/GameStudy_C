#pragma once

#include<string>
#include"List.h"
#include"Hash.h"

template<typename Key, typename Value>
class CHashNode
{
	template<typename Key, typename Value>
	friend class CHashIterator;
	template<typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;
public :
	~CHashNode()
	{
		typename CList<CHashNode<Key, Value>*>::iterator iter = m_Chain.begin();
		typename CList<CHashNode<Key, Value>*>::iterator iter = m_Chain.end();
		for (; iter != iterEnd; ++iter)
		{
			delete* iter;
		}
	}
public :
	Key first;
	Value second;
private :
	CList<CHashNode<Key, Value>*> m_Chain;
private :
	void Add(const Key& key, const Value& value)
	{
		CHashNode<Key, Vale>* Node = new CHashNode<Key, Value>;
		Node->first = key;
		Node->second = value;
		m_Chain.push_back(Node);
	}
	CHashNode<Key, Value>* Get(const Key& key)
	{
		typename CList<CHashNode<Key, Value>*>::iterator iter = m_Chain.begin();
		typename CList<CHashNode<Key, Value>*>::iterator iter = m_Chain.end();
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->first == key) return *iter;
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
public :
private :
	CHashNode<Key, Value>* m_Node;
public :
	bool operator == (const CHashIterator<Key, Value>& iter) const
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CHashIterator<Key, Value>& iter) const
	{
		return m_Node != iter.m_Node;
	}
	CHashNode<Key, Value>* operator * ()
	{
		return m_Node;
	}
};


template<typename Key, typename Value, int HASHSIZE=1223>
class CHashTable
{

private :
	typedef CHashNode<Key, Value> NODE;
	typedef CHashNode<Key, Value>* PNODE;
public :
	typedef CHashIterator<Key, Value> iterator;
private :
	int m_Size;
	NODE m_Array[HASHSIZE];
public :
	CHashTable()
	{

	}
	~CHashTable()
	{

	}
public :
	void insert(const Key& key, const Value& value)
	{
		CHash hash;
		unsigned __int64 HashKey = hash.GetHash<Key>(key);
		int Index = (HashKey % HASHSIZE);
		m_Array[Index].Add(key, Value);
		++m_Size;
	}
	iterator Find()
	{
		CHash hash;
		unsigned __int64 HashKey = hash.GetHash<Key>(key);
		int Index = (HashKey % HASHSIZE);
		PNODE Node = m_Array[Index].Get(key);
		iterator iter;
		iter.m_Node = Node;
		return iter;
	}
	Value& operator [] (const Key& key)
	{
		CHash hash;
		unsigned __int64 HashKey = hash.GetHash<Key>(key);
		int Index = (HashKey % HASHSIZE);
		PNODE Node = m_Array[Index].Get(key);
		if (!Node)
		{
			Value addValue;
			m_Array[Index].Add(key, addValue);
			Node = m_Array[Index].Get(key);
		}
		return Node->second;
	}
	bool IsValid(const iterator& iter)
	{
		return iter.m_Node != nullptr;
	}
};