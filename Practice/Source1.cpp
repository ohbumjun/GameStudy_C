
#include <iostream>
#include "sample.h"

int main()
{
	CAVLTree<int, int>	tree;

	for (int i = 0; i < 10; ++i)
	{
		tree.insert(i, i);
	}

	tree.erase(5);
	tree.erase(4);
	tree.erase(6);

	CAVLTree<int, int>::iterator	iter;

	for (iter = tree.begin(); iter != tree.end(); ++iter)
	{
		std::cout << "Key : " << iter->first << " Value : " << iter->second << std::endl;
		std::cout << "ParentKey : ";

		if (iter->IsParent())
			std::cout << iter->GetParentKey();

		else
			std::cout << "없음";

		std::cout << " Parent Value : ";

		if (iter->IsParent())
			std::cout << iter->GetParentValue() << std::endl;

		else
			std::cout << "없음" << std::endl;

		std::cout << "LeftKey : ";

		if (iter->IsLeft())
			std::cout << iter->GetLeftKey();

		else
			std::cout << "없음";

		std::cout << " Left Value : ";

		if (iter->IsLeft())
			std::cout << iter->GetLeftValue() << std::endl;

		else
			std::cout << "없음" << std::endl;

		std::cout << "RightKey : ";

		if (iter->IsRight())
			std::cout << iter->GetRightKey();

		else
			std::cout << "없음";

		std::cout << " Right Value : ";

		if (iter->IsRight())
			std::cout << iter->GetRightValue() << std::endl;

		else
			std::cout << "없음" << std::endl;

		std::cout << std::endl;
	}

	return 0;
}
