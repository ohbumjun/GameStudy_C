#include <algorithm>

#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "Packet.h"
#include "ErrorCode.h"
#include "User.h"
#include "Room.h"
#include "Lobby.h"

using PACKET_ID = NCommon::PACKET_ID;

NLogicLib::Lobby::Lobby()
{
}

NLogicLib::Lobby::~Lobby()
{
}

void NLogicLib::Lobby::Init(const short lobbyIndex, const short maxLobbyUserCount, const short maxRoomCountByLobby, const short maxRoomUserCount)
{
	m_LobbyIndex = lobbyIndex;
	m_MaxUserCount = (short)maxLobbyUserCount;

	// �̸� MaxUser ��ŭ �ϴ� LobbyUser ������ �����.
	for (int i = 0; i < maxLobbyUserCount; ++i)
	{
		LobbyUser lobbyUser;
		lobbyUser.Index = (short)i;
		lobbyUser.pUser = nullptr;

		m_UserList.push_back(lobbyUser);
	}

	// ���������� Room ������ �̸� �����صд�.
	for (int i = 0; i < maxRoomCountByLobby; ++i)
	{
		m_RoomList.emplace_back(new Room());
		m_RoomList[i]->Init((short)i, maxRoomUserCount);
	}
}

void NLogicLib::Lobby::Release()
{
	for (int i = 0; i < (int)m_RoomList.size(); ++i)
	{
		delete m_RoomList[i];
	}

	m_RoomList.clear();
}

void NLogicLib::Lobby::SetNetwork(TcpNet* pNetwork, ILog* pLogger)
{
	m_pRefLogger = pLogger;
	m_pRefNetwork = pNetwork;

	for (auto pRoom : m_RoomList)
	{
		pRoom->SetNetwork(pNetwork, pLogger);
	}
}

ERROR_CODE NLogicLib::Lobby::EnterUser(User* pUser)
{
	// �ִ� �ο����� �Ѿ����� Ȯ��
	if (m_UserIndexDic.size() >= m_MaxUserCount) {
		return ERROR_CODE::LOBBY_ENTER_MAX_USER_COUNT;
	}

	// ���� ���� User �� �����Ϸ��� �Ѵٸ� �ߺ�
	if (FindUser(pUser->GetIndex()) != nullptr) {
		return ERROR_CODE::LOBBY_ENTER_USER_DUPLICATION;
	}

	// �� �� �ִ� User �� �������� �ʴ� �ٴ� �ǹ�
	auto addRet = AddUser(pUser);
	if (addRet != ERROR_CODE::NONE) {
		return addRet;
	}
	pUser->EnterLobby(m_LobbyIndex);
}

ERROR_CODE NLogicLib::Lobby::LeaveUser(const int userIndex)
{
	return ERROR_CODE();
}

short NLogicLib::Lobby::GetUserCount()
{
	return 0;
}

NLogicLib::Room* NLogicLib::Lobby::CreateRoom()
{
	return nullptr;
}

NLogicLib::Room* NLogicLib::Lobby::GetRoom(const short roomIndex)
{
	return nullptr;
}

void NLogicLib::Lobby::SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserindex)
{
}

NLogicLib::User* NLogicLib::Lobby::FindUser(const int userIndex)
{
	// user Index ������ User ã�� 
	auto findIter = m_UserIndexDic.find(userIndex);

	if (findIter == m_UserIndexDic.end()) {
		return nullptr;
	}

	return (User*)findIter->second;
}

ERROR_CODE NLogicLib::Lobby::AddUser(User* pUser)
{
	// m_UserList �� User �� �� �� �ִ� ������ "�� �׸�" ���� ����
	// m_UserList.pUser �� nullptr �� �ƴ϶�� ����, �ش� �׸��� �̹� User �� �ִٴ� �ǹ�
	// pUser �� nullptr �� LobbyUser �� ã�´�. �� �׸��� ã�Ƽ� �װ��� User ������ �־��ش�.
	auto findIter = std::find_if(std::begin(m_UserList), std::end(m_UserList), 
		[](auto& lobbyUser) { return lobbyUser.pUser == nullptr; });

	// �� �׸��� ���ٸ�, �̹� �ִ� �ο����� �Ѿ��ٴ� �ǹ�
	if (findIter == std::end(m_UserList)) {
		return ERROR_CODE::LOBBY_ENTER_EMPTY_USER_LIST;
	}

	findIter->pUser = pUser;
	return ERROR_CODE::NONE;
}

void NLogicLib::Lobby::RemoveUser(const int userIndex)
{
}
