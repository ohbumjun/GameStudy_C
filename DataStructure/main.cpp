#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <forward_list>
#include <algorithm>

using namespace std;


int main()
{
	vector<int> v1{ 10, 30, 20 };
	sort(v1.begin(), v1.end());

	do
	{
		for (const auto& e : v1)
			cout << e << ", ";
		cout << endl;
	} while (std::next_permutation(v1.begin(), v1.end()));
	return 0;
}