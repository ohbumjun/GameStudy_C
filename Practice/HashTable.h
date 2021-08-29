#pragma once
#include"List.h"
#include"Hash.h"

template<typename Key, typename Value>
class CHashNode
{
	template<typename Key, typename Value, int HASHSIZE = 1223>
	friend class CHashTable;
	template<typename Key, typename Value, int HASHSIZE = 1223>
	friend class CHashIterator;
private :
	CList<CHashNode<Key, Value>*> m_Chain;
public :
	Key first;
	Value second;
public :
	CHashNode(){}
	~CHashNode()
	{
		typename CList<CHashNode<Key, Value>*>::iterator iter    = m_Chain.begin();
		typename CList<CHashNode<Key, Value>*>::iterator iterEnd = m_Chain.end();
		for (; iter != iterEnd; ++iter)
		{
			delete* iter;
		}
	}
public :
	void Add(const Key& key, const Value& value)
	{
		CHashNode<Key, Value>* NewNode = new CHashNode<Key, Value>;
		NewNode->first = key;
		NewNode->second = value;
		m_Chain.push_back(NewNode);
	}
	CHashNode<Key, Value>* Get(const Key& key)
	{
		typename CList<CHashNode<Key, Value>*>::iterator iter = m_Chain.begin();
		typename CList<CHashNode<Key, Value>*>::iterator iterEnd = m_Chain.end();
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
	template<typename Key, typename Value, int HASHSIZE = 1223>
	friend class CHashTable;
private :
	CHashNode<Key, Value>* m_Node;
public :
	CHashIterator()
	{
		m_Node = nullptr;
	}
public :
	bool operator == (const CHashIterator<Key, Value>& iter)
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CHashIterator<Key, Value>& iter)
	{
		return m_Node != iter.m_Node;
	}
	CHashNode<Key, Value>* operator * ()
	{
		return iter.m_Node;
	}
};

template<typename Key, typename Value, int HASHSIZE = 1223>
class CHashTable
{
public :
	CHashTable()
	{
		m_Size = 0;
	}
	~CHashTable()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete m_HashTable[i];
		}
		delete [] m
	}
private :
	CHashNode<Key, Value>* m_HashTable[HASHSIZE];
	int m_Size;
};