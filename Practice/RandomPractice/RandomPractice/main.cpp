#include <iostream>
#include <mutex>  // mutex �� ����ϱ� ���� �ʿ�
#include <thread>
#include <vector>
#include <string>

using namespace std;

int gstrcpy(char* src, char* dest)
{
    int sum = 0;

    for (int i = 0; i < sizeof(dest); ++i)
    {
        src[i] = dest[i];
    }
    return sum;
}

int main() {

    char cArra[] = "hello1";
    char cArra2[] = "hello2";

    gstrcpy(cArra, cArra2);

    cout << cArra << endl;

    return 0;
}

// Insertion Sort �˰����� ����ؼ� ���� ����
// ������ ������ List�� ������ ����
// head �� ��ȯ�ϴ� �Լ��� ���弼��
// (�⺻������ �˰� �ִ� InsertionNode �� �ݴ븦 ����Ѵٰ� �����ϱ�)