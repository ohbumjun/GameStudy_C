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
	using commonPacketId = NCommon::PACKET_ID;

	auto packetId = packetInfo.PacketId;
		
	switch (packetId)
	{
		/*
		- 클라이언트가 자기가 연결되었다고 패킷을 보내지는 않는다.
		  클라이언트 측에서 connect 함수를 호출할 뿐이다.
		- TcpNetwork::NewSession() 측에서 새로운 Session 을 만들어낼 때
		  만들어내는 Packet 이다.
		  음..서버 측에서 accept 를 한 이후에, 새롭게 클라이언트 연결 소켓을 만들어낼 때
		  만들어주는 Packet 정보로 보인다.
		*/
	case (int)netLibPacketId::NTF_SYS_CONNECT_SESSION:
		NtfSysConnctSession(packetInfo);
		break;
	case (int)netLibPacketId::NTF_SYS_CLOSE_SESSION:
		NtfSysCloseSession(packetInfo);
		break;
		/*
		자. 여기 아래 부분부터는 실제 클라이언트가 보낸 패킷을 처리하는 부분이다.
		*/
	case (int)commonPacketId::LOGIN_IN_REQ:
		Login(packetInfo);
		break;

	    // 새로운 패킷이 만들어지면, 여기 case 문이 점점 늘어날 것이다.
		// 방 입장
		// - client 가 방 번호를 패킷으로 보내야 한다. 그 방 번호를 통해서 클라이언트를 해당 방에 넣어준다.
		// - 만일 방 번호를 -1 로 보냈다면, 빈 방으로 넣어달라는 의미로 받아들어야 한다.
		// 방 out
		// 게임 시작
		// 게임 종료 
	}
	
}


NCommon::ERROR_CODE PacketProcess::NtfSysConnctSession(PacketInfo packetInfo)
{
	m_pRefLogger->Write(LOG_TYPE::L_INFO, "%s | NtfSysConnctSession. sessionIndex(%d)", __FUNCTION__, packetInfo.SessionIndex);
	return NCommon::ERROR_CODE::NONE;
}

NCommon::ERROR_CODE PacketProcess::NtfSysCloseSession(PacketInfo packetInfo)
{
	auto pUser = std::get<1>(m_pRefUserMgr->GetUser(packetInfo.SessionIndex));

	if (pUser) {		
		m_pRefUserMgr->RemoveUser(packetInfo.SessionIndex);		
	}
			
	m_pRefLogger->Write(LOG_TYPE::L_INFO, "%s | NtfSysCloseSesson. sessionIndex(%d)", __FUNCTION__, packetInfo.SessionIndex);
	return NCommon::ERROR_CODE::NONE;
}


NCommon::ERROR_CODE PacketProcess::Login(PacketInfo packetInfo)
{
	// 패스워드는 무조건 pass 해준다.
	// ID 중복이라면 에러 처리한다.
	NCommon::PktLogInRes resPkt;
	NCommon::PktLogInReq* reqPkt = (NCommon::PktLogInReq*)packetInfo.pRefData;

	// DB 를 연동할 경우, 여기에 Login 관련 검증 처리를 할 수 있을 것이다.
	NCommon::ERROR_CODE addRet = m_pRefUserMgr->AddUser(packetInfo.SessionIndex, reqPkt->szID);

	if (addRet != NCommon::ERROR_CODE::NONE) {

		// error 가 발생했다면, error 정보를 전송한다.
		// 이때. resPkt 은 실제 전송하는 패킷의 "데이터" 부분이다.
		resPkt.SetError(addRet);

		// 참고 : 여기서 바로 실제 send 를 호출하지 않는다.
		// 그 다음 network->Run Loop 에서 Send 해준다.
		// 즉, 네트워크 상의 클라이언트에게 패킷을 보내게 된다.
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)NCommon::PACKET_ID::LOGIN_IN_RES, sizeof(NCommon::PktLogInRes), (char*)&resPkt);
		
		return addRet;
	}
		
	resPkt.ErrorCode = (short)addRet;

	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)NCommon::PACKET_ID::LOGIN_IN_RES, sizeof(NCommon::PktLogInRes), (char*)&resPkt);

	return NCommon::ERROR_CODE::NONE;
}


