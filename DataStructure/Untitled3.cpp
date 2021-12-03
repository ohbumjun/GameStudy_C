#include<iostream>
#include<string>
#include<vector>
#include <algorithm>

using namespace std;

void permutation(vector<int>& vec, int k)
{
	if (k == vec.size() - 1)
	{
		for(int i = 0; i < vec.size(); i++)
			cout << vec[i] << ", ";
		return;
	}
	for(int i = k; i < vec.size(); i++)
	{
		int temp = vec[i];
		vec[i] = vec[k];
		vec[k] = temp;
		
		permutation(vec , k+1);
		
		temp = vec[i];
		vec[i] = vec[k];
		vec[k] = temp;
	}
}

int main()
{
	std::vector<int> vec = {10,40,30,50};
	permutation(vec,0);
	return 0;
}
