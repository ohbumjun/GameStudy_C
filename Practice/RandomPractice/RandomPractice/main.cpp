#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace std;

void Bresenham(int stR, int stC, int edR, int edC, vector<pair<int, int>>& vecP)
{
	// 가로 
	int x = stC;
	// 세로 
	int y = stR;

	int dx = edC - stC;
	int dy = edR - stR;

	int detP = 2 * dy - dx;

	cout << "dx,dy : " << dx << "." << dy << endl;

	while (x <= edC)
	{
		cout << "x,y : " << x << "." << y << endl;
		cout << "detP : " << detP << endl;

		vecP.push_back(make_pair(x, y));
		++x;

		if (detP < 0)
			detP = detP + 2 * dy;
		else
		{
			detP = detP + 2 * dy - 2 * dx;
			y++;
		}
	}
}

int main()
{
	//
	vector<pair<int, int>> vecPixel;
	vector<vector<int>> vecVec;
	vecVec.resize(8);

	for (int i = 0; i < 8; ++i)
		vecVec[i].resize(8);

	Bresenham(0, 0, 4, 7, vecPixel);

	for (const auto& e : vecPixel)
		cout << e.first << ", " << e.second << ".";
	cout << endl;

	return 0;
}