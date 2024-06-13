#include <cstring>

#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "User.h"
#include "UserManager.h"
//#include "Room.h"
#include "RoomManager.h"
#include "PacketProcess.h"

using LOG_TYPE = NServerNetLib::LOG_TYPE;
using ServerConfig = NServerNetLib::ServerConfig;

	
PacketProcess::PacketProcess() {}
PacketProcess::~PacketProcess() {}

void PacketProcess::Init(TcpNet* pNetwork, UserManager* pUserMgr, RoomManager* pLobbyMgr, ServerConfig* pConfig, ILog* pLogger)
{
	m_pRefLogger = pLogger;
	m_pRefNetwork = pNetwork;
	m_pRefUserMgr = pUserMgr;
	m_pRefRoomMgr = pLobbyMgr;
}
	
void PacketProcess::Process(PacketInfo packetInfo)
{
	using netLibPacketId = NServerNetLib::PACKET_ID;
	using commonPacketId = PACKET_ID;

	auto packetId = packetInfo.PacketId;
		
	switch (packetId)
	{
	case (int)netLibPacketId::NTF_SYS_CONNECT_SESSION:
		NtfSysConnctSession(packetInfo);
		break;
	case (int)netLibPacketId::NTF_SYS_CLOSE_SESSION:
		NtfSysCloseSession(packetInfo);
		break;
	case (int)commonPacketId::LOGIN_IN_REQ:
		Login(packetInfo);
		break;

	// 새로운 패킷이 만들어지면, 여기 case 문이 점점 늘어날 것이다.
	}
	
}


ERROR_CODE PacketProcess::NtfSysConnctSession(PacketInfo packetInfo)
{
	m_pRefLogger->Write(LOG_TYPE::L_INFO, "%s | NtfSysConnctSession. sessionIndex(%d)", __FUNCTION__, packetInfo.SessionIndex);
	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::NtfSysCloseSession(PacketInfo packetInfo)
{
	auto pUser = std::get<1>(m_pRefUserMgr->GetUser(packetInfo.SessionIndex));

	if (pUser) {		
		m_pRefUserMgr->RemoveUser(packetInfo.SessionIndex);		
	}
			
	m_pRefLogger->Write(LOG_TYPE::L_INFO, "%s | NtfSysCloseSesson. sessionIndex(%d)", __FUNCTION__, packetInfo.SessionIndex);
	return ERROR_CODE::NONE;
}


ERROR_CODE PacketProcess::Login(PacketInfo packetInfo)
{
	// 패스워드는 무조건 pass 해준다.
	// ID 중복이라면 에러 처리한다.
	PktLogInRes resPkt;
	PktLogInReq* reqPkt = (PktLogInReq*)packetInfo.pRefData;

	ERROR_CODE addRet = m_pRefUserMgr->AddUser(packetInfo.SessionIndex, reqPkt->szID);

	if (addRet != ERROR_CODE::NONE) {

		// error 가 발생했다면, error 정보를 전송한다.
		// 이때. resPkt 은 실제 전송하는 패킷의 "데이터" 부분이다.
		resPkt.SetError(addRet);

		// 참고 : 여기서 바로 실제 send 를 호출하지 않는다.
		// 그 다음 network->Run Loop 에서 Send 해준다.
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&resPkt);
		
		return addRet;
	}
		
	resPkt.ErrorCode = (short)addRet;

	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&resPkt);

	return ERROR_CODE::NONE;
}


