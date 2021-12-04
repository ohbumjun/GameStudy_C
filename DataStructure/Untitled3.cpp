#include <iostream>

using namespace std;

class SpaceShip
{
public:
    int color;
    void Fire() { cout << "Fire Missile" << endl; }
};

// 1) ����� ����� ����� �߰�
// - ������ ����� �߰� : �ڵ带 �ۼ��� ��, ����� �߰��Ǵ� ��
// - Ŭ������ ����� �߰��ϴ� ��
class LeftMissle : public SpaceShip
{
public:
    void Fire()
    {
        SpaceShip::Fire(); // ���� ��� ���
        cout << "Fire Left Missile" << endl;
    }
};

// 2) ������ ����� ����� �߰�
// - Ŭ������ �ƴ� ��ü�� ����� �߰��� �� �ִ�
// - ���� �ð��� �������� ����� �߰��ȴ�.
class RightMissile : public SpaceShip
{
    SpaceShip *ss; // ���� ��ü --> ����
public:
    RightMissile(SpaceShip *p) : ss(p) {}
    void Fire()
    {
        ss->Fire(); // ���� ��ü�� ����� �����ϰ�
        cout << "Fire Left Missile" << endl;
    }
};

int main()
{
    SpaceShip ss;
    ss.Fire();

    LeftMissle lm;
    lm.Fire();

    //OCP����!
    RightMissile rm(&lm); // error.- ������ �θ� �ʿ��ϴ�.
    // RightMissile rm(&ss);
    rm.Fire();
    
    return 0;
}
