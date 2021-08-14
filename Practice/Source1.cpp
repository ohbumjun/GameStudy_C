// 브릿지 패턴
// 상황 :
// mp3 클래스가 있고, 재생멈춤 기능이 있다.
// 1분 미리듣기를 제공하고 싶다.

// 구현부와, 인터페이스를 분리해서
// 상호독립적인 update를 편리하게 한다.

// 구현분의 변화없이, 인터페이스를 변경하고
// 인터페이스의 변화없이, 구현부를 변화한다

// 결국 계층의 추가를 이용한, 유연성의 증대

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