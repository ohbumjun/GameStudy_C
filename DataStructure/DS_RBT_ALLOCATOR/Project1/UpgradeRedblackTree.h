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

	// ���� Key �� Node ���� ���Ḯ��Ʈ ���·� �д�. (next pointer ���)
	// ���Ḯ��Ʈ ���� ù��° ��常�� left, right, parent �� ���Ѵ�.
	RBNode* parent;

	// Insert �ϴ� ���� ������ RED �� �����Ѵ�.
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

	// �̵� ������
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
		// Root Node �� Insert
		if (_root == nullptr)
		{
			_root = node;

			// Root Node ������ Black
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
					// �ߺ� Key ������ ���������, �ߺ� val �� ������� �ʴ´�.
					if (p->val == node->val)
						return;
					if (p->next == p)
						p->next = nullptr;
					p = p->next;
				}
				// ���� key ���� ���� Node ���� linked list ���·� �����Ѵ�.
				p->next = node;
				m_Count++;
				return;
			}
			// ���� Insert
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
			// ������ Insert
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

	//�ش� key �� ���� ��� ��� �����ֱ� 
	void Remove(T key)
	{
		RBNode* node = search(key);

		if (node == nullptr)
		{
			assert(info, "No node found");
			return;
		}

		RBNode* next = node->next;

		// ���Ḯ��Ʈ�� �ִ� ���׵��� ���� Free �����ش�.
		while (next != nullptr)
		{
			RBNode* temp = next->next;
			m_Allocator.Free(next);
			m_Count--;
			next = temp;
		}

		node->next = nullptr;

		// Tree �󿡼� �ش� ��带 ������� �Ѵ�.
		Remove(node);
	}

	//Remove "Single" with "key, value"
	void Remove(T key, V value)
	{
		RBNode* node = search(key);

		if (node == nullptr) THROW("no node found");

		// ���� key ���� ������ node ���Ḯ��Ʈ��, ù��° node �� �����´�.
		RBNode* prev = node;

		// �ش� ���Ḯ��Ʈ�� ���鼭, ���� value �� ���ϴ� ù��° node �� ã�´�.
		while (node != nullptr && node->value != value)
		{
			prev = node;
			node = node->next;
		}

		// �ش� key �� ���Ḯ��Ʈ �󿡼�, ������� �ϴ� value �� ���� ���
		// bucket �󿡼� ���� ù��° node �϶�
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
			// else �б⹮�� ���� �̻�, bucket ���� ù��° node ������ �ȵȴ�.
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
	// 1) Red Node �� �����ϴ� ����, �״�� �����ϸ� �ȴ�.
	// 2) Black Node �� �����ϴ� ��� + �� ��忡�� �ڼ� nil ������� ���� ��� ��δ� black ���� ����.
	//    ��� ��Ģ�� ���� => extra black �� ����Ѵ�.
	//    �����Ǵ� ��带 ��ü�� ���� extra black node �� �ο��� ����
	//    - red-and-black
	//    - doubly black �̳Ŀ� ���� ������ ó�� ������ �����Ѵ�.
	void Remove(RBNode* node)
	{
		if (node == nullptr)
			return;

		// ���� key �� ��尡 �ߺ����� �����Ѵٸ�, ���Ḯ��Ʈ �󿡼� �� ���� Node �� ������ ���̴�.
		if (node->next != nullptr)
		{
			node->value = node->next->value;
			auto temp = node->next->next;
			m_Allocator.Free(node->next);
			node->next = temp;
			m_Count--;

			return;
		}
		else if (isNotFirstNodeInList(node))
		{
			// ���� �����ϰ��� �ϴ� ��尡 ���Ḯ��Ʈ �󿡼� ������ node + 1st bucket node �� �ƴ�
			// node->next �� nullptr �ε�, isNotFirstNodeInList(node) �� true ��� ����
			// ���Ḯ��Ʈ �󿡼� ������ ����� ���̴�.

			// temp ��, �ش� key �� ���� ������, ���Ḯ��Ʈ���� ���� ù��° node
			RBNode* temp = search(node->key);

			// ���Ḯ��Ʈ ��, ���� ������� �ϴ� ���, �ٷ� �� ��带 ã�´�.
			while (temp->next != node)
			{
				temp = temp->next;
			}

			// ���� ��� next ���� nullptr �������ش�.
			temp->next = nullptr;

			m_Allocator.Free(node);

			m_Count--;

			return;
		}
		else 
		{
			// �ߺ� key ����, ���� ����� ��尡 �ش� key �� ������ ������ ����� ���
			// Tree �󿡼� ���� ���ߴ� �۾��� �����ؾ� �Ѵ�.

			RBNode* curNode = getSuccessorNode(node);
			RBNode* parentNode = node->parent;

			// successor Node �� ���� ���
			// ���� ����� key �� ��� ������ �����Ǵ� ����̴�.
			if (curNode == nullptr)
			{
				// root �� �׳� �����.
				if (node == m_Root)
				{
					if (m_Count != 1) THROW("NODE NOT DELETED");

					m_Root = nullptr;
				}
				else
				{
					// Black �� ���� ���, ������ ����� �Ѵ�.
					if (node->color == NODE_COLOR::BLACK)
					{
						removeBalanceDoubleBlack(node);
					}
					else
					{
						// ������ ����� ���
						RBNode* brotherNode = getBrother(node);

						if (brotherNode != nullptr)
							brotherNode->color = NODE_COLOR::RED;
					}

					if (parentNode->left == node)
						parentNode->left = nullptr;
					else
						parentNode->right = nullptr;
				}

				_allocator.Free(node);
				_count--;
				return;
			}

			// �ڽ��� �ϳ� �ְ� + succesor node �� �����ϴ� ���
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

	// ���Ḯ��Ʈ �󿡼�, ù��° ��尡 �ƴ� ����ΰ�
	// ù��° ��常�� parent, left, right ��带 ���� �� �ִ�.
	bool isNotFirstNodeInList(RBNode* n)
	{
		return n != _root && n->parent == nullptr && isLeafNode(n);
	}

	// �ش� key ���� ���� ������ Bucket ����, ���� ù��°�� �ش��ϴ� ���鸸��
	// ���ϸ鼭 search �ϳ�.
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
		// 2�� �ڽ��� ������ �ִٸ�
		if (node->left != nullptr && node->right != nullptr)
		{
			RBNode* leftNode = node->left;

			while (leftNode->right != nullptr)
				leftNode = leftNode->right;

			// ���� node ����, ���� �ѹ�, ������ �� Ÿ�� �������� succssor �� �����´�.
			return leftNode;
		}

		// leafNode �� ������ successorNode �� ������ ���� �ʰ� �ȴ�.
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
		// ��� �� ���ǹ��� ��� �ȴ�. ��� ���� ������ nullptr��� nil�� ����Ǳ� ����.
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

		// insert �� node �� leaf node �̴�.
		// RB Tree ���� ��, RED �� �ڽ��� Black �̶�� ������ �ִ�.
		// �� �� �ϳ��� Black �̶��, �ش� ������ ������ ���̹Ƿ� Fix �� �ʿ䰡 ����.
		if (curNode->color != NODE_COLOR::RED || curNode->parent->color != NODE_COLOR::RED)
			return;

		// Root ������ Ÿ�� �ö󰡸鼭 �ݺ��Ѵ�.
		while (curNode != m_Root && curNode->color == NODE_COLOR::RED 
			&& curNode->parent->color == NODE_COLOR::RED)
		{
			parent = curNode->parent;
			grdParent = getGrandNode(curNode);

			BalanceDirection nodeDir = (curNode->key < parent->key) ? BalanceDirection::LEFT : BalanceDirection::RIGHT;
			BalanceDirection parentDir = (parent->key < grdParent->key) ? BalanceDirection::LEFT : BalanceDirection::RIGHT;

			// Balance Case �� ��������.
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

			// �ڽ� Red, �θ� Red, Uncle Red
			if (uncleNode != nullptr && uncleNode->color == NODE_COLOR::RED)
			{
				// ���θ� ~ (�θ�, ����) ���� ��� �������ش�.
				uncleNode->color = NODE_COLOR::BLACK;
				parent->color = NODE_COLOR::BLACK;
				grdParent->color = NODE_COLOR::RED;

				// �׸��� �ٽ� (���θ�) ���� ���� �����Ѵ�.
				curNode = grdParent;
			}
			// �θ�� Red, Uncle �� Black �� ���
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

		// ���� 1) ��ȸ���� ������ �ڽĳ�带 ����Ű�� �ӽ� ������ ����.
		RBNode* temp = node->right;

		// ���� 2) cur�� temp�� �и���Ű�鼭, temp�� ���� �ڽĳ�带 cur �� ������ ���� ����
		node->right = temp->left;

		// ���� 3) ��� �μ��ΰ��Ѱ� nil�� �ƴ϶��, temp �� ���� �ڽĳ���� �θ� cur�� ����.
		if (temp->left != nullptr) temp->left->parent = node;

		// ���� 4) temp�� �θ� cur���� cur->parent�� �ٲ��ش�.
		temp->parent = node->parent;

		// ���� 5) cur�� ���� ���¿� ���� temp�� �̾��ִ� ����� ���� ����.
		// cur->parent �� �ڽ����� temp �� �������� ���̴�. cur->right �� �ڽ����� ����
		if (node->parent == nullptr)
		{
			m_Root = temp;
		}
		else if (node == node->parent->left) node->parent->left = temp;
		else node->parent->right = temp;

		// ���� 6) ���� ��� �����غ� �������� temp�� ������ �ڽĳ�带 cur�� ������.
		temp->right = node;

		// ���� 7) ���� cur�� �θ� temp�� �Ǹ� ��ȸ�� �Ϸ�.
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

	// ���� ���� �� ����
	const RBNode* front() const
	{
		RBNode* n = m_Root->left;

		if (n == nullptr)
			return m_Root;

		while (n->left != nullptr)
			n = n->left;

		return n;
	}

	// ���� ū �� ����
	const RBNode* back() const
	{
		RBNode* n = m_Root->right;

		if (n == nullptr)
			return m_Root;

		while (n->right != nullptr)
			n = n->right;

		return n;
	}

	void removeBalanceDoubleBlack(RBNode* node)
	{
		// ���� node->color == Doubly Black �̴�.
		// Doubly Black �� �Ǵ� ���� 2����
		// 1) ������� �ϴ� node �� black �̰�
		// 2) Ư�� ��忡�� ��� �ڽ� nil node ���� ���� black height �� �����ؾ� �Ѵٴ� 
		//    ������ ����� ��

		if (node == m_Root)
		{
			return;
		}

		RBNode* brother = getBrother(node);
		RBNode* parent	= node->parent;

		if (brother == nullptr)
		{
			// �θ�� �ö󰡼� ���� ���ߴ� �۾��� �����Ѵ�.
			removeBalanceDoubleBlack(parent);
		}
		else
		{
			// brother ���� ���� �����Ѵ�.
			if (brother->color == NODE_COLOR::RED)
			{
				// node		: black
				// brother	: red

				// parent node �������� ȸ���� �ϱ� ����
				// brother node �� parent node ������ ���� �ٲ��ش�.
				parent->color = NODE_COLOR::RED;
				brother->color = NODE_COLOR::BLACK;

				if (brother->parent->left == brother)
				{
					// brother �� parent �� ���� �ڽ��̶��
					rightRotate(parent);
				}
				else
				{
					leftRotate(parent);
				}

				// rotate ����, ���� node �� ���ؼ� ���� ���ߴ� �۾��� �����Ѵ�.
				removeBalanceDoubleBlack(node);
			}
			else
			{
				// brother->color �� black �̶��
				bool leftBrotherChild	= brother->left != nullptr && brother->left->color == NODE_COLOR::RED;
				bool rightBrotherChild	= brother->right != nullptr && brother->right->color == NODE_COLOR::RED;
			
				// brother �� �ڽ� ���� ���� �ٸ� ó���� ������ ���̴�
				if (leftBrotherChild || rightBrotherChild)
				{
					// ��, brother, node �� ���� �Ѵ� black �� ��Ȳ����
					// brother �� �ڽ� �� �ϳ��� red �� �ִٸ�
					if (leftBrotherChild)
					{
						if (brother->parent->left == brother)
						{
							// left left
							// �ٽ� : leftBrotherChild �� ������ ���� �ø��� ���̴�.
							// 1) leftBrotherChild ~ ? node �� ������ red ��, �� �θ���
							//    brother ���� �ø���
							brother->left->color = brother->color;
							// 2) ȸ�� ������ brother �� parent �� ������ �������ش�.
							brother->color = parent->color;
							rightRotate(parent);
						}
						else
						{
							// right right
							
							// 1) right right �� �����.
							// 2) brother->left �� right child �� brother �� �ȴ�.
							rightRotate(brother);

							// 3) parent �� right child �� brother->left �� �Ǵ� ��Ȳ����
							//    parent ���� ���� ȸ���� �ؾ� �Ѵ�.
							//	  ���� parent �� brother->left �� ������ �ٲ��ִ� ���̴�.
							leftRotate(parent);
							brother->left->color = parent->color;
						}
					}
					else
					{
						// rightBrotherChild == true
						if (brother->parent->left == brother)
						{
							// left right
							// brother �� parent �� left + brother �� right child

							// 1. left, left �� �����.
							// 2. brother->left �� left child �� brother �� ����.
							leftRotate(brother);

							// 3. ���� parent�� left child �� brother->left �̴�.
							// parent ���� ���� ȸ���� �ϱ� ���� parent �� brother->left �� 
							// ������ �ٲ��ش�.
							brother->right->color = parent->color;

							rightRotate(parent);
						}
						else
						{
							// right right
							
							brother->right->color = brother->color;

							brother->color = parent->color;

							leftRotate(parent);
						}
					}

					// ȸ�� ����, ���� �����ϰ��� �ϴ� node �� black ������
					// ȸ�� ���Ŀ��� ������, �����ϰ��� �ϴ� node �� parent �� parent �� �����ϰ��� �Ѵ�.
					parent->color = Color::BLACK;
				}
				else
				{
					// ���� node, brother, children ��� black �� ��
					// node, brother �� black �� ��Ƽ� parent �� �ű��
					// paernt �� double black Ȥ�� red-black �� �ȴ�. �̶� parent ���� �ٽ� �����Ѵ�.
					brother->color = NODE_COLOR::RED;

					if (brother->color == NODE_COLOR::BLACK)
					{
						removeBalanceDoubleBlack(parent);
					}
					else
					{
						// red-black �� black ���� ������ָ� �ȴ�.
						parent->color = NODE_COLOR::BLACK;
					}
				}
			}
		}
	}
	
private :
	RBNode* m_Root;

	size_t m_Count;

	_Allocator m_Allocator;
};