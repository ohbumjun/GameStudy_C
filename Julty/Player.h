#pragma once

#include "GameInfo.h"

// CMonster Ŭ������ ���漱�� �Ͽ� CMonster ��� Ŭ������ ��򰡿� �ִ� ��� �����ϰ� ����� �� �ְ� �Ѵ�.
// ���漱��� �� Ÿ���� ������ �����ϴ°��� �Ұ����ϰ� �� Ÿ�Կ� ���� �޸� �ּҸ��� ����� �� �ִ�.
// ��, �����ͷθ� ����� �����ϴٴ� ���̴�.
class CMonster;
class CPlayer
{
public:
    CPlayer();
    ~CPlayer();

private:
    // �Ʒ�ó�� ���� ��ü�� ����� ���ؼ��� �ݵ�� �� Ŭ������ �ִ� ��������� include�� �ϰ�
    // ��ü�� ���� ����ؾ� �Ѵ�.
    //CMonster    m_Monster;
    CMonster* m_pMonster;

public:
    void Output();
};

