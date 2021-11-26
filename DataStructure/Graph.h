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
		for (int i = 0; i < m_Size; i++)
		{
			delete m_EdgeArray[i];
		}
		delete[] m_EdgeArray;
	}
private :
	int m_Capacity;
	int m_Size;
	CEdge<T>** m_EdgeArray;
	bool m_Visit;
	T m_Data;
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
	void Add(CQueue<CGraphNode<T>*>& queue)
	{
		for (int i = 0; i < m_Size; i++)
		{
			// Edge 로 연결된 노드가 이미 방문한 노드라면 추가를 안한다
			if (m_EdgeArray[i]->m_Node->m_Visit) continue;

			queue.push(m_EdgeArray[i]->m_Node);
			m_EdgeArray[i]->m_Node->m_Visit = true; 

		}
	}
	void Add(CStack<CGraphNode<T>*>& stack)
	{
		for (int i = 0; i < m_Size; i++)
		{
			if (m_EdgeArray[i]->m_Node->m_Visit) continue;
			stack.push(m_EdgeArray[i]->m_Node);
			m_EdgeArray[i]->m_Node->m_Visit = true;
		}
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
	void BFS(void (*pFunc)(const T&))
	{
		if (m_Size == 0) return;
		for (int i = 0; i < m_Size; i++)
		{
			m_NodeArray[i]->m_Visit = false;
		}
		CQueue<CGraphNode<T>*> queue;
		queue.push(m_NodeArray[0]);
		m_NodeArray[0]->m_Visit = true;
		while (!queue)
		{
			CGraphNode<T>* Node = queue.front();
			queue.pop();
			Node->Add(queue);
			pFunc(Node->m_Data);
		}
	}
	void DFS(void(*pFunc)(const T&))
	{
		if (m_Size == 0) return;
		for (int i = 0; i < m_Size; i++)
		{
			m_NodeArray[i]->m_Visit = false;
		}
		CStack<CGraphNode<T>*> stack;
		stack.push(m_NodeArray[0]);
		m_NodeArray[0]->m_Visit = true;

		while (!stack.empty())
		{
			CGraphNode<T>* Node = stack.top();
			stack.pop();
			Node->Add(stack);
			pFunc(Node->m_Data);
		}
	}
};