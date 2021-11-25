#include<assert.h>

template<typename T>
class CEdge
{
	template<typename T>
	friend class CGraphNode;
	template<typename T>
	friend class CGraph;
private :
	CEdge():m_Node(nullptr){}
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
	CGraph()
	{
		m_Capacity = 4;
		m_Size = 0;
		m_EdgeArray = new CEdge<T>*[m_Capacity];
		m_Visit = false;
	}
	~CGraph()
	{
		delete[] m_EdgeArray;
	}
private :
	int m_Capacity;
	int m_Size;
	CEdge<T>** m_EdgeArray;
	bool m_Visit;
public :
	void AddEdge(CGraphNode<T>* Node)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CEdge<T>** EdgeArray = new CEdge<T>*[m_Capacity];
			memcpy(EdgeArray, m_EdgeArray, sizeof(CEdge<T>*) * m_Size);

			delete[] m_EdgeArray;
			m_EdgeArray = EdgeArray;
		}
		CEdge<T>* NewEdge = new CEdge;
		NewEdge->m_Node = Node;
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
		delete [] m_NodeArray;
	}
public :
	int m_Size;
	int m_Capacity;
	CGraphNode<T>** m_NodeArray;
public :
	void insert(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CGraphNode<T>** GraphNodes = new CGraphNode<T>*[m_Capacity];
			memcpy(GraphNodes, m_NodeArray, sizeof(CGraphNode<T>*) * m_Size);
			delete[] GraphNodes;
			m_NodeArray = GraphNodes;
		}
		CGraphNode<T>* Node = new CGraphNode;
		Node->m_Data = data;
		m_NodeArray[m_Size] = Node;
		+m_Size;
	}
	void AddEdge(const T& Src, const T& Dest)
	{
		CGraphNode<T>* SrcNode   = nullptr;
		CGraphNode<T>* DestNode = nullptr;
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == Src)
				SrcNode = Src;
			if (m_NodeArray[i]->m_Data == Dest)
				DestNode = Src;
			if (SrcNode && DestNode) break;
		}
		if (!SrcNode && !DestNode) return;
		SrcNode->AddEdge(DestNode);
		DestNode->AddEdge(SrcNode);
	}
};