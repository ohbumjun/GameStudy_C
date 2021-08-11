#pragma once
#include<assert.h>
#include "Queue.h"
#include "CDijkstra.h"
#include "List.h"

template<typename T>
class CEdge
{
	template<typename T>
	friend class CDijkstra;
	template<typename T>
	friend class CDijkstraNode;
};