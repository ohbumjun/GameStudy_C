#include <algorithm>

#include "../../NetLib/ILog.h"
#include "../../NetLib/TcpNetwork.h"
#include "../../Common/Packet.h"
#include "../../Common/ErrorCode.h"
#include "../User/User.h"
#include "../Room/Room.h"
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
	// -> if (m_UserIndexDic.size() >= m_MaxUserCount) ���ǰ� ���� �ǹ̾ƴѰ� ?
	auto addRet = AddUser(pUser);

	if (addRet != ERROR_CODE::NONE) {
		return addRet;
	}

	pUser->EnterLobby(m_LobbyIndex);

	m_UserIndexDic.insert({ pUser->GetIndex(), pUser });
	m_UserIDDic.insert({ pUser->GetID().c_str(), pUser });

	return ERROR_CODE::NONE;
}

ERROR_CODE NLogicLib::Lobby::LeaveUser(const int userIndex)
{
	// �ش� User ������ Lobby ���� �����Ѵ�.
	// ��� �����Ѵٶ�� ���ٴ� �ش� User ������ ��� �ִ� LobbyUser ���
	// �׸��� ����ִ� ���̴�.
	RemoveUser(userIndex);

	// ���� User ������ m_UserIndexDic ���� �����´�.
	auto pUser = FindUser(userIndex);

	if (pUser == nullptr) 
	{
		return ERROR_CODE::LOBBY_LEAVE_USER_NVALID_UNIQUEINDEX;
	}

	// �ٽ� user �� ���¸� LOGIN ���·� �����Ѵ�.
	pUser->LeaveLobby();

	m_UserIndexDic.erase(pUser->GetIndex());
	m_UserIDDic.erase(pUser->GetID().c_str());

	return ERROR_CODE::NONE;
}

short NLogicLib::Lobby::GetUserCount()
{
	return static_cast<short>(m_UserIndexDic.size());
}

NLogicLib::Room* NLogicLib::Lobby::CreateRoom()
{
	// ���ǰ� ���� ���� Room ������ �ϳ� �������ش�.
	for (int i = 0; i < (int)m_RoomList.size(); ++i)
	{
		if (m_RoomList[i]->IsUsed() == false) {
			Room* newRoom = m_RoomList[i];
			return newRoom;
		}
	}
	return nullptr;
}

NLogicLib::Room* NLogicLib::Lobby::GetRoom(const short roomIndex)
{
	if (roomIndex < 0 || roomIndex >= m_RoomList.size()) {
		return nullptr;
	}

	return m_RoomList[roomIndex];
}

void NLogicLib::Lobby::SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserindex)
{
	for (auto& pUser : m_UserIndexDic)
	{
		// passUserindex : �ش� Index �� User �� ������ User ���� ������.
		if (pUser.second->GetIndex() == passUserindex) {
			continue;
		}

		// ���� Domain �� Lobby �� �ƴ϶�� ������ �ʴ´�.
		// ��, Lobby �� ���� ���� ����̶�� ������ �ʴ´�.
		if (pUser.second->IsCurDomainInLobby() == false) {
			continue;
		}

		// �ش� Session �� SendBuffer �� ���� ������ �����ϸ�
		// ���� Frame ���� �ش� ������ ������ �ȴ�.
		m_pRefNetwork->SendData(pUser.second->GetSessioIndex(), packetId, dataSize, pData);
	}
}

void NLogicLib::Lobby::SendRoomListInfo(int sessionIndex)
{
	NCommon::PktRoomListRes resPkt;

	resPkt.ErrorCode = (short)ERROR_CODE::NONE;

	int roomCnt = 0;

	for (size_t index = 0; index < m_RoomList.size(); ++index)
	{
		if (m_RoomList[index]->IsUsed() == false)
		{
			continue;
		}

		roomCnt += 1;
		
		resPkt.RoomList[index].SettInfo(m_RoomList[index]->GetTitle(),
			(short)index,
			m_RoomList[index]->GetUserCount(),
			m_RoomList[index]->MaxUserCount()
		);
	}

	resPkt.RoomCount = roomCnt;

	// ���� �����͸� ���̱� ���� ������� ���� LobbyListInfo ũ��� ���� ������ �ȴ�.
	m_pRefNetwork->SendData(sessionIndex, (short)PACKET_ID::ROOM_LIST_RES, sizeof(resPkt), (char*)&resPkt);
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
	auto findIter = std::find_if(std::begin(m_UserList), std::end(m_UserList), 
		[userIndex](auto& lobbyUser) 
	{ 
		return lobbyUser.pUser != nullptr && lobbyUser.pUser->GetIndex() == userIndex; 
	});

	if (findIter == std::end(m_UserList)) {
		return;
	}

	findIter->pUser = nullptr;
}
