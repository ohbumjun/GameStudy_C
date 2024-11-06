#include <tuple>

#include "../../Common/Packet.h"
#include "../../Common/ErrorCode.h"
#include "../User/User.h"
#include "../../NetLib/TcpNetwork.h"
#include "../User/UserManager.h"
#include "../../LobbyManager.h"
#include "../Lobby/Lobby.h"
#include "../Game.h"
#include "../Room/Room.h"
#include "PacketProcess.h"

using PACKET_ID = NCommon::PACKET_ID;

namespace NLogicLib
{

	ERROR_CODE PacketProcess::RoomCreate(PacketInfo packetInfo)
	{
		NCommon::PktRoomCreateReq* reqPkt = (NCommon::PktRoomCreateReq*)packetInfo.pRefData;

		reqPkt->FromBytes(packetInfo.pRefData);

		// �׿� ���� response packet ����
		NCommon::PktRoomCreateRes resPkt;

		// �ش� Client Session �� �����Ǵ� User ������ ���´�.
		const std::tuple<NCommon::ERROR_CODE, User*> userTuple = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		NCommon::ERROR_CODE errorCode = std::get<0>(userTuple);
		User* pUser = std::get<1>(userTuple);

		if (errorCode != NCommon::ERROR_CODE::NONE)
		{
			// 1) �ش� session Index �� �����Ǵ� User �� ���ų�
			// 2) User �� Confirm ���� ���� ���
			resPkt.SetError(errorCode);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CREATE_RES, sizeof(resPkt), (char*)&resPkt);
			return errorCode;
		}

		// �ش� Packet �� �����Ǵ� user �� ã�Ҵµ� Lobby �� ���� ������� ���� ���
		if (pUser->IsCurDomainInLobby() == false)
		{
			// User �� ���� Lobby �� ���� ���� ���
			resPkt.SetError(ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CREATE_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_ENTER_INVALID_LOBBY_INDEX;
		}

		// ��. ���� User �� �� Room �� �������ָ� �ȴ�.
		Room* pRoom = nullptr;

		// User �� ���� Lobby �� �����´�.
		short userLobbyIndex = pUser->GetLobbyIndex();

		Lobby* userLobby = m_pRefLobbyMgr->GetLobby(userLobbyIndex);

		// ���ο� Room �� ������ �ϴ� ���
		pRoom = userLobby->CreateRoom();

		pRoom->CreateRoom(reqPkt->RoomTitle);

		resPkt.RoomIndex = pRoom->GetIndex();
		resPkt.RoomMaxUserCnt = pRoom->MaxUserCount();

		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CREATE_RES, sizeof(NCommon::PktRoomCreateRes), (char*)&resPkt);

		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomEnter(PacketInfo packetInfo)
	{
		// Ŭ���̾�Ʈ�κ��� �� Packet �� RoomEnterReq �̴�.
		NCommon::PktRoomEnterReq* reqPkt = (NCommon::PktRoomEnterReq*)packetInfo.pRefData;

		// �׿� ���� response packet ����
		NCommon::PktRoomEnterRes resPkt;

		// �ش� Client Session �� �����Ǵ� User ������ ���´�.
		const std::tuple<NCommon::ERROR_CODE, User*> userTuple = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		NCommon::ERROR_CODE errorCode = std::get<0>(userTuple);
		User* pUser = std::get<1>(userTuple);

		if (errorCode != NCommon::ERROR_CODE::NONE)
		{
			// 1) �ش� session Index �� �����Ǵ� User �� ���ų�
			// 2) User �� Confirm ���� ���� ���
			resPkt.SetError(errorCode);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(resPkt), (char*)&resPkt);
			return errorCode;
		}

		// �ش� Packet �� �����Ǵ� user �� ã�Ҵµ� Lobby �� ���� ������� ���� ���
		if (pUser->IsCurDomainInLobby() == false)
		{
			// User �� ���� Lobby �� ���� ���� ���
			resPkt.SetError(ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_ENTER_INVALID_LOBBY_INDEX;
		}

		// ��. ���� User �� �� Room �� �������ָ� �ȴ�.
		Room* pRoom = nullptr;

		// User �� ���� Lobby �� �����´�.
		short userLobbyIndex = pUser->GetLobbyIndex();

		Lobby* userLobby = m_pRefLobbyMgr->GetLobby(userLobbyIndex);

		// ���� Room �� ���� �ϴ� ���
		pRoom = userLobby->GetRoom(reqPkt->RoomIndex);

		if (pRoom == nullptr)
		{
			// ������ �濡 ���� ������ ���� ���
			resPkt.SetError(ERROR_CODE::ROOM_CREATE_NO_ROOM);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CREATE_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_CREATE_NO_ROOM;
		}


		pRoom->EnterUser(pUser);

		pUser->EnterRoom(userLobbyIndex, pRoom->GetIndex());

		resPkt.Result = (short)ERROR_CODE::NONE;
		resPkt.RoomIndex = pRoom->GetIndex();

		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(NCommon::PktRoomEnterRes), (char*)&resPkt);

		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomLeave(PacketInfo packetInfo)
	{
		NCommon::PktRoomLeaveRes resPkt;

		// ���� �������� �ϴ� Client Session �� User ������ �����´�.
		auto [errorCode, pUser] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		if (errorCode != ERROR_CODE::NONE) 
		{
			// 1)  �ش� Session �� �����Ǵ� User �� ���ų�
			// 2) User �� Confirm ���� ���� ���
			resPkt.SetError(errorCode);

			m_pRefNetwork->SendData(packetInfo.SessionIndex, 
				(short)PACKET_ID::ROOM_LEAVE_RES, sizeof(resPkt), (char*)&resPkt);

			return errorCode;
		}

		// UserManager �� m_UserObjPoolIndex
		auto userIndex = pUser->GetIndex();

		if (pUser->IsCurDomainInRoom() == false) 
		{
			// User �� ���� Room �� �������� �ʴ� ���
			resPkt.SetError(ERROR_CODE::ROOM_LEAVE_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_LEAVE_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_LEAVE_INVALID_DOMAIN;
		}

		// User �� ���� Lobby Idx ������ �����´�.
		// -> Room ���� ���� ����, Lobby �� �ǵ��ư��� �ϱ� �������� ���δ�.
		auto lobbyIndex = pUser->GetLobbyIndex();

		Lobby* pLobby = m_pRefLobbyMgr->GetLobby(lobbyIndex);

		if (pLobby == nullptr) 
		{
			// User �� ���� Lobby �� �������� �ʴ� ���
			resPkt.SetError(ERROR_CODE::ROOM_ENTER_INVALID_LOBBY_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_LEAVE_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_ENTER_INVALID_LOBBY_INDEX;
		}

		// User �� ���� Room ������ �����´�.
		Room* pRoom = pLobby->GetRoom(pUser->GetRoomIndex());

		if (pRoom == nullptr) 
		{
			resPkt.SetError(ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_LEAVE_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
		}

		// �ش� Room ���� User �� ���ش�.
		auto leaveRet = pRoom->LeaveUser(userIndex);
		if (leaveRet != ERROR_CODE::NONE) 
		{
			// 1) �ش� Room �� User �� �������� �ʴ� ���
			// 2) �ش� Room �� ��������� ���� ���¿��ٸ�
			resPkt.SetError(leaveRet);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_LEAVE_RES, sizeof(resPkt), (char*)&resPkt);
			return leaveRet;
		}

		// ���� ������ �κ�� ����
		pUser->EnterLobby(lobbyIndex);

		// �뿡 ������ �������� �뺸
		pRoom->NotifyLeaveUserInfo(pUser->GetID().c_str());

		// User Client ���� RoomLeaveRes ��Ŷ�� ������.
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_LEAVE_RES, sizeof(resPkt), (char*)&resPkt);
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomChat(PacketInfo packetInfo)
	{
		NCommon::PktRoomChatReq* reqPkt = (NCommon::PktRoomChatReq*)packetInfo.pRefData;
		NCommon::PktRoomChatRes resPkt;

		auto [errorCode, pUser] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		if (errorCode != ERROR_CODE::NONE) 
		{
			// 1) �ش� Session �� �����Ǵ� User �� ���ų�
			// 2) User �� Confirm ���� ���� ��� (�α��� ���� ���� �����ΰ�.?)
			resPkt.SetError(errorCode);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CHAT_RES, sizeof(resPkt), (char*)&resPkt);
			return errorCode;
		}

		// ���� Room �� �����ִ� User �� �ƴ� ���
		if (pUser->IsCurDomainInRoom() == false) {
			resPkt.SetError(ERROR_CODE::ROOM_CHAT_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CHAT_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_CHAT_INVALID_DOMAIN;
		}

		auto lobbyIndex = pUser->GetLobbyIndex();
		auto pLobby = m_pRefLobbyMgr->GetLobby(lobbyIndex);
		if (pLobby == nullptr) {
			// ���� �ش� User �� ���� Lobby �� ���� ���
			resPkt.SetError(ERROR_CODE::ROOM_CHAT_INVALID_LOBBY_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CHAT_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_CHAT_INVALID_LOBBY_INDEX;
		}

		auto pRoom = pLobby->GetRoom(pUser->GetRoomIndex());

		if (pRoom == nullptr) {
			// ���� �ش� User �� ���� Room �� ���� ���
			resPkt.SetError(ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CHAT_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
		}

		// ���� User �� ������ ������ User ���� ä�� �޽����� �����Ѵ�.
		pRoom->NotifyChat(pUser->GetSessioIndex(), pUser->GetID().c_str(), reqPkt->Msg);

		// ���� User �� �����Ǵ� Client ���� ä�� Packet �� ������.
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_CHAT_RES, sizeof(resPkt), (char*)&resPkt);
		
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomMasterGameStart(PacketInfo packetInfo)
	{
		NCommon::PktRoomMaterGameStartRes resPkt;

		auto [errorCode, pUser] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		if (errorCode != ERROR_CODE::NONE) {
			// 1) �ش� Session �� �����Ǵ� User �� ���ų�
			// 2) User �� Confirm ���� ���� ���
			resPkt.SetError(errorCode);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_MASTER_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return errorCode;
		}

		if (pUser->IsCurDomainInRoom() == false) {
			// User �� ���� Room �� �������� �ʴ� ���
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_MASTER_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_DOMAIN;
		}

		auto lobbyIndex = pUser->GetLobbyIndex();
		auto pLobby = m_pRefLobbyMgr->GetLobby(lobbyIndex);
		if (pLobby == nullptr) {
			// User �� ���� Lobby �� �������� �ʴ� ���
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_LOBBY_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_MASTER_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_LOBBY_INDEX;
		}

		auto pRoom = pLobby->GetRoom(pUser->GetRoomIndex());
		if (pRoom == nullptr) {
			// User �� ���� Room �� �������� �ʴ� ���
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_ROOM_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_MASTER_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_ROOM_INDEX;
		}

		// ������ �´��� Ȯ��
		if (pRoom->IsMaster(pUser->GetIndex()) == false) {
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_MASTER);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_MASTER_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_MASTER;
		}

		// ���� �ο��� 2���ΰ�?
		// ���� : �� ������ �����ؾ� �� ���� �ִ�. 2�� �̻��� �ƴ� ����� ���� X, 2�� �̻��̸� ���� ����
		if (pRoom->GetUserCount() != 2) {
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_USER_COUNT);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_MASTER_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_USER_COUNT;
		}

		// ���� ���°� ������ ���ϴ� ������?
		if (pRoom->GetGameObj()->GetState() != GameState::NONE) {
			// STARTTING, ING, END �� �ϳ��� ���
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_GAME_STATE);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_MASTER_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_GAME_STATE;
		}


		// ���� ���� ���� ����
		pRoom->GetGameObj()->SetState(GameState::STARTTING);

		// ���� �ٸ� �������� ������ ���� ���� ��û�� ������ �˸���
		pRoom->SendToAllUser((short)PACKET_ID::ROOM_MASTER_GAME_START_NTF,
			0,			// data size �� 0
			nullptr,	// �ش� Packet �� ���ٸ� Data �� ����. ���� Packet ID ������ �Ǻ�
			pUser->GetIndex());

		// Master Game Start ��û Client���� �亯�� ������.
		m_pRefNetwork->SendData(
			packetInfo.SessionIndex, 
			(short)PACKET_ID::ROOM_MASTER_GAME_START_RES, 
			sizeof(resPkt), 
			(char*)&resPkt
		);
		
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomGameStart(PacketInfo packetInfo)
	{
		NCommon::PktRoomGameStartRes resPkt;

		auto [errorCode, pUser] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		if (errorCode != ERROR_CODE::NONE) {
			resPkt.SetError(errorCode);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return errorCode;
		}

		if (pUser->IsCurDomainInRoom() == false) {
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_DOMAIN;
		}

		auto lobbyIndex = pUser->GetLobbyIndex();
		auto pLobby = m_pRefLobbyMgr->GetLobby(lobbyIndex);
		if (pLobby == nullptr) {
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_LOBBY_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_LOBBY_INDEX;
		}

		auto pRoom = pLobby->GetRoom(pUser->GetRoomIndex());
		if (pRoom == nullptr) {
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_ROOM_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_ROOM_INDEX;
		}

		// ���� ���°� ������ ���ϴ� ������?
		if (pRoom->GetGameObj()->GetState() != GameState::STARTTING) {
			resPkt.SetError(ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_GAME_STATE);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_MASTER_GAME_START_INVALID_GAME_STATE;
		}


		//TODO: �̹� ���� ���� ��û�� �ߴ°�?

		//TODO: �濡�� ���� ���� ��û�� ���� ����Ʈ�� ���

		// ���� �ٸ� �������� ���� ���� ��û�� ������ �˸���

		// ��û�ڿ��� �亯�� ������.
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_GAME_START_RES, sizeof(resPkt), (char*)&resPkt);


		// ���� ���� �����Ѱ�?
		// �����̸� ���� ���� ���� GameState::ING
		// ���� ���� ��Ŷ ������
		// ���� ���� ���� �κ� �˸���
		// ������ ���� ���� �ð� ����
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomList(PacketInfo packetInfo)
	{
		NCommon::PktRoomListReq* reqPkt = (NCommon::PktRoomListReq*)packetInfo.pRefData;

		NCommon::PktRoomListRes resPkt;

		// �ش� Client Session �� �����Ǵ� User ������ ���´�.
		const std::tuple<NCommon::ERROR_CODE, User*> userTuple = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		NCommon::ERROR_CODE errorCode = std::get<0>(userTuple);
		User* pUser = std::get<1>(userTuple);

		if (errorCode != NCommon::ERROR_CODE::NONE)
		{
			// 1) �ش� session Index �� �����Ǵ� User �� ���ų�
			// 2) User �� Confirm ���� ���� ���
			resPkt.SetError(errorCode);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(resPkt), (char*)&resPkt);
			return errorCode;
		}

		// �ش� Packet �� �����Ǵ� user �� ã�Ҵµ� Lobby �� ���� ������� ���� ���
		if (pUser->IsCurDomainInLobby() == false)
		{
			// User �� ���� Lobby �� ���� ���� ���
			resPkt.SetError(ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_ENTER_INVALID_LOBBY_INDEX;
		}

		// User �� ���� Lobby �� ã�´�.
		Lobby* pLobby = m_pRefLobbyMgr->GetLobby(pUser->GetLobbyIndex());

		if (pLobby == nullptr) {
			resPkt.SetError(ERROR_CODE::LOBBY_LEAVE_INVALID_LOBBY_INDEX);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOBBY_LEAVE_RES, sizeof(NCommon::PktLobbyLeaveRes), (char*)&resPkt);
			return ERROR_CODE::LOBBY_LEAVE_INVALID_LOBBY_INDEX;
		}

		// �ش� Lobby �� Room List ������ �����ش�.
		pLobby->SendRoomListInfo(packetInfo.SessionIndex);

		return ERROR_CODE::NONE;
	}
}