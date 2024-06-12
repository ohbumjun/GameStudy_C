#pragma once



namespace NServerNetLib
{
	// 서버에 관한 설정 정보들
	struct ServerConfig
	{
		unsigned short Port;
		int BackLogCount;

		int MaxClientCount;
		int ExtraClientCount; // 가능하면 로그인에서 짜르도록 MaxClientCount + 여유분을 준비한다.
		
		short MaxClientSockOptRecvBufferSize;
		short MaxClientSockOptSendBufferSize;
		short MaxClientRecvBufferSize;
		short MaxClientSendBufferSize;

		int MaxRoomCount;
		int MaxRoomUserCount;
	};

	const int MAX_IP_LEN = 32; // IP 문자열 최대 길이
	const int MAX_PACKET_BODY_SIZE = 1024; // 최대 패킷 보디 크기
	
	struct ClientSession
	{
		bool IsConnected() { return SocketFD != 0 ? true : false; }

		void Clear()
		{
			Seq = 0;
			SocketFD = 0;
			IP[0] = '\0';
			RemainingDataSize = 0;
			PrevReadPosInRecvBuffer = 0;
			SendSize = 0;
		}

		int Index = 0;

		// 연결된 순서..? TcpNetwork 의 m_ConnectSeq 값이 세팅된다.
		long long Seq = 0;

		// SOCKET 핸들 값
		unsigned long long	SocketFD = 0;
		char    IP[MAX_IP_LEN] = { 0, };

		char*   pRecvBuffer = nullptr; 
		int     RemainingDataSize = 0;
		int     PrevReadPosInRecvBuffer = 0;

		char*   pSendBuffer = nullptr; 
		int     SendSize = 0;
	};

	struct RecvPacketInfo
	{
		//  TcpNetwork 에서 ClientSession 의 index 값이다.
		//  특정 Client 와 통신 과정에서 Packet 정보를 담는 struct ?
		int SessionIndex = 0;		
		short PacketId = 0;
		short PacketBodySize = 0;
		char* pRefData = 0;
	};
	

	enum class SOCKET_CLOSE_CASE : short
	{
		SESSION_POOL_EMPTY = 1,
		SELECT_ERROR = 2,
		SOCKET_RECV_ERROR = 3,
		SOCKET_RECV_BUFFER_PROCESS_ERROR = 4,
		SOCKET_SEND_ERROR = 5,
		FORCING_CLOSE = 6,
	};
	

	enum class PACKET_ID : short
	{
		NTF_SYS_CONNECT_SESSION = 2,
		NTF_SYS_CLOSE_SESSION = 3,
				
	};

#pragma pack(push, 1)
	struct PacketHeader
	{
		short TotalSize;
		short Id;
		unsigned char Reserve;
	};

	const int PACKET_HEADER_SIZE = sizeof(PacketHeader);


	struct PktNtfSysCloseSession : PacketHeader
	{
		int SockFD;
	};
#pragma pack(pop)
}