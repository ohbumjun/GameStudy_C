#pragma once
#include <memory>

#include "Packet.h"
#include "ErrorCode.h"


namespace NServerNetLib
{
	struct ServerConfig;
	class ILog;
	class ITcpNetwork;
}


class UserManager;
class RoomManager;
class PacketProcess;

class Server
{
public:
	Server();
	~Server();

	ERROR_CODE Init();

	void Run();

	void Stop();


private:
	ERROR_CODE LoadConfig();

	void Release();


private:
	bool m_IsRun = false;

	std::unique_ptr<NServerNetLib::ServerConfig> m_pServerConfig;
	std::unique_ptr<NServerNetLib::ILog> m_pLogger;

	std::unique_ptr<NServerNetLib::ITcpNetwork> m_pNetwork;
	
	std::unique_ptr<PacketProcess> m_pPacketProc;
	
	// UserManager : 로그인된 유저 정보들을 저장한다.
	std::unique_ptr<UserManager> m_pUserMgr;
	
	// RoomManager : 현재 서버 구조는 MMO 구조를 기반으로 하고자 한다.
	// 룸. 에 대한 관리를 하는 Class 이다.
	std::unique_ptr<RoomManager> m_pRoomMgr;
		
};

