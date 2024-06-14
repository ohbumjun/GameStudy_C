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
		// 클라이언트로부터 온 Packet 은 RoomEnterReq 이다.
		NCommon::PktRoomEnterReq* reqPkt = (NCommon::PktRoomEnterReq*)packetInfo.pRefData;

		// 그에 대한 response packet 변수
		NCommon::PktRoomEnterRes resPkt;

		// 해당 Client Session 에 대응되는 User 정보를 얻어온다.
		const std::tuple<NCommon::ERROR_CODE, User*> userTuple = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		NCommon::ERROR_CODE errorCode = std::get<0>(userTuple);
		User* pUser = std::get<1>(userTuple);

		if (errorCode != NCommon::ERROR_CODE::NONE)
		{
			// 1) 해당 session Index 에 대응되는 User 가 없거나
			// 2) User 가 Confirm 이지 않은 경우
			resPkt.SetError(errorCode);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(resPkt), (char*)&resPkt);
			return errorCode;
		}

		// 해당 Packet 에 대응되는 user 를 찾았는데 Lobby 에 아직 들어있지 않은 경우
		if (pUser->IsCurDomainInLobby() == false)
		{
			// User 가 현재 Lobby 에 있지 않은 경우
			resPkt.SetError(ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::ROOM_ENTER_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::ROOM_ENTER_INVALID_LOBBY_INDEX;
		}

		// 자. 이제 User 가 들어갈 Room 을 배정해주면 된다.
		Room* pRoom = nullptr;

		if (reqPkt->IsCreate)
		{
			// 새로운 Room 을 만들어야 하는 경우
		}
		else
		{
			// 기존 Room 에 들어가야 하는 경우
		}
	}
}