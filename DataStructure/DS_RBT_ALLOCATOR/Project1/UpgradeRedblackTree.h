#pragma once

#include "Flag.h"
#include "GeneralAllocator.h"

const enum NODE_COLOR : bool { BLACK = false, RED = true };

template<typename T, typename V>
struct RBNode
{
	T key;
	V val;

	RBNode* left;
	RBNode* right;
	RBNode* next;

	// 같은 Key 의 Node 들은 연결리스트 형태로 둔다. (next pointer 사용)
	// 연결리시트 상의 첫번째 노드만이 left, right, parent 를 지닌다.
	RBNode* parent;

	// Insert 하는 노드는 무조건 RED 로 시작한다.
	RBNode color = NODE_COLOR::RED;
};

template <typename T, typename V, typename _Allocator = AlignedAllocator<RBNode<T, V>>>
class RBTree
{
	static_assert(!std::is_pointer<TAllocator>::value, "TAllocator must not be pointer type, use ref type instead");
	
	// std::decay<TAllocator>::type> : remove reference, const, volatile from TAllocator type
	// ex) if TAllocator is const SomeAllocator&, then std::decay<TAllocator>::type would be SomeAllocator
	static_assert(std::is_base_of<Allocable, typename std::decay<TAllocator>::type>::value, "type parameter of this class must derive from BaseClass");

	enum BalanceDirection
	{
		LEFT = 0,
		RIGHT = 1,
	};

	enum BalanceCase
	{
		RL = 0,  
		RR = 1,  
		LL = 2,  
		LR = 3,  
	};
public :
	template<typename A, typename B, class _Alloc>
	class ConstIterator
	{
		typedef RBTree<A, B, _Alloc> Tree;

	public :
		explicit ConstIterator(const RBNode* node, const Tree* tree)
			: m_Node(node)
			, m_Tree(tree)
		{
		}

		bool MoveNext()
		{
			if (m_Node == nullptr) return false;

			m_Node = m_Tree->NextNode(m_Node);

			if (m_Node == nullptr) return false;

			return true;
		}

		const RBNode& Current()
		{
			return *m_Node;
		}

		const RBNode& operator*()
		{
			return Current();
		}

		ConstIterator& operator++()
		{
			return preIncrement();
		}

		ConstIterator operator++(int)
		{
			return postIncrement();
		}

		bool operator==(const ConstIterator& it) const
		{
			return it.m_Node == m_Node;
		}

		bool operator!=(const ConstIterator& it) const
		{
			return it.m_Node != m_Node;
		}

	private :

		ConstIterator& preIncrement()
		{
			MoveNext();
			return *this;
		}

		ConstIterator postIncrement()
		{
			ConstIterator p = *this;
			MoveNext();
			return p;
		}

		const RBNode* m_Node;

		const Tree* m_Tree;
	};


	using ConstIterator = ConstIterator<T, V, _Allocator>;

public :
	explicit RBTree()
		: m_Count(0)
		, m_Root(nullptr)
	{ }

	explicit RBTree(_Allocator& allocator)
		: m_Count(0)
		, m_Root(nullptr)
		, m_Allocator(allocator)
	{ }

	RBTree(const RBTree& o)
		: m_Count(0)
		, m_Root(nullptr)
	{
		findByPostOrder(o.m_Root, [=](Node* n)->bool
		{
			this->Insert(n->key, n->val);
			return false;
		});
	}

	// 이동 생성자
	RBTree(RBTree&& o)
		: m_Count(o.m_Count)
		, m_Root(o.m_Root)
		, m_Allocator(o.m_Allocator)
	{
		o.m_Count = 0;
		o.m_Root = nullptr;
	}

	~RBTree()
	{
		Clear();
	}

	void Insert(T key, V val)
	{
		void* mem = m_Allocator.Allocate(sizeof(Node), __FILE__, __LINE__);
		RBNode* node = new (mem) RBNode();

		node->key = key;
		node->val = val;

		Insert(node);
	}

	void Insert(RBNode* node)
	{
		// Root Node 를 Insert
		if (_root == nullptr)
		{
			_root = node;

			// Root Node 색생은 Black
			_root->color = NODE_COLOR::BLACK;
			_count++;

			return;
		}

		// Find parent
		RBNode* p = _root;

		while (p != nullptr)
		{
			if (p->key == node->key)
			{
				while (p->next != nullptr)
				{
					// 중복 Key 까지는 허용하지만, 중복 val 는 허용하지 않는다.
					if (p->val == node->val)
						return;
					if (p->next == p)
						p->next = nullptr;
					p = p->next;
				}
				// 같은 key 값을 지닌 Node 들은 linked list 형태로 연결한다.
				p->next = node;
				m_Count++;
				return;
			}
			// 왼쪽 Insert
			else if (node->key < p->key)
			{
				if (p->left == nullptr)
				{
					node->parent = p;
					p->left = node;
					break;
				}

				p = p->left;
			}
			// (node->key > p->key)
			// 오른쪽 Insert
			else
			{
				if (p->right == nullptr)
				{
					node->parent = p;
					p->right = node;
					break;
				}

				p = p->right;
			}
		}

		insertFixup(node);
		m_Count++;
	}

	//해당 key 를 가진 모든 노드 지워주기 
	void Remove(T key)
	{
		RBNode* node = search(key);

		if (node == nullptr)
		{
			assert(info, "No node found");
			return;
		}

		RBNode* next = node->next;

		// 연결리스트에 있는 사항들을 전부 Free 시켜준다.
		while (next != nullptr)
		{
			RBNode* temp = next->next;
			m_Allocator.Free(next);
			m_Count--;
			next = temp;
		}

		node->next = nullptr;

		// Tree 상에서 해당 노드를 지워줘야 한다.
		Remove(node);
	}

	//Remove "Single" with "key, value"
	void Remove(T key, V value)
	{
		RBNode* node = search(key);

		if (node == nullptr) THROW("no node found");

		// 같은 key 값을 가지는 node 연결리스트의, 첫번째 node 를 가져온다.
		RBNode* prev = node;

		// 해당 연결리스트를 돌면서, 같은 value 를 지니는 첫번째 node 를 찾는다.
		while (node != nullptr && node->value != value)
		{
			prev = node;
			node = node->next;
		}

		// 해당 key 의 연결리스트 상에서, 지우고자 하는 value 를 가진 노드
		if (prev == node) 
		{
			Remove(node);
		}
		else //node in bucket
		{
			if (node == nullptr)
			{
				THROW("no node found");
			}
			if (!isNotFirstNodeInList(node))
			{
				THROW("node must not be not next node");
			}
			prev->next = node->next;
			m_Allocator.Free(node);
			m_Count--;
		}
	}

	// remove case reference :
	// https://www.notion.so/6-1-ce6647c0df8b4a248d533ed65ba15425
	// 1) Red Node 를 삭제하는 경우는, 그대로 삭제하면 된다.
	// 2) Black Node 를 삭제하는 경우 + 각 노드에서 자손 nil 노드들까지 가는 모든 경로는 black 수가 같다.
	//    라는 원칙이 위배 => extra black 을 사용한다.
	//    삭제되는 노드를 대체한 색에 extra black node 를 부여한 이후
	//    - red-and-black
	//    - doubly black 이냐에 따라서 별도의 처리 로직을 적용한다.
	void Remove(RBNode* node)
	{
		if (node == nullptr)
			return;

		// 같은 key 의 노드가 중복으로 존재한다면, 연결리스트 상에서 2번째 Node 를 삭제할 것이다.
		if (node->next != nullptr)
		{
			node->value = node->next->value;
			auto temp = node->next->next;
			m_Allocator.Free(node->next);
			node->next = temp;
			m_Count--;

			return;
		}
		// node->next 는 nullptr 인데, isNotFirstNodeInList(node) 가 true 라는 것은
		// 연결리스트 상에서 마지막 노드라는 것이다.
		else if (isNotFirstNodeInList(node))
		{
			RBNode* temp = search(node->key);

			// 연결리스트 상에서 현재 지우고자 하는 노드를 찾는다.
			while (temp->next != node)
			{
				temp = temp->next;
			}

			// 이전 노드 next 값을 nullptr 세팅해준다.
			temp->next = nullptr;

			m_Allocator.Free(node);

			m_Count--;

			return;
		}
		// 중복 key 없이, 현재 지우는 노드가 해당 key 를 가지는 유일한 노드일 경우
		else 
		{
			RBNode* curNode = getSuccessorNode(node);
			RBNode* parentNode = node->parent;

			// successor Node 가 없는 경우
			// 현재 지우는 key 의 노드 색상이 삭제되는 경우이다.
			if (curNode == nullptr)
			{
				if (node == m_Root)
				{
					if (m_Count != 1) THROW("NODE NOT DELETED");

					m_Root = nullptr;
				}
				else
				{
					// Black 을 지울 경우, 조정을 해줘야 한다.
					if (node->color == NODE_COLOR::BLACK)
						removeFixDoubleBlack(n);
					else
					{
						RBNode* brotherNode = getBrother(node);

						if (brotherNode != nullptr)
							brotherNode->color = NODE_COLOR::RED;
					}

					if (parentNode->left == node)
						parentNode->left = nullptr;
					else
						parentNode->right = nullptr;
				}

				_allocator.Free(n);
				_count--;
				return;
			}

			if (hasOneChild(n))
			{
				if (n == _root)
				{
					_root = c;
					c->parent = nullptr;

					if (c->color == Color::RED && (!isLeaf(n)))
					{
						c->color = Color::BLACK;
						Node* newc = c->left == nullptr ? c->right : c->left;
						if (newc != nullptr)
							removeFixDoubleBlack(newc);
					}

					_allocator.Free(n);
					_count--;
				}
				else
				{
					c->parent = p;
					if (isLeftChild(n))
						p->left = c;
					else
						p->right = c;

					if (c->color == Color::BLACK && n->color == Color::BLACK)
						removeFixDoubleBlack(c);
					else
						c->color = Color::BLACK;

					_allocator.Free(n);
					_count--;
				}
				return;
			}

			n->key = c->key;
			n->value = c->value;
			n->next = c->next;
			c->next = nullptr;

			Remove(c);
		}
	}

private :


	// left -> self -> right
	template<typename F>
	RBNode* findByInOrder(RBNode* node, F predicate)
	{
		if (node == nullptr)
			return node;

		RBNode* r = nullptr;

		if (node->left != nullptr)
		{
			r = findByInOrder(node->left, predicate);
			if (r != nullptr)
				return r;
		}

		if (predicate(node))
			return node;

		if (node->right != nullptr)
		{
			r = findByInOrder(node->right, predicate);
			if (r != nullptr)
				return r;
		}

		return r;
	}

	// left -> right -> self
	template<typename F>
	RBNode* findByPostOrder(RBNode* node, F predicate)
	{
		if (node == nullptr)
			return node;

		RBNode* r = nullptr;

		if (node->right != nullptr)
		{
			r = findByPostOrder(node->right, predicate);
			if (r != nullptr)
				return r;
		}
		
		if (node->left != nullptr)
		{
			r = findByPostOrder(node->left, predicate);
			if (r != nullptr)
				return r;
		}

		if (predicate(node))
			return node;

		return r;
	}

	bool isLeafNode(RBNode* n) const
	{
		return n->left == nullptr && n->right == nullptr;
	}

	// 연결리스트 상에서, 첫번째 노드가 아닌 노드인가
	// 첫번째 노드만이 parent, left, right 노드를 가질 수 있다.
	bool isNotFirstNodeInList(RBNode* n)
	{
		return n != _root && n->parent == nullptr && isLeafNode(n);
	}

	RBNode* search(T key) const
	{
		RBNode* target = _root;
		while (target != nullptr && key != target->key)
		{
			if (key == target->key)
				return target;

			if (key < target->key)
				target = target->left;
			else
				target = target->right;
		}
		return target;
	}

	RBNode* getBrother(RBNode* node)
	{
		RBNode* p = node->parent;

		if (node == nullptr)
			return nullptr;

		if (node == p->left)
			return p->right;
		else
			return p->left;
	}

	RBNode* getSuccessorNode(RBNode* node)
	{
		// 2개 자식을 가지고 있다면
		if (node->left != nullptr && node->right != nullptr)
		{
			RBNode* leftNode = node->left;

			while (leftNode->right != nullptr)
				leftNode = leftNode->right;

			// 현재 node 기준, 왼쪽 한번, 오른쪽 쭉 타고 내려가서 succssor 를 가져온다.
			return leftNode;
		}

		if (isLeafNode(node))
			return nullptr;

		// Single child
		if (n->left != nullptr)
			return n->left;
		else
			return n->right;
	}


	RBNode* getUncle(RBNode* node)
	{
		RBNode* p = node->parent;

		return getBrother(p);
	}

	RBNode* getGrandNode(RBNode* cur)
	{
		// 사실 이 조건문은 없어도 된다. 모든 노드는 생성시 nullptr대신 nil로 연결되기 때문.
		if (cur == nullptr || cur->parent == nullptr) return nullptr;
		return cur->parent->parent;
	}

	inline void changeColor(RBNode* a, RBNode* b)
	{
		NODE_COLOR temp = a->color;
		a->color = b->color;
		b->color = temp;
	}

	void insertFixup(RBNode* node)
	{
		RBNode* parent = nullptr;
		RBNode* grdParent = nullptr;
		RBNode* curNode = node;		    // child

		if (node->parent == nullptr) throw("node is nullptr. not possible case");

		// insert 된 node 는 leaf node 이다.
		// RB Tree 조건 중, RED 의 자식은 Black 이라는 조건이 있다.
		// 둘 중 하나라도 Black 이라면, 해당 조건은 만족한 것이므로 Fix 할 필요가 없다.
		if (curNode->color != NODE_COLOR::RED || curNode->parent->color != NODE_COLOR::RED)
			return;

		// Root 노드까지 타고 올라가면서 반복한다.
		while (curNode != m_Root && curNode->color == NODE_COLOR::RED 
			&& curNode->parent->color == NODE_COLOR::RED)
		{
			parent = curNode->parent;
			grdParent = getGrandNode(curNode);

			BalanceDirection nodeDir = (curNode->key < parent->key) ? BalanceDirection::LEFT : BalanceDirection::RIGHT;
			BalanceDirection parentDir = (parent->key < grdParent->key) ? BalanceDirection::LEFT : BalanceDirection::RIGHT;

			// Balance Case 를 따져본다.
			BalanceCase rotate;

			if (nodeDir == BalanceDirection::LEFT && parentDir == BalanceDirection::LEFT)
			{
				rotate = BalanceDirection::LL;
			}
			else if (nodeDir == BalanceDirection::LEFT && parentDir == BalanceDirection::RIGHT)
			{
				rotate = BalanceDirection::LR;
			}
			else if (nodeDir == BalanceDirection::RIGHT && parentDir == BalanceDirection::LEFT)
			{
				rotate = BalanceDirection::RL;
			}
			else if (nodeDir == BalanceDirection::RIGHT && parentDir == BalanceDirection::RIGHT)
			{
				rotate = BalanceDirection::RR;
			}

			RBNode* uncleNode = getUncle(curNode);

			// 자신 Red, 부모 Red, Uncle Red
			if (uncleNode != nullptr && uncleNode->color == NODE_COLOR::RED)
			{
				// 조부모 ~ (부모, 삼촌) 색상 모두 변경해준다.
				uncleNode->color = NODE_COLOR::BLACK;
				parent->color = NODE_COLOR::BLACK;
				grdParent->color = NODE_COLOR::RED;

				// 그리고 다시 (조부모) 노드로 부터 시작한다.
				curNode = grdParent;
			}
			// 부모는 Red, Uncle 은 Black 인 경우
			else
			{
				// Insert Case 4
				switch (rotate)
				{
				case BalanceCase::LL:
					changeColor(parent, grdParent);
					rightRotate(grdParent);
					break;
				case BalanceCase::RR:
					changeColor(parent, grdParent);
					leftRotate(grdParent);
					break;
				case BalanceCase::LR:
					leftRotate(parent);
					curNode = parent;
					parent = curNode->parent;
					rightRotate(grdParent);
					changeColor(parent, grdParent);
					break;
				case BalanceCase::RL:
					rightRotate(parent);
					curNode = parent;
					parent = curNode->parent;
					leftRotate(grdParent);
					changeColor(parent, grdParent);
					break;
				}
				n = parent;
			}
		}


		m_Root->color = NODE_COLOR::BLACK;
	}

	void leftRotate(RBNode* node)
	{
		/*
			  p				    p
			   \			     \
			   |n|              (tp)
			  /   \             /   \
			n.l	  (tp)  --->   |n|   tp.r
				  / \         / \
				tp.l tp.r   n.l tp.l

		*/

		// 과정 1) 좌회전시 오른쪽 자식노드를 가리키는 임시 포인터 생성.
		RBNode* temp = node->right;

		// 과정 2) cur과 temp를 분리시키면서, temp의 왼쪽 자식노드를 cur 의 오른쪽 노드로 세팅
		node->right = temp->left;

		// 과정 3) 방금 인수인계한게 nil이 아니라면, temp 의 왼쪽 자식노드의 부모를 cur로 지정.
		if (temp->left != nullptr) temp->left->parent = node;

		// 과정 4) temp의 부모를 cur에서 cur->parent로 바꿔준다.
		temp->parent = node->parent;

		// 과정 5) cur의 현재 상태에 따라서 temp를 이어주는 경우의 수가 나뉨.
		// cur->parent 의 자식으로 temp 를 세팅해줄 것이다. cur->right 을 자식으로 세팅
		if (node->parent == nullptr)
		{
			m_Root = temp;
		}
		else if (node == node->parent->left) node->parent->left = temp;
		else node->parent->right = temp;

		// 과정 6) 이제 모든 사전준비가 끝났으니 temp의 오른쪽 자식노드를 cur로 지정함.
		temp->right = node;

		// 과정 7) 이제 cur의 부모가 temp가 되면 좌회전 완료.
		node->parent = temp;
	}
	void rightRotate(RBNode* node)
	{
		/*
				  p			      p
				 /		         /
			 |node|            (tp)
			  /   \           /   \
			(tp)  n.r  ---> tp.l  |n|
			/ \	                  / \
		 tp.l tp.r              tp.r n.r

		*/

		RBNode* temp = node->left;
		node->left = temp->right;

		if (temp->right != nullptr) temp->right->parent = node;

		temp->parent = node->parent;

		if (node->parent == nullptr)
		{
			m_Root = temp;
		}
		else if (node == node->parent->left) node->parent->left = temp;
		else node->parent->right = temp;

		temp->right = node;
		node->parent = temp;
	}

	// 가장 작은 값 리턴
	const RBNode* front() const
	{
		RBNode* n = m_Root->left;

		if (n == nullptr)
			return m_Root;

		while (n->left != nullptr)
			n = n->left;

		return n;
	}

	// 가장 큰 값 리턴
	const RBNode* back() const
	{
		RBNode* n = m_Root->right;

		if (n == nullptr)
			return m_Root;

		while (n->right != nullptr)
			n = n->right;

		return n;
	}
	
private :
	RBNode* m_Root;

	size_t m_Count;

	_Allocator m_Allocator;
};