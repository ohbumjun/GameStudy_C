#pragma once

#include <vector>
#include <unordered_map>

namespace NServerNetLib
{
	class ITcpNetwork;
}

namespace NServerNetLib
{
	class ILog;
}

namespace NCommon
{
	enum class ERROR_CODE :short;
}

using ERROR_CODE = NCommon::ERROR_CODE;

namespace NLogicLib
{
	using TcpNet = NServerNetLib::ITcpNetwork;
	using ILog = NServerNetLib::ILog;

	class User;
	class Room;

	// ���� LobbyUser ��� ����ü�� ���� ������
	// LobbyUser �� ������ "�׸�"
	// Init ���� Max User ����ŭ �ش� ����ü ������ ����� �ȴ�.
	struct LobbyUser
	{
		short Index = 0;
		User* pUser = nullptr;
	};

	class Lobby
	{
	public:
		Lobby();
		virtual ~Lobby();

		void Init(const short lobbyIndex, 
			const short maxLobbyUserCount, 
			const short maxRoomCountByLobby, 
			const short maxRoomUserCount);

		void Release();

		void SetNetwork(TcpNet* pNetwork, ILog* pLogger);

		short GetIndex() { return m_LobbyIndex; }


		ERROR_CODE EnterUser(User* pUser);
		ERROR_CODE LeaveUser(const int userIndex);

		short GetUserCount();


		Room* CreateRoom();

		Room* GetRoom(const short roomIndex);

		short MaxUserCount() { return (short)m_MaxUserCount; }

		short MaxRoomCount() { return (short)m_RoomList.size(); }

		void SendRoomListInfo(int sessionIndex);

	protected:
		void SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserindex = -1);

		User* FindUser(const int userIndex);

		ERROR_CODE AddUser(User* pUser);

		void RemoveUser(const int userIndex);

	protected :
		ILog* m_pRefLogger;
		TcpNet* m_pRefNetwork;

		// ���� ���� Lobby �� ������ �� �ִ�.
		// ���° Lobby �ΰ�
		short m_LobbyIndex = 0;
		short m_MaxUserCount = 0;

		std::vector<LobbyUser> m_UserList;

		// User Index ~ User Map
		std::unordered_map<int, User*> m_UserIndexDic;

		// ID ~ User Map
		std::unordered_map<const char*, User*> m_UserIDDic;

		// �ϳ��� Lobby �� ���� Room  �� ����ִ� �����ΰ� ?
		std::vector<Room*> m_RoomList;
	};
};