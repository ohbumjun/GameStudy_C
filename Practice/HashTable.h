#pragma once
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
		typename CList<CHashNode<Key, Value>*>::iterator iter    = m_Chain.begin();
		typename CList<CHashNode<Key, Value>*>::iterator iterEnd = m_Chain.end();
		for (; iter != iterEnd; ++iter)
			delete* iter;
	}
public :
	Key first;
	Value second;
private :
	CList<CHashNode<Key, Value>*> m_Chain;
public :
	void Add(const Key& key, const Value& value)
	{
		CHashNode<Key, Value>* NewNode = new CHashNode<Key,Value>;
		NewNode->first = key;
		NewNode->second = value;
		m_Chain.push_back(NewNode);
	}
	CHashNode<Key, Value>* Get(const Key& key)
	{
		typename CList<CHashNode<Key, Value>*>::iterator iter = m_Chain.begin();
		typename CList<CHashNode<Key, Value>*>::iterator iterEnd = m_Chain.end();
		for (; iter != iterEnd; ++iter)
			return *iter;
		return nullptr;
	}
};

template<typename Key, typename Value>
class CHashIterator
{
	template<typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;
private :
	CHashNode<Key, Value>* m_Node;
public :
	bool operator == (const CHashIterator<Key,Value>& iter) const
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

template<typename Key, typename Value, int HASHSIZE = 1223>
class CHashTable
{
public :
	typedef CHashIterator<Key, Value> iterator;
private :
	typedef CHashNode<Key, Value>* PNODE;
	typedef CHashNode<Key, Value> NODE;
private :
	NODE m_Array[HASHSIZE];
	int m_Size{ 0 };
public :
	void insert(const Key& key, const Value& value)
	{
		CHash hash;
		unsigned __int64 NewKey = hash.GetHash<Key>(key);
		int Index = (int)(NewKey % HASHSIZE);
		m_Array[Index].Add(key, value);
	}
	Value& operator [] (const Key& key)
	{
		CHash hash;
		unsigned __int64 NewKey = hash.GetHash<Key>(key);
		int Index = (int)(NewKey % HASHSIZE);
		PNODE Node = m_Array[Index].Get(key);
		if (!Node)
		{
			Value addValue;
			m_Array[Index].Add(key, addValue);
			Node = m_Array[Index].Get(key);
		}
		return Node->second;
	}
	iterator  Find (const Key& key)
	{
		CHash hash;
		unsigned __int64 NewKey = hash.GetHash<Key>(key);
		int Index = (int)(NewKey % HASHSIZE);
		PNODE Node = m_Array[Index].Get(key);
		iterator iter;
		iter.m_Node = Node;
		return iter;
	}
	bool IsValid(const iterator& iter)
	{
		return iter.m_Node != nullptr;
	}
};