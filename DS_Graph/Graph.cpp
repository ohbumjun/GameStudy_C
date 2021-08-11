#include "Graph.h"
#include "Queue.h"
#include "Stack.h"
#include<iostream>

void OutputNode(const int& data)
{
	std::cout << data << " -> ";
}


int main()
{
	CGraph<int> graph;
	
	for (int i = 1; i < 9; ++i)
	{

		graph.insert(1);
	}

	graph.AddEdge(1, 2);
	graph.AddEdge(1, 5);
	graph.AddEdge(1, 6);
	graph.AddEdge(2, 3);
	graph.AddEdge(5, 6);
	graph.AddEdge(7, 8);
	graph.AddEdge(3, 4);

	std::cout << "------ BFS ------" << std::endl;
	graph.BFS(OutputNode);

	return 0;
}