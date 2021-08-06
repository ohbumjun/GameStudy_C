#pragma once
#include<assert.h>

template<typename KEY,typename VALUE>
class CAVLTreeNode
{
	template<typename KEY, typename VALUE>
	friend class CAVLTreeIterator;
	template<typename KEY, typename VALUE>
	friend class CAVLTree;
public :
	CAVLTreeNode()
	{
		m_Next = nullptr;
		m_Prev = nullptr;
		m_Left = nullptr;
		m_Right = nullptr;
		m_Parent = nullptr;
	}
	~CAVLTreeNode()
	{
	}
private :
	CAVLTreeNode<KEY, VALUE>* m_Next;
	CAVLTreeNode<KEY, VALUE>* m_Prev;
	CAVLTreeNode<KEY, VALUE>* m_Left;
	CAVLTreeNode<KEY, VALUE>* m_Right;
	CAVLTreeNode<KEY, VALUE>* m_Parent;
public :
	KEY first;
	VALUE second;
public :
	bool IsParent() const 
	{
		return m_Parent != nullptr;
	}
	int GetParentKey() const
	{
		return m_Parent->first;
	}
	int GetParentValue() const
	{
		return m_Parent->second;
	}
	bool IsLeft() const
	{
		return m_Left != nullptr;
	}
	int GetLeftKey() const
	{
		return m_Left->first;
	}
	int GetLeftValue() const
	{
		return m_Left->second;
	}
	bool IsRight() const
	{
		return m_Right != nullptr;
	}
	int GetRightKey() const
	{
		return m_Right->first;
	}
	int GetRightValue() const
	{
		return m_Right->second;
	}
};


template<typename KEY, typename VALUE>
class CAVLTreeIterator
{
	template<typename KEY, typename VALUE>
	friend class CAVLTree;
public :
	CAVLTreeIterator() : m_Node(nullptr)
	{
	};
	~CAVLTreeIterator()
	{
	}
private :
	CAVLTreeNode<KEY, VALUE>* m_Node;
public :
	bool operator == (const CAVLTreeNode<KEY, VALUE>* Node) const
	{
		return m_Node == Node;
	}
	bool operator != (const CAVLTreeNode<KEY, VALUE>* Node) const
	{
		return m_Node != Node;
	}
	bool operator == (const CAVLTreeIterator<KEY, VALUE>& iter) const
	{
		return m_Node == iter.m_Node;
	}
	bool operator != (const CAVLTreeIterator<KEY, VALUE>& iter) const
	{
		return m_Node != iter.m_Node;
	}
	void operator ++ ()
	{
		m_Node = m_Node->m_Next;
	}
	void operator ++ (int)
	{
		m_Node = m_Node->m_Next;
	}
	void operator -- ()
	{
		m_Node = m_Node->m_Prev;
	}
	void operator -- (int)
	{
		m_Node = m_Node->m_Prev;
	}
	CAVLTreeNode<KEY, VALUE>* operator -> ()
	{
		return m_Node;
	}
};

template<typename KEY, typename VALUE>
class CAVLTree
{
private :
	typedef CAVLTreeNode<KEY, VALUE> NODE;
	typedef CAVLTreeNode<KEY, VALUE>* PNODE;
public :
	typedef CAVLTreeIterator<KEY, VALUE> iterator;
public :
	CAVLTree()
	{
		m_Size = 0;
		m_Root = nullptr;
		m_Begin = new NODE;
		m_End = new NODE;
	}
	~CAVLTree()
	{
	}
private :
	CAVLTreeNode<KEY, VALUE>* m_Begin;
	CAVLTreeNode<KEY, VALUE>* m_End;
	CAVLTreeNode<KEY, VALUE>* m_Root;
	int m_Size;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	iterator begin() const {
		iterator result;
		result.m_Node = m_Begin->m_Next;
		return result;
	}
	iterator end() const {
		iterator result;
		result.m_Node = m_End;
		return result;
	}
	iterator Find(const KEY& key)
	{
		return Find(key, m_Root);
	}
	void insert(const KEY& key, const VALUE& value)
	{
		if (!m_Root)
		{
			m_Root = new NODE;
			m_Root->first = key;
			m_Root->second = value;
			
			m_Begin->m_Next = m_Root;
			m_Root->m_Prev = m_Begin;
			
			m_Root->m_Next = m_End;
			m_End->m_Prev = m_Root;

		}
		else insert(key, value, m_Root);
		++m_Size;
	}
	// ������ȸ : root -> left -> right 
	void PreOrder(void(*pFunc)(const KEY& , const VALUE&))
	{
		PreOrder(pFunc, m_Root);
	}
	// ������ȸ : left -> root -> right
	void InOrder(void(*pFunc)(const KEY&, const VALUE&))
	{
		InOrder(pFunc, m_Root);
	}
	// ������ȸ : left -> right -> root
	void PostOrder(void(*pFunc)(const KEY&, const VALUE&))
	{
		PostOrder(pFunc, m_Root);
	}
	iterator erase(const KEY& key)
	{
		iterator iter = Find(key);
		if (iter == end()) return end();
		return erase(iter);
	}
	iterator erase(const iterator& iter)
	{
		if (iter == m_Begin || iter == end() || iter.m_Node == nullptr) 
			return end();
		// ��������� ��
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			PNODE Parent = iter.m_Node->m_Parent;
			// �θ� ���ٸ� 
			if (!Parent)
			{
				delete iter.m_Node;
				m_Root = nullptr;
				m_Begin->m_Next = m_End;
				m_End->m_Prev = m_Begin;
				--m_Size;
				return end();
			}

			if (Parent->m_Left == iter.m_Node)
				Parent->m_Left = nullptr;
			else
				Parent->m_Right = nullptr;
		
			PNODE Prev = iter.m_Node->m_Prev;
			PNODE Next = iter.m_Node->m_Next;
			Prev->m_Next = Next;
			Next->m_Parent = Prev;
			delete iter.m_Node;
			--m_Size;
			ReBalance(Parent);
			iterator result;
			result.m_Node = Next;
			return result;
		}
		// ���� ��尡 ����
		if (iter.m_Node->m_Left)
		{
			PNODE MaxNode = FindMax(iter.m_Node->m_Left);
			PNODE Parent = MaxNode->m_Parent;
			PNODE LeftChild = MaxNode->m_Left;

			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;
			
			if (Parent->m_Left == MaxNode)
				Parent->m_Left = LeftChild;
			else
				Parent->m_Right = LeftChild;
			if (LeftChild)
				LeftChild->m_Parent = Parent;

			// ����Ʈ ���� 
			PNODE Prev = MaxNode->m_Prev;
			PNODE Next = MaxNode->m_Next;
			Prev->m_Next = Next;
			Next->m_Prev = Prev;

			delete MaxNode;
			--m_Size;

			ReBalance(Parent);
			iterator result;
			result.m_Node = Next;
			return result;
		}
		
		// ������ ��尡 ���� 
		PNODE MinNode = FindMin(iter.m_Node->m_Right);
		PNODE Parent = MinNode->m_Parent;
		PNODE RightChild = MinNode->m_Right;

		iter.m_Node->first = MinNode->first;
		iter.m_Node->second = MinNode->second;

		if (Parent->m_Left == MinNode)
			Parent->m_Left = RightChild;
		else
			Parent->m_Right = RightChild;

		// ����Ʈ ���� 
		PNODE Prev = MinNode->m_Prev;
		PNODE Next = MinNode->m_Next;
		Prev->m_Next = Next;
		Next->m_Prev = Prev;

		delete MinNode;
		--m_Size;

		ReBalance(Parent);
		iterator result;
		result.m_Node = Next;
		return result;
	}
private :
	PNODE FindMax(PNODE Node)
	{
		if (Node->m_Right) return FindMax(Node->m_Right);
		return Node;
	}
	PNODE FindMin(PNODE Node)
	{
		if (Node->m_Left) return FindMin(Node->m_Left);
		return Node;
	}
	void PreOrder(void(*pFunc)(const KEY&, const VALUE&),PNODE Node)
	{
		if (!Node) return;
		pFunc(Node->first, Node->second);
		PreOrder(pFunc, Node->m_Left);
		PreOrder(pFunc, Node->m_Right);
	}
	// ������ȸ : left -> root -> right
	void InOrder(void(*pFunc)(const KEY&, const VALUE&),PNODE Node)
	{
		if (!Node) return;
		InOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second);
		InOrder(pFunc, Node->m_Left);
	}
	// ������ȸ : left -> right -> root
	void PostOrder(void(*pFunc)(const KEY&, const VALUE&),PNODE Node)
	{
		if (!Node) return;
		PostOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second);
		PostOrder(pFunc, Node->m_Left);
	}
	PNODE insert(const KEY& key, const VALUE& value, PNODE Node)
	{
		// ���س�庸�� �۴ٸ� �����̴�.
		if (Node->first > key)
		{
			// ���� ���س���� ���� �ڽĳ�尡 �ִٸ� �� ���� �ڽĳ�带
			// ���س��� �Ͽ� �ٽ� Ž���� �ϰ� �Ѵ�.
			if (Node->m_Left)
				return insert(key, value, Node->m_Left);

			// ���̻� ���� �ڽĳ�尡 ���� ��� �� ��ġ�� ���� ��带 �����Ͽ�
			// �߰����־�� �Ѵ�.
			PNODE	NewNode = new NODE;

			NewNode->first = key;
			NewNode->second = value;

			// ���س���� ���� �ڽĳ��� �����Ѵ�.
			Node->m_Left = NewNode;
			NewNode->m_Parent = Node;

			// �������� ��ġ�� �ȴٴ°��� �θ��庸�� �۴ٴ� ���̴�.
			// �׷��Ƿ� �θ����� �������� �θ��� ���̿� ���� ������
			// ��带 ����Ʈ�� �������ֵ��� �Ѵ�.
			PNODE	Prev = Node->m_Prev;

			Prev->m_Next = NewNode;
			NewNode->m_Prev = Prev;

			NewNode->m_Next = Node;
			Node->m_Prev = NewNode;

			ReBalance(NewNode);

			return NewNode;
		}

		// ����� �������� ���� ũ�ٴ� ���̹Ƿ� ���������� Ž���� �غ��� �Ѵ�.
		// ���� ������ �ڽĳ�尡 ���� ��� ���س�带 ������ �ڽĳ��� �Ͽ�
		// Ž���ϰ� �Ѵ�.
		if (Node->m_Right)
			return insert(key, value, Node->m_Right);

		// ���̻� ������ �ڽĳ�尡 ���� ��� �� ��ġ�� ���� ��带 �����Ͽ�
		// �߰����־�� �Ѵ�.
		PNODE	NewNode = new NODE;

		NewNode->first = key;
		NewNode->second = value;

		// ���س���� ������ �ڽĳ��� �����Ѵ�.
		Node->m_Right = NewNode;
		NewNode->m_Parent = Node;

		PNODE	Next = Node->m_Next;

		Node->m_Next = NewNode;
		NewNode->m_Prev = Node;

		NewNode->m_Next = Next;
		Next->m_Prev = NewNode;

		ReBalance(NewNode);

		return NewNode;
	}
	iterator Find(const KEY& key, PNODE& Node)
	{
		if (!Node) return end();
		if (key == Node->first)
		{
			iterator iter;
			iter.m_Node = Node;
			return iter;
		}
		if (Node->first > key)
			return Find(key, Node->m_Left);
		return Find(key, Node->m_Right);
	}
	int GetHeight(PNODE Node) const
	{
		if (!Node) return 0;
		int Left = GetHeight(Node->m_Left);
		int Right = GetHeight(Node->m_Right);
		int Height = Left < Right ? Right : Left;
		return Height + 1;
	}
	int BalanceFactor(PNODE Node)
	{
		return GetHeight(Node->m_Left) - GetHeight(Node->m_Right);
	}
	PNODE RotationLeft(PNODE Node)
	{
		PNODE Parent = Node->m_Parent;
		PNODE RightChild = Node->m_Right;
		PNODE RightLeftChild = RightChild->m_Left;

		// ���س��, ������ �ڽ� ���
		Node->m_Parent = RightChild;
		RightChild->m_Left = Node;

		// ������ �ڽ� ���, �θ���
		if (Parent)
		{
			if (Parent->m_Left == Node)
				Parent->m_Left = RightChild;
			else
				Parent->m_Right = RightChild;
		}
		// �θ��尡 ���ٴ� ���� , ���س�尡 root ����� �ǹ�
		else
			m_Root = RightChild;
		if (RightChild)
			RightChild->m_Parent = Parent;

		// ���� ���, ������ ���� ���
		Node->m_Right = RightLeftChild;
		if(RightLeftChild)
			RightLeftChild->m_Parent = Node;

		return RightChild;

	}
	PNODE RotationRight(PNODE Node)
	{
		PNODE Parent = Node->m_Parent;
		PNODE LeftChild = Node->m_Left;
		PNODE LeftRightChild = LeftChild->m_Right;

		// ����, ���� �ڽ�
		Node->m_Parent = LeftChild;
		LeftChild->m_Right = Node;

		// �θ�, ���� �ڽ�
		if (Parent)
		{
			if (Parent->m_Left == Node)
				Parent->m_Left = LeftChild;
			else
				Parent->m_Right = LeftChild;
		}
		else
			m_Root = LeftChild;
		if (LeftChild)
			LeftChild->m_Parent = Node;

		// ����, ���� ������ 
		Node->m_Left = LeftRightChild;
		if(LeftRightChild)
			LeftRightChild->m_Parent = Node;
		
		return LeftChild;
	
	}
	void ReBalance(PNODE Node)
	{
		if (!Node) return;
		int Factor = BalanceFactor(Node);
		// ������
		if (Factor <= -2)
		{
			int RightFactor = BalanceFactor(Node->m_Right);
			if (RightFactor <= 0)
			{
				// ��,�� 
				// ���س�� --> ���� ȸ��
				Node = RotationLeft(Node);
			}
			else
			{
				// ��,��
				// ������ �ڽ� ��� ���� ��ȸ��
				RotationRight(Node->m_Right);
				// ���س�� ��ȸ��
				Node = RotationLeft(Node);
			}
		}
		// ����
		else if (Factor >= 2)
		{
			int LeftFactor = BalanceFactor(Node->m_Left);
			if (LeftFactor >= 0)
			{
				// ��,��
				Node = RotationRight(Node);
			}
			else
			{
				// ��,��
				RotationLeft(Node->m_Left);
				Node = RotationRight(Node);
			}
		}
		ReBalance(Node->m_Parent);
	}
};




