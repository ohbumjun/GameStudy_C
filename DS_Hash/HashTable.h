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
		// 최종 결과물 얻어내기
		// 왜 typename ?
		// 식별자는 예기치않은 토큰이 필요합니다.

		// iterfator는 CList에 종속적인 형식
		// 이 경우라면, 문법적으로 typename을 붙여준다
		// 붙여준다
		// template을 사용할 때 !! 
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
	// HashSize를 "소수"로 잡아주는 이유 ?
	// 우리는 %, 나머지.를 key로 잡을 것인데
	// "소수"같은 경우, 1과 자기 자신으로만 나눠지므로
	// 다른 수에 비해, 중복될 확률이 적다 .
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

		// 해쉬 충돌
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

	// &를 붙이면 lvalue가 되어서, 대입형태가 가능하게 되는 것이다 
	Value& operator [] (const Key& key)
	{
		CHash hash;

		unsigned __int64 HashKey = hash.GetHash<Key>(key);
		int Index = (int)(HashKey % HASHSIZE);

		PNODE Node = m_Array[Index];

		// 못찾았다면 
		// key를 이용해서, 추가를 해버릴 것이다 
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
		// 유효성 체크 
		return iter.m_Node != nullptr;
	}

};

