#pragma once

template <typename KEY, typename VALUE>
class CBinaryTreeNode
{
	template<typename KEY, typename VALUE>
	friend class CBinaryTree;
	template<typename KEY, typename VALUE>
	friend class CBinaryTreeIterator;
private :
	CBinaryTreeNode() :
		m_Left(nullptr),
		m_Right(nullptr),
		m_Parent(nullptr),
		m_Next(nullptr),
		m_Prev(nullptr){}
	~CBinaryTreeNode(){}
private :
	CBinaryTreeNode<KEY, VALUE>* m_Left;
	CBinaryTreeNode<KEY, VALUE>* m_Right;
	CBinaryTreeNode<KEY, VALUE>* m_Parent;
	CBinaryTreeNode<KEY, VALUE>* m_Next;
	CBinaryTreeNode<KEY, VALUE>* m_Prev;
public :
	KEY first;
	VALUE second;
};

template<typename KEY, typename VALUE>
class CBinaryTreeIterator
{
	template<typename KEY, typename VALUE>
	friend class CBinaryTree;
public :
	CBinaryTreeIterator() : m_Node(nullptr){}
	~CBinaryTreeIterator(){}
private :
	CBinaryTreeNode<KEY, VALUE>* m_Node;
public :
	bool operator == (const CBinaryTreeIterator<KEY, VALUE>& iter) const
	{
		return m_Node == iter.m_Node;
	}
};

