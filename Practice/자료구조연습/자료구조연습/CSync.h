#pragma once

#include <process.h>
#include <windows.h>

class CSync {
public :
	CSync(CRITICAL_SECTION* Crt) : m_Crt(Crt)
{
		EnterCriticalSection(m_Crt);
}
	~CSync()
{
		LeaveCriticalSection(m_Crt);
}
private :
	CRITICAL_SECTION *m_Crt;
};