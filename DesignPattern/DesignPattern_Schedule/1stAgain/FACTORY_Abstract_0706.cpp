//�߻� ���丮 ����

// ���丮( ��ü�������� Ŭ����)�� �������̽��� �����, �ڽ�Ŭ������ ������ ��������

struct IButton
{
    virtual void Draw() = 0;
};
struct IEdit
{
    virtual void Draw() = 0;
};
//-------------------------------
struct XPButton : public IButton
{
    void Draw() { cout << "XPButton Draw" << endl; }
};
struct XPEdit : public IEdit
{
    void Draw() { cout << "XPEdit Draw" << endl; }
};
//----------------------
struct MACButton : public IButton
{
    void Draw() { cout << "MACButton Draw" << endl; }
};
struct MACEdit : public IEdit
{
    void Draw() { cout << "MACEdit Draw" << endl; }
};
//--------------------------
// ���嵵 �������̽� ���� ������ - �߻� ���丮
struct IFactory
{
    virtual IButton *CreateButton() = 0;
    virtual IEdit *CreateEdit() = 0;
    virtual ~IFactory() {}
};
// �� ��Ÿ���� ��Ʈ���� ����� ������ ������
class XPFactory : public IFactory
{
public:
    virtual IButton *CreateButton() { return new XPButton; }
    virtual IEdit *CreateEdit() { return new XPEdit; }
};
class MACFactory : public IFactory
{
public:
    virtual IButton *CreateButton() { return new MACButton; }
    virtual IEdit *CreateEdit() { return new MACEdit; }
};
int main(int argc, char **argv)
{
    IFactory *p; // ä��������

    if (strcmp(argv[1], "XP") == 0)
        p = new XPFactory;
    else
        p = new MACFactory;
    //--------------------------
    IButton *btn = p->CreateButton();
    btn->Draw();
}