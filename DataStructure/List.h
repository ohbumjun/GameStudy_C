#include<assert.h>

template<typename T>
class CEdge
{
	template<typename T>
	friend class CGraph;

	template<typename  T>
	friend class CGraphNode;

private :
	CEdge()
	{
		m_Node = nullptr;
	}
	~CEdge(){}
private :
	class CGraphNode<T>* m_Node;
};

template<typename T>
class CGraphNode
{
	template<typename T>
	friend class CGraph;
private :
	CGraphNode()
	{
		m_Size = 0;
		m_Capacity = 1;
		m_EdgeArray = new CEdge<T>*[m_Capacity];
		m_Visit = false;
	}
	~CGraphNode()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete m_EdgeArray[i];
		}
		delete[] m_EdgeArray;
	}
private :
	CEdge<T>** m_EdgeArray;
	int m_Size;
	int m_Capacity;
	T m_Data;
	bool m_Visit;
private :
	void AddEdge(CGraphNode<T>* Node)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CEdge<T>** Array = new CEdge<T>*[m_Capacity];
			memcpy(Array, m_EdgeArray, sizeof(CEdge<T>*) * m_Size);

			delete[] m_EdgeArray;
			m_EdgeArray = Array;
		}
		CEdge<T>* Edge = new CEdge<T>;
		Edge->m_Node = Node;

		m_EdgeArray[m_Size] = Edge;
		++m_Size;
	}
};

template<typename T>
class CGraph
{
public :
	CGraph()
	{
		m_Size = 0;
		m_Capacity = 4;
		m_NodeArray = new CGraphNode<T>*[m_Capacity];
	}
	~CGraph()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete m_NodeArray[i];
		}
		delete[] m_NodeArray;
	}
private :
	int m_Size;
	int m_Capacity;
	CGraphNode<T>** m_NodeArray;
public :
	void insert(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CGraphNode<T>** Array = new CGraphNode<T>*[m_Capacity];
			memcpy(Array, m_NodeArray, sizeof(CGraphNode<T>*) * m_Size);
			delete[] m_NodeArray;
			m_NodeArray = Array;
		}
		CGraphNode<T>* Node = new CGraphNode<T>;
		Node->m_Data = data;
		m_NodeArray[m_Size] = Node;
		++m_Size;
	}

	// 노드에 edge 를 추가해준다.
	void AddEdge(const T& Src, const T& Dest)
	{
		CGraphNode<T>* SrcNode = nullptr;
		CGraphNode<T>* DestNode = nullptr;
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == Src)
				SrcNode = m_NodeArray[i];
			else if (m_NodeArray[i]->m_Data == Dest)
				DestNode = m_NodeArray[i];
			if (SrcNode && DestNode)
				break;
		}
		if (!SrcNode || !DestNode) return;
		SrcNode->AddEdge(DestNode);
		DestNode->AddEdge(SrcNode);
	}
};