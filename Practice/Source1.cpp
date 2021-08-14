// �긴�� ����
// ��Ȳ :
// mp3 Ŭ������ �ְ�, ������� ����� �ִ�.
// 1�� �̸���⸦ �����ϰ� �ʹ�.

// �����ο�, �������̽��� �и��ؼ�
// ��ȣ�������� update�� ���ϰ� �Ѵ�.

// �������� ��ȭ����, �������̽��� �����ϰ�
// �������̽��� ��ȭ����, �����θ� ��ȭ�Ѵ�

// �ᱹ ������ �߰��� �̿���, �������� ����

#include<iostream>
#include<string>
#include<Windows.h>

using namespace std;

struct IMP3
{
	virtual void Play() = 0;
	virtual void Stop() = 0;
	virtual ~IMP3(){}
};

class IPod : public IMP3
{
public:
	void Play()
	{
		cout << "play MP3" << endl;
	}
	void Stop()
	{
		cout << "stop MP3" << endl;
	}
};

class MP3
{
	IMP3* pImpl;
public :
	MP3(IMP3* p = 0) :pImpl(p)
	{
		if (pImpl == 0) pImpl = new IPod;
	}
	void Play() { pImpl->Play(); }
	void Stop() { pImpl->Stop(); }
	void PlayOneMinute()
	{
		pImpl->Play();
		Sleep(1000);
		pImpl->Stop();
	}
};

class Client
{
public:
	void UseMp3(MP3* p) { p->PlayOneMinute(); }
};

int main()
{
	Client* client = new Client;
	MP3* mp3 = new MP3;
	client->UseMp3(mp3);
}