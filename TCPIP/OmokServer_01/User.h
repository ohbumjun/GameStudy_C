#pragma once
#include <string>


class User
{
public :
	enum class DOMAIN_STATE {
		NONE = 0,
		LOGIN = 1,
		ROOM = 2,
	};

public:
	User() {}
	virtual ~User() {}

	void Init(const short index)
	{
		m_Index = index;
	}

	void Clear()
	{			
		m_SessionIndex = 0;
		m_ID = "";
		m_IsConfirm = false;
		m_CurDomainState = DOMAIN_STATE::NONE;
		m_RoomIndex = -1;
	}

	void Set(const int sessionIndex, const char* pszID)
	{
		m_IsConfirm = true;
		m_CurDomainState = DOMAIN_STATE::LOGIN;

		m_SessionIndex = sessionIndex;
		m_ID = pszID;

	}

	short GetIndex() { return m_Index; }

	int GetSessioIndex() { return m_SessionIndex;  }

	std::string& GetID() { return m_ID;  }

	bool IsConfirm() { return m_IsConfirm;  }
		
	short GetRoomIndex() { return m_RoomIndex; }

	void EnterRoom(const short roomIndex)
	{
		m_RoomIndex = roomIndex;
		m_CurDomainState = DOMAIN_STATE::ROOM;
	}

	bool IsCurDomainInLogIn() {
		return m_CurDomainState == DOMAIN_STATE::LOGIN ? true : false;
	}

	bool IsCurDomainInRoom() {
		return m_CurDomainState == DOMAIN_STATE::ROOM ? true : false;
	}

		
protected:
	short m_Index = -1;
		
	// 대응되는 Client Session Index
	int m_SessionIndex = -1;

	// User 가 입력하는 ID
	std::string m_ID;
		
	bool m_IsConfirm = false;
		
	// 상태
	// ex) 로그인 상태인지, 로그아웃 상태인지
	DOMAIN_STATE m_CurDomainState = DOMAIN_STATE::NONE;

	// 들어간 방의 idx
	short m_RoomIndex = -1;
};
