#pragma once

#include "../../Common/PacketID.h"
#include "ErrorCode.h"
#include <string>
#include <vector>

#include "../Includes.h"
#include "../Utils.h"


namespace NCommon
{

#pragma pack(push, 1)
	struct PktHeader
	{
		short TotalSize;
		short Id;
		unsigned char Reserve;
	};

	struct PktBase
	{
		short ErrorCode = (short)ERROR_CODE::NONE;
		void SetError(ERROR_CODE error) { ErrorCode = (short)error; }
	};

	//- 로그인 요청
	const int MAX_USER_ID_SIZE = 16;
	const int MAX_USER_PASSWORD_SIZE = 16;
	struct PktLogInReq
	{
		char szID[MAX_USER_ID_SIZE] = { 0, };
		char szPW[MAX_USER_PASSWORD_SIZE] = { 0, };
	};

	struct PktLogInRes : PktBase
	{
	};

	//- 채널 리스트 요청
	// Request에는 Body가 없음.
	const int MAX_LOBBY_LIST_COUNT = 20;
	struct LobbyListInfo
	{
		short LobbyId;
		short LobbyUserCount;
		short LobbyMaxUserCount;
	};

	// - 로비 정보 ? 관련 response
	struct PktLobbyListRes : PktBase
	{
		short LobbyCount = 0;
		LobbyListInfo LobbyList[MAX_LOBBY_LIST_COUNT];
	};

	// - 로비 입장 요청
	struct PktLobbyEnterReq
	{
		short LobbyId;
	};

	// - 로비 입장 응답
	struct PktLobbyEnterRes : PktBase
	{
		short MaxUserCount;
		short MaxRoomCount;
	};

	// - 로비에서 나가기 요청
	// ex) Room::NotifyLeaveUserInfo
	struct PktLobbyLeaveReq {};

	// - 로비에서 나가기 응답
	struct PktLobbyLeaveRes : PktBase
	{
	};

	// Lobby 내 최대 Room 개수
	const int MAX_ROOM_LIST_COUNT = 20;

	struct RoomListInfo
	{
		short RoomIndex;		// Lobby 내 Room Index
		short RoomUserCount;	// Lobby 내 User Count
		short RoomMaxUserCount;	// Lobby 내 최대 User Count

		RoomListInfo(short pRoomIndex, short pRoomUserCount, short pRoomMaxUserCount)
		{
			RoomIndex = pRoomIndex;
			RoomUserCount = pRoomUserCount;
			RoomMaxUserCount = pRoomMaxUserCount;
		}
	};

	//- 룸 목록 요청
	struct PktRoomListReq
	{
		short CurrentLobbyIndex;
	};

	const int MAX_ROOM_TITLE_SIZE = 16;

	//- 룸 목록 응답
	struct PktRoomListRes : PktBase
	{
		short RoomCount = 0;
		// RoomListInfo RoomList[MAX_ROOM_LIST_COUNT];
		std::vector<RoomListInfo> RoomList;
	};

	// - 룸 생성 요청
	struct PktRoomCreateReq
	{
		short RoomTitleSize;
		wchar_t RoomTitle[MAX_ROOM_TITLE_SIZE + 1];	// 새로 만들 룸의 제목 ?

		void FromBytes(char* data)
		{
			// Extract RoomTitleSize (2 bytes)
			RoomTitleSize = static_cast<short>(*data);
			data += sizeof(short);

			// Extract RoomTitle bytes (RoomTitleSize bytes)
#ifdef _WIN32
			std::string utf8Title(data, data + RoomTitleSize);

			std::wstring wideCharTitle;

			int requiredWideChars = MultiByteToWideChar(CP_ACP, 0,
				utf8Title.c_str(), (int)utf8Title.size(), NULL, 0);

			MultiByteToWideChar(CP_ACP, 0, utf8Title.c_str(),
				(int)utf8Title.size(), &wideCharTitle[0], requiredWideChars);

			wcscpy_s(RoomTitle, MAX_ROOM_TITLE_SIZE + 1, wideCharTitle.c_str()); // Safe string copy
#endif
		}
	};

	struct PktRoomCreateRes : PktBase
	{
		short RoomIndex;	// 만들어진 room 의 index
		short RoomMaxUserCnt;
	};

	//- 룸에 들어가기 요청
	struct PktRoomEnterReq
	{
		short RoomIndex;	// 몇번째 룸에 들어가고 싶은가
	};

	// - 룸에 들어가기 응답
	struct PktRoomEnterRes : PktBase
	{
		short Result;
		short RoomIndex;
		short RoomUserUniqueId;
	};

	//- 룸에 있는 유저에게 새로 들어온 유저 정보 통보
	// ex) Room::NotifyEnterUserInfo
	struct PktRoomEnterUserInfoNtf
	{
		char UserID[MAX_USER_ID_SIZE] = { 0, };
	};

	//- 룸 나가기 요청
	struct PktRoomLeaveReq {};

	struct PktRoomLeaveRes : PktBase
	{
	};

	//- 룸에서 나가는 유저 통보(로비에 있는 유저에게)
	struct PktRoomLeaveUserInfoNtf
	{
		char UserID[MAX_USER_ID_SIZE] = { 0, };
	};

	//- 룸 채팅 요청
	const int MAX_ROOM_CHAT_MSG_SIZE = 256;

	struct PktRoomChatReq
	{
		wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE + 1] = { 0, };
	};

	// - 룸 채팅 응답
	struct PktRoomChatRes : PktBase
	{
	};

	//- 룸 채팅 내용 ?
	// ex) Room::NotifyChat
	struct PktRoomChatNtf
	{
		char UserID[MAX_USER_ID_SIZE] = { 0, };
		wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
	};

	// 방장의 게임 시작 요청
	struct PktRoomMaterGameStartReq
	{};

	struct PktRoomMaterGameStartRes : PktBase
	{};

	// 게임 시작 메세지 및 통보
	struct PktRoomMaterGameStartNtf
	{};

	// 방장이 아닌 사람의 게임 시작 요청
	struct PktRoomGameStartReq
	{};

	struct PktRoomGameStartRes : PktBase
	{};

	// 방장 아닌 사람의 게임 시작 메세지 및 통보
	struct PktRoomGameStartNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};

	// ?
	const int DEV_ECHO_DATA_MAX_SIZE = 1024;

	struct PktDevEchoReq
	{
		short DataSize;
		char Datas[DEV_ECHO_DATA_MAX_SIZE];
	};

	struct PktDevEchoRes : PktBase
	{
		short DataSize;
		char Datas[DEV_ECHO_DATA_MAX_SIZE];
	};

#pragma pack(pop)




}
