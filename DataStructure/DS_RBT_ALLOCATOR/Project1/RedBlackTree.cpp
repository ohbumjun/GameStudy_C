#include "Flag.h"
#include "RedBlackTree.h"

Node* RedBlackTree::getGrandNode(Node* cur)
{
	// 사실 이 조건문은 없어도 된다. 모든 노드는 생성시 nullptr대신 nil로 연결되기 때문.
	if (cur == nullptr || cur->parent == nullptr) return nullptr;
	return cur->parent->parent;
}

Node* RedBlackTree::getUncleNode(Node* cur)
{
	Node* tempGrand(getGrandNode(cur));
	if (tempGrand == nullptr) return nullptr;
	// 조부모의 오른쪽 자식노드가 부모노드면 왼쪽이 삼촌노드다.
	if (cur->parent == tempGrand->left) return tempGrand->right;
	// 조부모의 왼쪽 자식노드가 부모노드면 오른쪽이 삼촌노드다.
	else return tempGrand->left;
}

void RedBlackTree::leftRotate(Node* cur)
{
	// 과정 1) 좌회전시 오른쪽 자식노드를 가리키는 임시 포인터 생성.
	Node* temp = cur->right;

	// 과정 2) cur과 temp를 분리시키면서, temp의 왼쪽 자식노드를 cur 의 오른쪽 노드로 세팅
	cur->right = temp->left;

	// 과정 3) 방금 인수인계한게 nil이 아니라면, temp 의 왼쪽 자식노드의 부모를 cur로 지정.
	if (temp->left != nil) temp->left->parent = cur;

	// 과정 4) temp의 부모를 cur에서 cur->parent로 바꿔준다.
	temp->parent = cur->parent;

	// 과정 5) cur의 현재 상태에 따라서 temp를 이어주는 경우의 수가 나뉨.
	// cur->parent 의 자식으로 temp 를 세팅해줄 것이다. cur->right 을 자식으로 세팅
	if (cur->parent == nil) nil->right = temp;
	else if (cur == cur->parent->left) cur->parent->left = temp;
	else cur->parent->right = temp;

	// 과정 6) 이제 모든 사전준비가 끝났으니 temp의 왼쪽 자식노드를 cur로 지정함.
	temp->left = cur;

	// 과정 7) 이제 cur의 부모가 temp가 되면 좌회전 완료.
	cur->parent = temp;
}

void RedBlackTree::rightRotate(Node* cur)
{
	// left Rotate에서 right과 left만 바뀌고 나머지는 똑같다.
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
	// 부모가 root 노드일 때 까지 반복한다. (root노드는 흑색)
	while (cur->parent->color == Color::RED)
	{
		Node* grandNode(getGrandNode(cur));		// 조부모 노드
		Node* uncleNode(getUncleNode(cur));		// 삼촌 노드

		// 부모가 왼쪽 자식인 경우 (-> 삼촌은 오른쪽)
		if (cur->parent == grandNode->left)
		{	
			// [CASE 1]: 부모도 삼촌도 모두 빨간색인 경우
			if (uncleNode->color == Color::RED)
			{	
				// (부모와 삼촌) 그리고 조부모 노드의 색깔을 반전시켜준다.
				cur->parent->color = uncleNode->color = Color::BLACK;
				grandNode->color = Color::RED;
				cur = grandNode;	// cur을 조부모로 올려서 다음 loop를 준비한다.
			}
			else
			{
				// [CASE 2]: 삼촌이 흑색이고, cur이 오른쪽 자식노드인 경우
				if (cur == cur->parent->right)
				{
					cur = cur->parent;	// cur을 한 칸 올리고,
					leftRotate(cur);	// 좌회전 한다.
				}
				// [CASE 3]: 삼촌이 흑색이고, cur이 왼쪽 자식노드인 경우
				cur->parent->color = !cur->parent->color;	// 부모 색깔 반전
				grandNode->color = !grandNode->color;		// 조부모 색깔 반전
				rightRotate(grandNode);	// 우회전 한다. (주의! grandNode로 우회전)
			}
		}
		// 부모가 오른쪽 자식인 경우 (-> 삼촌은 왼쪽)
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
				// 삼촌이 흑색이고, cur이 왼쪽 자식노드인 경우
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
	parent = nil;				// 일반적으로 parent가 nil을 가리킨다.
	tail = nil->right;			// 일반적으로 tail이 root 노드를 가리킨다.

	while (tail != nil)			// [과정 1]: 삽입할 위치를 찾는다.
	{
		parent = tail;			// parent를 tail로 한 칸 내려야 tail을 이동시킬 수 있다.
		int var = tail->key;
		if (data == var)		// [오류]: 트리에 이미 값이 존재할 경우 uniqueness에 위반된다.
		{
			cout << "[ERROR] Duplicated input value!" << endl;
			return;
		}
		// 삽입할 값이 현재 노드의 key보다 작다면 왼쪽 자식노드로 내려간다.
		else if (data < var) tail = tail->left;
		// 삽입할 값이 현재 노드의 key보다 크다면 오른쪽 자식노드로 내려간다.
		else tail = tail->right;
	}	// 반복문을 탈출함 = 삽입할 위치가 parent에 저장되었음을 의미한다.

	// new 함수를 이용해 새로운 노드를 만든다.
	// 새로운 노드는 무조건 RED 로 세팅하고 시작한다.
	tail = new Node(data, Color::RED, parent, nil, nil);	

	if (parent == nil)	// root 노드를 삽입하는 경우,
	{
		parent->right = tail;	// nil->right = tail로 root노드로 만들어주고
		tail->color = Color::BLACK;	// root 노드는 반드시 반드시 흑색이어야 한다.
	}

	// 삽입할 값이 삽입 위치의 노드의 key보다 작다면 왼쪽 자식노드에 삽입한다.
	// 삽입할 값이 삽입 위치의 노드의 key보다 크다면 오른쪽 자식노드에 삽입한다.
	else if (data < parent->key) parent->left = tail;
	else parent->right = tail;

	this->numOfElement++;		// 트리의 노드 개수를 1 증가시킨다.
	insertFix(tail);		// 레드블랙트리의 특성을 만족하도록 매 삽입연산 때 트리 구조를 수정한다.

}

void RedBlackTree::preOrderTraverse(Node* root, std::string& stringBuilder, std::string& padding, const std::string& pointer, const bool hasRightChild) const
{
}

std::string RedBlackTree::showTree(Node* root) const
{
	return std::string();
}
