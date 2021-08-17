#pragma once
#include"Stack.h"
#include"Queue.h"


template<typename T>
class CEdge
{
	template<typename T>
	friend class CGraphNode;
	template<typename T>
	friend class CGraph;
public :
	CEdge()
	{
		m_Node = nullptr;
	}
	~CEdge()
	{

	}
private :
	class CGraphNode<T>* m_Node;
};

template<typename T>
class CGraphNode
{
	template<typename T>
	friend class CGraph;
private :
	int m_Size;
	int m_Capacity;
};

template<typename T>
class CGraph
{

};