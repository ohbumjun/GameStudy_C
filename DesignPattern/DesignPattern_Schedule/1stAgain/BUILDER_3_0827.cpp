#include <vector>
#include <iostream>

using namespace std;

struct Weapon
{
    int m_Power;
};

struct Armor
{
    int m_Defense;
};

// Product
class Character
{
public:
    Weapon *m_Weapon;
    Armor *m_Armor;
    void DisplayParts()
    {
        cout << "Weapon power" << m_Weapon->m_Power << endl;
        cout << "Armor Defense" << m_Armor->m_Defense << endl;
    }
};

// Builder interface
class CharacterBuilder
{
public:
    virtual Weapon *CreateWeapon() = 0;
    virtual Armor *CreateArmor() = 0;
};

// Director, Product 생성 공정 제공
class CharacterDirector
{
    CharacterBuilder *m_Builder;

public:
    void SetBuilder(CharacterBuilder *newBuilder)
    {
        m_Builder = newBuilder;
    }
    Character *CreateCharacter()
    {
        Character *character = new Character;
        character->m_Weapon = m_Builder->CreateWeapon();
        character->m_Armor = m_Builder->CreateArmor();
        return character;
    }
};

class ArcherBuilder : public CharacterBuilder
{
public:
    Weapon *CreateWeapon()
    {
        Weapon *weapon = new Weapon();
        weapon->m_Power = 50;
        return weapon;
    }
    Armor *CreateArmor()
    {
        Armor *armor = new Armor();
        armor->m_Defense = 200;
        return armor;
    }
};

class WarriorBuilder : public CharacterBuilder
{
public:
    Weapon *CreateWeapon()
    {
        Weapon *weapon = new Weapon();
        weapon->m_Power = 200;
        return weapon;
    }
    Armor *CreateArmor()
    {
        Armor *armor = new Armor();
        armor->m_Defense = 100;
        return armor;
    }
};

int main()
{
    Character *character;

    CharacterDirector director;
    ArcherBuilder archer;
    director.SetBuilder(&archer);
    character = director.CreateCharacter();
    character->DisplayParts();

    WarriorBuilder warrior;
    director.SetBuilder(&warrior);
    character = director.CreateCharacter();
    character->DisplayParts();
}