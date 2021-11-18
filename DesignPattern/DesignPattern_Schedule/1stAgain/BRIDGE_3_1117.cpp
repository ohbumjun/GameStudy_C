// 브릿지 패턴
// 상황 :
// mp3 클래스가 있고, 재생 멈춤 기능이 있다
// 1분 미리듣기를 제공하고 싶다

// 구현부와, 인터페이스(추상부,사용자가 사용하는 클래스)를
// 분리해서
// 상호 독립적인 Update(변경,확장,추가)를 편리하게 한다.
// 구현부의 변화없이, 인터페이스를 변경하고
// 인터페이스의 변화없이 구현부를 변경할 수 있다
// 결국 계층의 추가를 이용한 유연성의 증대

#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

// Implementation
struct IMP3
{
    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual ~IMP3() {}
}
// Concrete Implementor
class IPod : public IMP3
{
public:
    void Play() { cout << "Play MP3 with IPod" << endl; }
    void Stop() { cout << "Stop MP3 with IPod" << endl; }
}

// 사용자가 구현부(IMP3)를 직접 사용하게 하지 말고
// 중간층(Bridge)를 도입하면, 변화에 쉽게 대응할 수 있다

// Abstraction
class MP3
{
    IMP3 *pImpl;

public:
    MP3(IMP3 *p = 0) : pImpl(p)
    {
        if (pImpl == 0)
            pImpl = new IPod;
    }
    void Play() { pImpl->Play(); } // 결국은 구현 클래스 사용
    void Stop() { pImpl->Stop(); }
    void PlayOneMinute()
    {
        pImpl->Play();
        Sleep(1000); // 1분 뒤에
        pImpl->Stop();
    }
}

// Client
class People
{
public:
    // 1분만 재생하고
    // 멈추게 하고 싶다
    void UseMP3(MP3 *p) { p->PlayOneMinute(); }
}

int
main()
{
    People *pp = new People;
    MP3 *mp3 = new MP3;
    pp->UseMP3(mp3);
}