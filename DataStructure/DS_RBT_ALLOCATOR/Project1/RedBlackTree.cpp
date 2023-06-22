#include "Flag.h"
#include "RedBlackTree.h"

Node* RedBlackTree::getGrandNode(Node* cur)
{
	// ��� �� ���ǹ��� ��� �ȴ�. ��� ���� ������ nullptr��� nil�� ����Ǳ� ����.
	if (cur == nullptr || cur->parent == nullptr) return nullptr;
	return cur->parent->parent;
}

Node* RedBlackTree::getUncleNode(Node* cur)
{
	Node* tempGrand(getGrandNode(cur));
	if (tempGrand == nullptr) return nullptr;
	// ���θ��� ������ �ڽĳ�尡 �θ���� ������ ���̳���.
	if (cur->parent == tempGrand->left) return tempGrand->right;
	// ���θ��� ���� �ڽĳ�尡 �θ���� �������� ���̳���.
	else return tempGrand->left;
}

void RedBlackTree::leftRotate(Node* cur)
{
	// ���� 1) ��ȸ���� ������ �ڽĳ�带 ����Ű�� �ӽ� ������ ����.
	Node* temp = cur->right;

	// ���� 2) cur�� temp�� �и���Ű�鼭, temp�� ���� �ڽĳ�带 cur �� ������ ���� ����
	cur->right = temp->left;

	// ���� 3) ��� �μ��ΰ��Ѱ� nil�� �ƴ϶��, temp �� ���� �ڽĳ���� �θ� cur�� ����.
	if (temp->left != nil) temp->left->parent = cur;

	// ���� 4) temp�� �θ� cur���� cur->parent�� �ٲ��ش�.
	temp->parent = cur->parent;

	// ���� 5) cur�� ���� ���¿� ���� temp�� �̾��ִ� ����� ���� ����.
	// cur->parent �� �ڽ����� temp �� �������� ���̴�. cur->right �� �ڽ����� ����
	if (cur->parent == nil) nil->right = temp;
	else if (cur == cur->parent->left) cur->parent->left = temp;
	else cur->parent->right = temp;

	// ���� 6) ���� ��� �����غ� �������� temp�� ���� �ڽĳ�带 cur�� ������.
	temp->left = cur;

	// ���� 7) ���� cur�� �θ� temp�� �Ǹ� ��ȸ�� �Ϸ�.
	cur->parent = temp;
}

void RedBlackTree::rightRotate(Node* cur)
{
	// left Rotate���� right�� left�� �ٲ�� �������� �Ȱ���.
	Node* temp = cur->left;
	cur->left = temp->right;

	if (temp->right != nil) temp->right->parent = cur;

	temp->parent = cur->parent;

	if (cur->parent == nil) nil->right = temp;
	else if (cur == cur->parent->left) cur->parent->left = temp;
	else cur->parent->right = temp;

	temp->right = cur;
	cur->parent = temp;
}

void RedBlackTree::insertFix(Node* cur)
{
	// �θ� root ����� �� ���� �ݺ��Ѵ�. (root���� ���)
	while (cur->parent->color == Color::RED)
	{
		Node* grandNode(getGrandNode(cur));		// ���θ� ���
		Node* uncleNode(getUncleNode(cur));		// ���� ���

		// �θ� ���� �ڽ��� ��� (-> ������ ������)
		if (cur->parent == grandNode->left)
		{	
			// [CASE 1]: �θ� ���̵� ��� �������� ���
			if (uncleNode->color == Color::RED)
			{	
				// (�θ�� ����) �׸��� ���θ� ����� ������ ���������ش�.
				cur->parent->color = uncleNode->color = Color::BLACK;
				grandNode->color = Color::RED;
				cur = grandNode;	// cur�� ���θ�� �÷��� ���� loop�� �غ��Ѵ�.
			}
			else
			{
				// [CASE 2]: ������ ����̰�, cur�� ������ �ڽĳ���� ���
				if (cur == cur->parent->right)
				{
					cur = cur->parent;	// cur�� �� ĭ �ø���,
					leftRotate(cur);	// ��ȸ�� �Ѵ�.
				}
				// [CASE 3]: ������ ����̰�, cur�� ���� �ڽĳ���� ���
				cur->parent->color = !cur->parent->color;	// �θ� ���� ����
				grandNode->color = !grandNode->color;		// ���θ� ���� ����
				rightRotate(grandNode);	// ��ȸ�� �Ѵ�. (����! grandNode�� ��ȸ��)
			}
		}
		// �θ� ������ �ڽ��� ��� (-> ������ ����)
		else
		{
			if (uncleNode->color == Color::RED)
			{
				cur->parent->color = uncleNode->color = Color::BLACK;
				grandNode->color = Color::RED;
				cur = grandNode;
			}
			else
			{
				// ������ ����̰�, cur�� ���� �ڽĳ���� ���
				if (cur == cur->parent->left)
				{
					cur = cur->parent;
					leftRotate(cur);
				}
				cur->parent->color = !cur->parent->color;
				grandNode->color = !grandNode->color;
				rightRotate(grandNode);
			}
		}
	}

	nil->right->color = Color::BLACK;
}

void RedBlackTree::insertNode(Node* cur, const int& data)
{
	Node* parent, * tail;
	parent = nil;				// �Ϲ������� parent�� nil�� ����Ų��.
	tail = nil->right;			// �Ϲ������� tail�� root ��带 ����Ų��.

	while (tail != nil)			// [���� 1]: ������ ��ġ�� ã�´�.
	{
		parent = tail;			// parent�� tail�� �� ĭ ������ tail�� �̵���ų �� �ִ�.
		int var = tail->key;
		if (data == var)		// [����]: Ʈ���� �̹� ���� ������ ��� uniqueness�� ���ݵȴ�.
		{
			cout << "[ERROR] Duplicated input value!" << endl;
			return;
		}
		// ������ ���� ���� ����� key���� �۴ٸ� ���� �ڽĳ��� ��������.
		else if (data < var) tail = tail->left;
		// ������ ���� ���� ����� key���� ũ�ٸ� ������ �ڽĳ��� ��������.
		else tail = tail->right;
	}	// �ݺ����� Ż���� = ������ ��ġ�� parent�� ����Ǿ����� �ǹ��Ѵ�.

	// new �Լ��� �̿��� ���ο� ��带 �����.
	// ���ο� ���� ������ RED �� �����ϰ� �����Ѵ�.
	tail = new Node(data, Color::RED, parent, nil, nil);	

	if (parent == nil)	// root ��带 �����ϴ� ���,
	{
		parent->right = tail;	// nil->right = tail�� root���� ������ְ�
		tail->color = Color::BLACK;	// root ���� �ݵ�� �ݵ�� ����̾�� �Ѵ�.
	}

	// ������ ���� ���� ��ġ�� ����� key���� �۴ٸ� ���� �ڽĳ�忡 �����Ѵ�.
	// ������ ���� ���� ��ġ�� ����� key���� ũ�ٸ� ������ �ڽĳ�忡 �����Ѵ�.
	else if (data < parent->key) parent->left = tail;
	else parent->right = tail;

	this->numOfElement++;		// Ʈ���� ��� ������ 1 ������Ų��.
	insertFix(tail);		// �����Ʈ���� Ư���� �����ϵ��� �� ���Կ��� �� Ʈ�� ������ �����Ѵ�.

}

void RedBlackTree::preOrderTraverse(Node* root, std::string& stringBuilder, std::string& padding, const std::string& pointer, const bool hasRightChild) const
{
}

std::string RedBlackTree::showTree(Node* root) const
{
	return std::string();
}
