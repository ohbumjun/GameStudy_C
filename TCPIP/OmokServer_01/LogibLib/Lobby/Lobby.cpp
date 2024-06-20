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

	// 미리 MaxUser 만큼 일단 LobbyUser 정보를 만든다.
	for (int i = 0; i < maxLobbyUserCount; ++i)
	{
		LobbyUser lobbyUser;
		lobbyUser.Index = (short)i;
		lobbyUser.pUser = nullptr;

		m_UserList.push_back(lobbyUser);
	}

	// 마찬가지로 Room 정보도 미리 마련해둔다.
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
	// 최대 인원수를 넘었는지 확인
	if (m_UserIndexDic.size() >= m_MaxUserCount) {
		return ERROR_CODE::LOBBY_ENTER_MAX_USER_COUNT;
	}

	// 만약 같은 User 가 입장하려고 한다면 중복
	if (FindUser(pUser->GetIndex()) != nullptr) {
		return ERROR_CODE::LOBBY_ENTER_USER_DUPLICATION;
	}

	// 들어갈 수 있는 User 가 존재하지 않는 다는 의미
	// -> if (m_UserIndexDic.size() >= m_MaxUserCount) 조건과 같은 의미아닌가 ?
	auto addRet = AddUser(pUser);
	if (addRet != ERROR_CODE::NONE) {
		return addRet;
	}
	pUser->EnterLobby(m_LobbyIndex);
}

ERROR_CODE NLogicLib::Lobby::LeaveUser(const int userIndex)
{
	RemoveUser(userIndex);

	// 음.. 이렇게 User 와 관련된 정보들을 m_UserindexDic, m_UserIDDic, 여러 형태로 관리하는 게
	// 맞는 것인가 ?
	auto pUser = FindUser(userIndex);

	if (pUser == nullptr) 
	{
		return ERROR_CODE::LOBBY_LEAVE_USER_NVALID_UNIQUEINDEX;
	}

	// 다시 user 의 상태를 LOGIN 상태로 변경한다.
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
	for (int i = 0; i < (int)m_RoomList.size(); ++i)
	{
		if (m_RoomList[i]->IsUsed() == false) {
			return m_RoomList[i];
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
		if (pUser.second->GetIndex() == passUserindex) {
			continue;
		}

		if (pUser.second->IsCurDomainInLobby() == false) {
			continue;
		}

		m_pRefNetwork->SendData(pUser.second->GetSessioIndex(), packetId, dataSize, pData);
	}
}

NLogicLib::User* NLogicLib::Lobby::FindUser(const int userIndex)
{
	// user Index 정보로 User 찾기 
	auto findIter = m_UserIndexDic.find(userIndex);

	if (findIter == m_UserIndexDic.end()) {
		return nullptr;
	}

	return (User*)findIter->second;
}

ERROR_CODE NLogicLib::Lobby::AddUser(User* pUser)
{
	// m_UserList 는 User 가 들어갈 수 있는 일종의 "빈 그릇" 같은 역할
	// m_UserList.pUser 가 nullptr 이 아니라는 것은, 해당 그릇에 이미 User 가 있다는 의미
	// pUser 가 nullptr 인 LobbyUser 를 찾는다. 빈 그릇을 찾아서 그곳에 User 정보를 넣어준다.
	auto findIter = std::find_if(std::begin(m_UserList), std::end(m_UserList), 
		[](auto& lobbyUser) { return lobbyUser.pUser == nullptr; });

	// 빈 그릇이 없다면, 이미 최대 인원수를 넘었다는 의미
	if (findIter == std::end(m_UserList)) {
		return ERROR_CODE::LOBBY_ENTER_EMPTY_USER_LIST;
	}

	findIter->pUser = pUser;
	return ERROR_CODE::NONE;
}

void NLogicLib::Lobby::RemoveUser(const int userIndex)
{
	auto findIter = std::find_if(std::begin(m_UserList), std::end(m_UserList), [userIndex](auto& lobbyUser) { return lobbyUser.pUser != nullptr && lobbyUser.pUser->GetIndex() == userIndex; });

	if (findIter == std::end(m_UserList)) {
		return;
	}

	findIter->pUser = nullptr;
}
