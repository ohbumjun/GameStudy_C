#pragma once

#include <time.h>
#include <chrono>
#include <vector>
#include "../../NetLib/ILog.h"
#include "../../NetLib/TcpNetwork.h"

namespace NLogicLib
{
	// �Ϲ� user Class �� Connected user �� �и��ϴ� ������ �����ΰ� ?
	struct ConnectedUser
	{
		void Clear()
		{
			m_IsLoginSuccess = false;
			m_ConnectedTime = 0;
		}

		bool m_IsLoginSuccess = false;

		// �ش� User Ȥ�� Client �� connect �� �ð� ?
		// ex) ConnectedUserManager::SetConnectSession
		time_t m_ConnectedTime = 0;
	};

	// ���� UserManager �� �ִµ��� �� Class �� ���� ������ �ֳ� ?
	class ConnectedUserManager
	{
		using TcpNet = NServerNetLib::ITcpNetwork;
		using ILog = NServerNetLib::ILog;
	public:
		ConnectedUserManager() {};
		virtual ~ConnectedUserManager() {};

		// ex) PacketProcess::Init 
		void Init(const int maxSessionCount, TcpNet* pNetwork, NServerNetLib::ServerConfig* pConfig, ILog* pLogger);

		void SetConnectSession(const int sessionIndex);
		void SetLogin(const int sessionIndex);
		void SetDisConnectSession(const int sessionIndex);
		void LoginCheck();

	private:
		ILog* m_pRefLogger;
		TcpNet* m_pRefNetwork;

		// �ִ� client Session ���� ��ŭ �ϴ� Init �Լ����� �����д�.
		std::vector<ConnectedUser> ConnectedUserList;

		bool m_IsLoginCheck = false;

		std::chrono::system_clock::time_point m_LatestLoginCheckTime = std::chrono::system_clock::now();
		int m_LatestLogincheckIndex = -1;
	};
}
