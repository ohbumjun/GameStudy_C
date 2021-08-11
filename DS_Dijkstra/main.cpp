#include<iostream>
#include"CDijkstra.h"

int main()
{
	CDijkstra<const char*> dijkstra;

	dijkstra.insert("1");
	dijkstra.insert("1");
	dijkstra.insert("1");
	dijkstra.insert("1");
	dijkstra.insert("1");
	dijkstra.insert("1");

	dijkstra.AddEdge("1", "2", 10);
	dijkstra.AddEdge("1", "6", 5);

	dijkstra.AddEdge("2", "3", 5);
	dijkstra.AddEdge("2", "5", 9);

	dijkstra.AddEdge("3", "5", 5);
	dijkstra.AddEdge("3", "4", 9);

	CList<const char*> FindList;
	if (dijkstra.Find(&FindList, "1", "4");
	{
		CList<const char*>::iterator iter;
			for (iter = FindList.begin(); iter != FindList.end(); ++iter)
			{

			}
	}
	else
	{

	}


	return 0;
}