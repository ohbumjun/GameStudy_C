#include <iostream>

using namespace std;

class A
{
public :
	~A()
	{
		cout << "A destruct" << endl;
	}
};

int main()
{
	A** Array_A = new A*[5]; // ���� ������ 1���� �迭�� ���� ������  
	for (int i = 0; i < 5; i++)
	{
		Array_A[i] = new A;
		cout << (Array_A[i][0]) << endl;	
	}

	for (int i = 0; i < 5; i++)
	{
		// cout << "i : " << i << endl;
		delete Array_A[i];
	}
	delete [] Array_A;
	
	// A* A_A = new A[5];
	// delete [] A_A;
	
	return 0;
}
