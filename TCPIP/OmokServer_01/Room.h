﻿#pragma once

#include <vector>
#include <string>
#include <memory>

#include "User.h"


namespace NServerNetLib { class ITcpNetwork; }
namespace NServerNetLib { class ILog; }


	
using TcpNet = NServerNetLib::ITcpNetwork;
using ILog = NServerNetLib::ILog;
class Game;

namespace NLogicLib
{
	class Room
	{
	public:
		Room();
		virtual ~Room();

		void Init(const short index, const short maxUserCount);

		void SetNetwork(TcpNet* pNetwork, ILog* pLogger);

		// User 들이 모두 나가면 Room 정리
		void Clear();

		short GetIndex() { return m_Index; }

		// user 가 한명이라도 있다면 사용중
		bool IsUsed() { return m_IsUsed; }

		short MaxUserCount() { return m_MaxUserCount; }

		short GetUserCount() { return (short)m_UserList.size(); }


		NCommon::ERROR_CODE CreateRoom(const wchar_t* pRoomTitle);

		NCommon::ERROR_CODE EnterUser(User* pUser);

		NCommon::ERROR_CODE LeaveUser(const short userIndex);

		bool IsMaster(const short userIndex);

		Game* GetGameObj();

		void Update();

		void SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserindex = -1);

		void NotifyEnterUserInfo(const int userIndex, const char* pszUserID);

		void NotifyLeaveUserInfo(const char* pszUserID);

		void NotifyChat(const int sessionIndex, const char* pszUserID, const wchar_t* pszMsg);

	private:
		ILog* m_pRefLogger;
		TcpNet* m_pRefNetwork;

		// 하나의 Lobby 가 여러개의 Room 을 가지고 있는데
		// 그중 몇번째 Room 인지 -> Lobby 측에서 해당 정보를 세팅해준다.
		short m_Index = -1;
		short m_MaxUserCount;

		bool m_IsUsed = false;
		std::wstring m_Title;
		std::vector<User*> m_UserList;

		Game* m_pGame = nullptr;
	};
}