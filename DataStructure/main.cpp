#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <forward_list>
#include <algorithm>

using namespace std;

// NO !
#define MAX_SPEED 200

// ���� �ش� ������� Car ����ü �ȿ����� �ǹ��ִ� ������̶��
// �ٸ� �������� ����ϵ��� ���ǵ� ����� �ƴϴ�
// ����, ����ü ���� ���Խ�Ű�� ���� 
struct Car
{
	enum
	{
		MAX_SPD   = 200,
		FUEL_STEP = 2,
		ACC_STEP  = 10,
		BRK_STEP  = 10
	};
};

// ����, enum�� ������ ����ü ���ο� �����ϴ� ���� �δ㽺���ٸ�
// �̸� ������ �̿��ؼ�, ����� ���Ǵ� ������ ����ϴ� �͵� ���� ��� 
namespace CAR_CONST
{
	enum
	{
		MAX_SPD = 200,
		FUEL_STEP = 2,
		ACC_STEP = 10,
		BRK_STEP = 10
	};
}




int main()
{
	const int num = 12;
	const int * ptr = &num;
	const int* (&ref) = ptr;
	cout << *ptr << endl;
	cout << *ref << endl;

	return 0;
}