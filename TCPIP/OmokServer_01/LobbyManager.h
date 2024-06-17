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
		// �ִ� Lobby ����
		int MaxLobbyCount;
		// Lobby �� �ִ� User ��
		int MaxLobbyUserCount;
		// Lobby �� �ִ� Room ��
		int MaxRoomCountByLobby;
		// Room �� �ִ� User ��
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

		// Lobby ������ vector ���·� �����Ѵ�.
		// Init �Լ� ���ο��� �ִ� Lobby ������ŭ ������ �صд�.
		std::vector<Lobby> m_LobbyList;

	};
}