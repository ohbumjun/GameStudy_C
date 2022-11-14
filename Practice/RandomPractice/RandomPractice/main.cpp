#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <vector>

using namespace std;

template<typename T, typename Pred>
void Erase(T& t, Pred P)
{
	auto iter = t.begin();
	auto iterEnd = t.end();

	for (; iter != iterEnd;)
	{
		if (P(iter->first))
		{
			iter = t.erase(iter);
			continue;
		}
		++iter;
	}
};

struct PredP
{
	bool operator () (int elem)
	{
		return (elem & 1) == 0;
	}
};

int main() {
	
	std::unordered_map<int, int> mapKeys = { {1,3},{2,4},{3,5} };

	Erase(mapKeys, PredP());

	for (const auto& [k, v] : mapKeys)
	{
		cout << "k,v : " << k << "," << v << endl;
	}
	
	return 0;
}

// vec 내에서, 3이라는 값이 몇번째 Idx 에 위치하는지를 구하는 함수 ?
// find 함수 + 그 외 함수