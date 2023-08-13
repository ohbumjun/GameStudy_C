#pragma once
#include <iostream>
#include <string>

using namespace std;

namespace Color
{
	const enum NODE_COLOR : bool { BLACK = false, RED = true };
};

class Node
{
	friend class RedBlackTree;
private :
	int key;   
	bool color;
	Node* parent, * left, * right;
	friend class RedblackTree;
public :
	explicit Node(const int& paramKey = NULL, const bool paramColor = Color::BLACK,
		Node* paramParent = nullptr, Node* paramLeft = nullptr, Node* paramRight = nullptr) :
		key(paramKey), color(paramColor), parent(paramParent), left(paramLeft),
		right(paramRight) {}
};

// https://blog.naver.com/PostView.naver?blogId=leeinje66&logNo=221945641530
class RedBlackTree
{
private :
	int numOfElement; // 트리 내 존재하는 원소 개수
	
	// 2가지 역할을 수행한다.
	// 1) Root Node 를 가리키는 역할을 한다.
	// 2) 모든 leaft 노드들의 tail 역할도 같이 한다.
	// - 모든 적색 노드의 자식 노드는 흑색이다. 라는 조건을 만족시키도록 돕는다.
	Node* nil;        

private :
	Node* getGrandNode(Node* cur);
	Node* getUncleNode(Node* cur);
	void leftRotate(Node* cur);
	void rightRotate(Node* cur);
	void insertFix(Node* cur); // 균형 맞도록 트리 구조 변경
	void insertNode(Node* cur, const int& data);
	void preOrderTraverse(Node* root, std::string& stringBuilder, std::string& padding,
		const std::string& pointer, const bool hasRightChild) const ;
	std::string showTree(Node* root) const; // pre-order traverse 출력 함수 

public:		
	// 멤버 함수 (public)
	explicit RedBlackTree() : numOfElement(0), nil(new Node(-1, Color::BLACK)) {

		// nil 노드의 왼쪽, 오른쪽, parent 모두 nil 노드로 세팅한다.
		nil->left = nil->right = nil->parent = nil;

		// 초기화 리스트에서 nil(new Node(-1, Color::BLACK, nil, nil, nil) 하면 안됨.
		// nil을 생성할 때는 nil이 nullptr이므로 멤버 포인터 변수가 nullptr이 되기 때문이다.
	}
	void insertNode(const int& data) { insertNode(nil, data); }
	std::string showTree() const { return showTree(nil->right); }


};

