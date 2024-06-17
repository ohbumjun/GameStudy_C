#pragma once
#include <vector>
#include <unordered_map>

namespace NServerNetLib
{
	class TcpNetwork;
}

namespace NServerNetLib
{
	class ILog;
}

namespace NLogicLib
{
	struct LobbyManagerConfig
	{
		// 최대 Lobby 개수
		int MaxLobbyCount;
		// Lobby 내 최대 User 수
		int MaxLobbyUserCount;
		// Lobby 내 최대 Room 수
		int MaxRoomCountByLobby;
		// Room 내 최대 User 수
		int MaxRoomUserCount;
	};

	struct LobbySmallInfo
	{
		short Num;
		short UserCount;
	};

	class Lobby;

	class LobbyManager
	{
		using TcpNet = NServerNetLib::ITcpNetwork;
		using ILog = NServerNetLib::ILog;

	public:
		LobbyManager();
		virtual ~LobbyManager();

		void Init(const LobbyManagerConfig& config, TcpNet* pNetwork, ILog* pLogger);

		Lobby* GetLobby(short lobbyId);


	public:
		void SendLobbyListInfo(const int sessionIndex);


	private:
		ILog* m_pRefLogger;
		TcpNet* m_pRefNetwork;

		// Lobby 정보를 vector 형태로 관리한다.
		// Init 함수 내부에서 최대 Lobby 개수만큼 생성을 해둔다.
		std::vector<Lobby> m_LobbyList;

	};
}