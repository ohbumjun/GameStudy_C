#pragma once

#include "Flag.h"
#include "GeneralAllocator.h"

const enum NODE_COLOR : bool { BLACK = false, RED = true };

template<typename T, typename V>
struct PRRedBlackNode
{
	T key;
	V value;

	PRRedBlackNode* left;
	PRRedBlackNode* right;
	PRRedBlackNode* next;

	//if have duplicate key save in bucket as linked list ///
	//only first node has left/right/parent, next linked nodes don't have any of them
	PRRedBlackNode* parent;
	PRRedBlackNode color;
};

template <typename T, typename V, typename TAllocator = AlignedAllocator<PRRedBlackNode<T, V>>>
class PRRedBlackTree
{
	static_assert(!std::is_pointer<TAllocator>::value, "TAllocator must not be pointer type, use ref type instead");
	
	// std::decay<TAllocator>::type> : remove reference, const, volatile from TAllocator type
	// ex) if TAllocator is const SomeAllocator&, then std::decay<TAllocator>::type would be SomeAllocator
	static_assert(std::is_base_of<Allocable, typename std::decay<TAllocator>::type>::value, "type parameter of this class must derive from BaseClass");

};