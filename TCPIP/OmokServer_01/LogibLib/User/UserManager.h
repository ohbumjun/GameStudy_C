#pragma once
#include <unordered_map>
#include <deque>
#include <string>
#include <vector>

#include "ErrorCode.h"

namespace NCommon
{
	enum class ERROR_CODE :short;
}
using ERROR_CODE = NCommon::ERROR_CODE;
	
namespace NLogicLib
{
	class User;
}
namespace NLogicLib
{

	class UserManager
	{
	public:
		UserManager();
		virtual ~UserManager();

		void Init(const int maxUserCount);

		// Login 을 하면 User 가 추가된다.
		NCommon::ERROR_CODE AddUser(const int sessionIndex, const char* pszID);
		NCommon::ERROR_CODE RemoveUser(const int sessionIndex);
		std::tuple<NCommon::ERROR_CODE, User*> GetUser(const int sessionIndex);
	private:
		User* AllocUserObjPoolIndex();
		void ReleaseUserObjPoolIndex(const int index);
		User* FindUser(const int sessionIndex);
		User* FindUser(const char* pszID);
	private:
		// UserObjPool 과 ClientSession 은 아예 다른 것인가 ?
		std::vector<User> m_UserObjPool;
		// m_UserObjPool 에서 사용가능한 idx 정보들을 dequeu 형태로 관리한다.
		std::deque<int> m_UserObjPoolIndex;
		// sesion index 로 User 를 찾을 때 사용하는 map
		std::unordered_map<int, User*> m_UserSessionDic;
		// ID 로 User 를 찾을 때 사용하는 map
		std::unordered_map<const char*, User*> m_UserIDDic;
	};
}