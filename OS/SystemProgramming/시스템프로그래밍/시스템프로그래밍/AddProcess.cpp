#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// AddProcess.cpp
// - main �Լ��� �������ڸ� �����ϴ� ���α׷�
int _tmain(int argc, TCHAR* argv[])
{
	DWORD val1, val2;

	// __ttoi �Լ� ?
	// - ���ڿ� �����͸� ������ �����ͷ� �����Ű�� atoi +
	// �����ڵ� ����� _wtoi �� ���� �����ϱ� ���� ��ũ��
	val1 = _ttoi(argv[1]);
	val2 = _ttoi(argv[2]);

	_tprintf(_T("%d + %d = %d \n"), val1, val2, val1 + val2);

	// ���α׷� ������ ��� ���߱� ����
	// getchar() �Լ��� �����ڵ� ����� getwchar() �Լ��� ���� �����ϱ� ���� ��ũ��
	_gettchar();

	return 0;
}