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
	int numOfElement; // Ʈ�� �� �����ϴ� ���� ����
	
	// 2���� ������ �����Ѵ�.
	// 1) Root Node �� ����Ű�� ������ �Ѵ�.
	// 2) ��� leaft ������ tail ���ҵ� ���� �Ѵ�.
	// - ��� ���� ����� �ڽ� ���� ����̴�. ��� ������ ������Ű���� ���´�.
	Node* nil;        

private :
	Node* getGrandNode(Node* cur);
	Node* getUncleNode(Node* cur);
	void leftRotate(Node* cur);
	void rightRotate(Node* cur);
	void insertFix(Node* cur); // ���� �µ��� Ʈ�� ���� ����
	void insertNode(Node* cur, const int& data);
	void preOrderTraverse(Node* root, std::string& stringBuilder, std::string& padding,
		const std::string& pointer, const bool hasRightChild) const ;
	std::string showTree(Node* root) const; // pre-order traverse ��� �Լ� 

public:		
	// ��� �Լ� (public)
	explicit RedBlackTree() : numOfElement(0), nil(new Node(-1, Color::BLACK)) {

		// nil ����� ����, ������, parent ��� nil ���� �����Ѵ�.
		nil->left = nil->right = nil->parent = nil;

		// �ʱ�ȭ ����Ʈ���� nil(new Node(-1, Color::BLACK, nil, nil, nil) �ϸ� �ȵ�.
		// nil�� ������ ���� nil�� nullptr�̹Ƿ� ��� ������ ������ nullptr�� �Ǳ� �����̴�.
	}
	void insertNode(const int& data) { insertNode(nil, data); }
	std::string showTree() const { return showTree(nil->right); }


};

