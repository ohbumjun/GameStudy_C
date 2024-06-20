#include <tuple>

#include "../../Common/Packet.h"
#include "../../Common/PacketId.h"
#include "../../Common/ErrorCode.h"
#include "../../NetLib/TcpNetwork.h"
#include "../User/ConnectedUserManager.h"
#include "../User/User.h"
#include "../User/UserManager.h"
#include "../Lobby/LobbyManager.h"
#include "../Lobby/Lobby.h"
#include "PacketProcess.h"

using PACKET_ID = NCommon::PACKET_ID;

namespace NLogicLib
{
	/*
	 * Client ������ �α��� ��û�� ���� �� �������ִ� �Լ�
	 *
	 */
	ERROR_CODE PacketProcess::Login(PacketInfo packetInfo)
	{
		//TODO: ���� �����Ͱ� PktLogInReq ũ�⸸ŭ���� �����ؾ� �Ѵ�.
		// �н������ ������ pass ���ش�. -> �� ���ǵ� ������ ���̴�. (DB ������ ���)
		// ID �ߺ��̶�� ���� ó���Ѵ�.

		NCommon::PktLogInRes resPkt;

		// packetInfo.pRefData �� Client Application �� ���� Data �߿��� Packet Header �κ��� ������
		// Packet Body �κ��� �����Ѵ�.
		NCommon::PktLogInReq* reqPkt = (NCommon::PktLogInReq*)packetInfo.pRefData;

		// DB �� ������ ���, ���⿡ Login ���� ���� ó���� �� �� ���� ���̴�.
		NCommon::ERROR_CODE addRet = m_pRefUserMgr->AddUser(packetInfo.SessionIndex, reqPkt->szID);

		if (addRet != ERROR_CODE::NONE) {

			// error �� �߻��ߴٸ�, error ������ �����Ѵ�.
			// �̶�. resPkt �� ���� �����ϴ� ��Ŷ�� "������" �κ��̴�.
			resPkt.SetError(addRet);


			// ���� : ���⼭ �ٷ� ���� send �� ȣ������ �ʴ´�.
			// �� ���� network->Run Loop ���� Send ���ش�.
			// ��, ��Ʈ��ũ ���� Ŭ���̾�Ʈ���� ��Ŷ�� ������ �ȴ�.
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(NCommon::PktLogInRes), (char*)&resPkt);
			return addRet;
		}

		// �ش� Client Session �� ���ؼ� Login ó���� �Ѵ�.
		m_pConnectedUserManager->SetLogin(packetInfo.SessionIndex);

		resPkt.ErrorCode = (short)addRet;

		// �ٽ� �ش� Ŭ���̾�Ʈ ������ ���� Client Application ������ ��Ŷ�� ������.
		// (����) ���� ��Ʈ��ũ �������� SendSocket �� ȣ���ϰ� �ȴ�.
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOGIN_IN_RES, sizeof(NCommon::PktLogInRes), (char*)&resPkt);

		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::LobbyList(PacketInfo packetInfo)
	{
		// ���� ���� �����ΰ�?
		// ���� �κ� ���� ���� �����ΰ�?
		NCommon::PktLobbyListRes resPkt;
		auto [errorCode, pUser] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

		if (errorCode != ERROR_CODE::NONE) {
			// 1) ������ ����.
			// 2) Confirm ���°� �ƴϴ�.
			resPkt.SetError(errorCode);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOBBY_LIST_RES, sizeof(resPkt), (char*)&resPkt);
			return errorCode;
		}

		if (pUser->IsCurDomainInLogIn() == false) 
		{
			// User �� Login ���°� �ƴϴ�.
			resPkt.SetError(ERROR_CODE::LOBBY_LIST_INVALID_DOMAIN);
			m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOBBY_LIST_RES, sizeof(resPkt), (char*)&resPkt);
			return ERROR_CODE::LOBBY_LIST_INVALID_DOMAIN;
		}

		m_pRefLobbyMgr->SendLobbyListInfo(packetInfo.SessionIndex);

		return ERROR_CODE::NONE;
	}
}