#include <tuple>

#include "Packet.h"
#include "ErrorCode.h"
#include "User.h"
#include "NetLib/TcpNetwork.h"
#include "UserManager.h"
#include "LobbyManager.h"
#include "Lobby.h"
#include "Game.h"
#include "Room.h"
#include "PacketProcess.h"

using PACKET_ID = NCommon::PACKET_ID;

namespace NLogicLib
{
	NCommon::ERROR_CODE PacketProcess::RoomEnter(PacketInfo packetInfo)
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

		if (reqPkt->IsCreate)
		{
			// ���ο� Room �� ������ �ϴ� ���
		}
		else
		{
			// ���� Room �� ���� �ϴ� ���
		}
	}
}