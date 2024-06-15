#pragma once

#include <time.h>
#include <chrono>
#include <vector>
#include "../../NetLib/ILog.h"
#include "../../NetLib/TcpNetwork.h"

namespace NLogicLib
{
	// 일반 user Class 와 Connected user 를 분리하는 이유가 무엇인가 ?
	struct ConnectedUser
	{
		void Clear()
		{
			m_IsLoginSuccess = false;
			m_ConnectedTime = 0;
		}

		bool m_IsLoginSuccess = false;
		time_t m_ConnectedTime = 0;
	};

	class ConnectedUserManager
	{
		using TcpNet = NServerNetLib::ITcpNetwork;
		using ILog = NServerNetLib::ILog;
	public:
		ConnectedUserManager() {};
		virtual ~ConnectedUserManager() {};

		// ex) PacketProcess::Init 
		void Init(const int maxSessionCount, TcpNet* pNetwork, NServerNetLib::ServerConfig* pConfig, ILog* pLogger);
		
	private:
		ILog* m_pRefLogger;
		TcpNet* m_pRefNetwork;

		// 최대 client Session 개수 만큼 일단 Init 함수에서 만들어둔다.
		std::vector<ConnectedUser> ConnectedUserList;

		bool m_IsLoginCheck = false;

		std::chrono::system_clock::time_point m_LatestLoginCheckTime = std::chrono::system_clock::now();
		int m_LatestLogincheckIndex = -1;
	};
}
