#pragma once
#include <windows.h>
#include <process.h>

class CThread {
public :
	CThread();
	~CThread();
protected:
	HANDLE m_Event;
	HANDLE m_Thread;
public :
	virtual void Run();
	virtual bool Init();
protected:
	void Suspend();
	void Resume();
	void Start();
	void WaitEvent();
public :
	static unsigned int __stdcall  ThreadFunction(void* Arg);
};

inline CThread::CThread() :
m_Event(0),
m_Thread(0)
{}

inline CThread::~CThread()
{
	if (m_Event)
	{
		CloseHandle(m_Event);
		m_Event = 0;
	}
}

inline void CThread::Run()
{}

inline bool CThread::Init()
{
	m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_Thread = (HANDLE)_beginthreadex(NULL, 0, 
		CThread::ThreadFunction, this, 0, NULL);
}

inline void CThread::Suspend()
{}

inline void CThread::Resume()
{}

inline void CThread::Start()
{
	SetEvent(m_Event);
}

inline void CThread::WaitEvent()
{
	WaitForSingleObject(m_Event, INFINITE);
}

inline unsigned int CThread::ThreadFunction(void* Arg)
{
	CThread* Thread = (CThread*)Arg;
	Thread->WaitEvent();
	Thread->Run();
}
