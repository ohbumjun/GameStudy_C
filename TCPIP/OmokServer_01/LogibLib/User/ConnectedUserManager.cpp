#include "ConnectedUserManager.h"

namespace NLogicLib
{
	void ConnectedUserManager::Init(const int maxSessionCount, TcpNet* pNetwork, 
		NServerNetLib::ServerConfig* pConfig, ILog* pLogger)
	{
		m_pRefLogger = pLogger;
		m_pRefNetwork = pNetwork;
		
		// ex) maxsessioncount : TCPNetwork ������ ������ �ִ� Client ���� ����
		for (int i = 0; i < maxSessionCount; ++i)
		{
			ConnectedUserList.emplace_back(ConnectedUser());
		}

		m_IsLoginCheck = pConfig->IsLoginCheck;
	}
	void ConnectedUserManager::SetConnectSession(const int sessionIndex)
	{
		/*
		>> time(&ConnectedUserList[sessionIndex].m_ConnectedTime);: 
			This line calls the time function from the time.h library. 
			The time function gets the current system time in seconds since the epoch 
			(January 1, 1970, 00:00:00 UTC). 
			The result is stored in the memory location pointed to by the argument.

			��, ConnectedUserList[sessionIndex].m_ConnectedTime �� ���� �ð� ������ �����Ѵ�.
		*/
		time(&ConnectedUserList[sessionIndex].m_ConnectedTime);
	}
	void ConnectedUserManager::SetLogin(const int sessionIndex)
	{
		ConnectedUserList[sessionIndex].m_IsLoginSuccess = true;
	}
	void ConnectedUserManager::SetDisConnectSession(const int sessionIndex)
	{
		ConnectedUserList[sessionIndex].Clear();
	}
	void ConnectedUserManager::LoginCheck()
	{
		if (m_IsLoginCheck == false) {
			return;
		}	
		
		auto curTime = std::chrono::system_clock::now();
		auto diffTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - m_LatestLoginCheckTime);

		// 60�и��� ���� �˻�
		if (diffTime.count() < 60)
		{
			return;
		}
		else
		{
			// 60 mili �� ���� logic check �ð��� �˻��Ѵ�.
			m_LatestLoginCheckTime = curTime;
		}

		auto curSecTime = std::chrono::system_clock::to_time_t(curTime);

		const auto maxSessionCount = (int)ConnectedUserList.size();

		if (m_LatestLogincheckIndex >= maxSessionCount) {
			m_LatestLogincheckIndex = -1;
		}

		// ��...User �ϳ��ϳ� idx �� �������Ѱ��鼭 �˻��Ѵٴ� �ǹ��ΰ� ?
		++m_LatestLogincheckIndex;

		// ? 100 �� �� �� ���ϴ� ���� ?
		// -> LoginCheck �ѹ��� ���� ���� 100�� User ? �� ���ؼ� Login ���θ�
		// ����ؼ� �˻��ϴ� ������ ���δ�.
		auto lastCheckIndex = m_LatestLogincheckIndex + 100;
		if (lastCheckIndex > maxSessionCount) {
			lastCheckIndex = maxSessionCount;
		}

		for (; m_LatestLogincheckIndex < lastCheckIndex; ++m_LatestLogincheckIndex)
		{
			auto i = m_LatestLogincheckIndex;

			if (ConnectedUserList[i].m_ConnectedTime == 0 ||
				ConnectedUserList[i].m_IsLoginSuccess == false)
			{
				// ���ʿ� login �� ���°� �ƴ� User �������ٸ� �ǳʶڴ�.
				continue;
			}

			auto diff = curSecTime - ConnectedUserList[i].m_ConnectedTime;

			if (diff >= 180)
			{
				// ���� �ð� �̻� ���� Login check �� �ȵ� User ��� ������ ������ ���´�.
				m_pRefLogger->Write(NServerNetLib::LOG_TYPE::L_WARN, "%s | Login Wait Time Over. sessionIndex(%d).", __FUNCTION__, i);
				m_pRefNetwork->ForcingClose(i);
			}
		}
	}
}
