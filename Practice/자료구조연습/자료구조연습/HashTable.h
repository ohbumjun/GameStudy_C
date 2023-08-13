#pragma once

#include "Hash.h"
#include "List.h"

template<typename Key, typename Value>
class CHashNode {
	template<typename Key, typename Value>
	friend class CHashIterator;
	template<typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;
private :
	CList<CHashNode<Key, Value>*> m_Chain;
private :
	Key first;
	Value second;
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
		if ((*iter)->first == key)
			return (*iter);
	}
	return nullptr;
}
};

template<typename Key, typename Value>
class CHashIterator {
	template<typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;
private :
	CHashIterator() : m_Node(nullptr){}
	~CHashIterator(){}
private :
	CHashNode<Key, Value>* m_Node;
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
		return m_Node;
}
};

template<typename Key, typename Value, int HASHSIZE = 1223>
class CHashTable {
	typedef CHashNode<Key, Value> NODE;
	typedef CHashNode<Key, Value>* PNODE;
	typedef CHashIterator<Key, Value> iterator;
private :
	NODE m_Array[HASHSIZE];
	int m_Size;
public :
	void insert(const Key& key, const Value& value)
{
		CHash hash;
		unsigned __int64 HashKey = hash.GetKey<Key>(key);
		int Index = (int)(HashKey % HASHSIZE);

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
	iterator Find(const Key& key)
{
		CHash hash;
		unsigned __int64 HashKey = hash.GetKey<Key>(key);
		int Index = (HashKey % HASHSIZE);

		CHashNode<Key, Value>* FoundNode = m_Array[Index].Get(key);

		iterator iter;
		iter.m_Node = FoundNode;
		return iter;
}
	Value& operator [] (const Key& key)
	{
		CHash hash;
		const unsigned __int64 HashKey = hash.GetKey<Key>(key);
		int Index = (int)(HashKey % HASHSIZE);
		PNODE Node = m_Array[Index].Get(key);

		if (!Node)
		{
			Value value;
			m_Array[Index].Add(key, value);
			Node = m_Array[Index].Get(key);
		}

		return Node->second;
	}
	bool IsValid(const iterator& iter) const
{
		return iter.m_Node != nullptr;
}
};

// Stack Allocator
// (시스템 프로그래밍) 5장.
// (시스템 프로그래밍) 메모리 관리