using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static csharp_test_client.RoomInfoManager;

namespace csharp_test_client
{
    // 사실 이것도 main form class 이다.
    // 다만 Packet 처리와 관련된 녀석들만 여기에 모아놓은 것이라고 생각하면 된다.
    public partial class mainForm
    {
        Dictionary<PACKET_ID, Action<byte[]>> PacketFuncDic = new Dictionary<PACKET_ID, Action<byte[]>>();

        void SetPacketHandler()
        {
            //PacketFuncDic.Add(PACKET_ID.PACKET_ID_ERROR_NTF, PacketProcess_ErrorNotify);
            PacketFuncDic.Add(PACKET_ID.LOGIN_RES, PacketProcess_LoginResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_ENTER_RES, PacketProcess_RoomEnterResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_USER_LIST_NTF, PacketProcess_RoomUserListNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_NEW_USER_NTF, PacketProcess_RoomNewUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_RES, PacketProcess_RoomLeaveResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_USER_NTF, PacketProcess_RoomLeaveUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_LIST_RES, PacketProcess_RoomListResponse);            
            PacketFuncDic.Add(PACKET_ID.ROOM_CREATE_RES, PacketProcess_RoomCreateResponse);            
            PacketFuncDic.Add(PACKET_ID.ROOM_CHAT_RES, PacketProcess_RoomChatResponse);            
            PacketFuncDic.Add(PACKET_ID.ROOM_CHAT_NOTIFY, PacketProcess_RoomChatNotify);
            PacketFuncDic.Add(PACKET_ID.LOBBY_LIST_RES, PacketProcess_LobbyListResponse);
            PacketFuncDic.Add(PACKET_ID.LOBBY_ENTER_RES, PacketProcess_LobbyEnterResponse);
            //PacketFuncDic.Add(PACKET_ID.PACKET_ID_ROOM_RELAY_NTF, PacketProcess_RoomRelayNotify);
        }

        void PacketProcess(PacketData packet)
        {
            var packetType = (PACKET_ID)packet.PacketID;
            //DevLog.Write("Packet Error:  PacketID:{packet.PacketID.ToString()},  Error: {(ERROR_CODE)packet.Result}");
            //DevLog.Write("RawPacket: " + packet.PacketID.ToString() + ", " + PacketDump.Bytes(packet.BodyData));

            if (PacketFuncDic.ContainsKey(packetType))
            {
                PacketFuncDic[packetType](packet.BodyData);
            }
            else
            {
                DevLog.Write("Unknown Packet Id: " + packet.PacketID.ToString());
            }         
        }

        void PacketProcess_ErrorNotify(byte[] bodyData)
        {
            var notifyPkt = new ErrorNtfPacket();
            notifyPkt.FromBytes(bodyData);

            DevLog.Write($"에러 통보 받음:  {notifyPkt.Error}");
        }


        void PacketProcess_LoginResponse(byte[] bodyData)
        {
            var responsePkt = new LoginResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"로그인 결과:  {(ERROR_CODE)responsePkt.Result}");
        }
		void PacketProcess_LobbyListResponse(byte[] bodyData)
		{
			var responsePkt = new LobbyListResPacket();
			responsePkt.FromBytes(bodyData);

            // Lobby List 정보를 UI 에 뿌려줘야 한다.
            RefreshLobbyListInfo();

			for (int i = 0; i < responsePkt.LobbyCount; ++i)
			{
				LobbyListInfo lobbyListInfo = responsePkt.LobbyList[i];
				AddLobbyListInfo(lobbyListInfo.LobbyId, lobbyListInfo.LobbyUserCount, lobbyListInfo.LobbyMaxUserCount);
			}
		}

		void PacketProcess_RoomListResponse(byte[] bodyData)
		{
			var responsePkt = new RoomListResPacket();
			responsePkt.FromBytes(bodyData);

			// Lobby List 정보를 UI 에 뿌려줘야 한다.
			RefreshRoomListInfo();

			for (int i = 0; i < responsePkt.RoomCount; ++i)
			{
				RoomListInfo roomListInfo = responsePkt.RoomList[i];
				AddRoomListInfo(roomListInfo.RoomIndex, roomListInfo.RoomUserCount, roomListInfo.RoomMaxUserCount);
			}
		}
        void PacketProcess_RoomCreateResponse(byte[] bodyData)
        {
            var responsePkt = new RoomCreateResPacket();

            responsePkt.FromBytes(bodyData);

            roomInfoManager.currentRoomInfos.Add(
                new CurrentRoomInfo(responsePkt.RoomIndex, 0, responsePkt.RoomMaxUserCnt));

            ResetRoomListTextWithRoomInfo();
        }

        void PacketProcess_LobbyEnterResponse(byte[] bodyData)
		{
			var responsePkt = new LobbyEnterResPacket();

			responsePkt.FromBytes(bodyData);

            // 현재 로비 정보 표시
            curLobbyLabelNum.Text = listBoxLobby.SelectedIndex.ToString();

			userInfo.currentLobbyId = (short)listBoxLobby.SelectedIndex;
		}

		void PacketProcess_RoomEnterResponse(byte[] bodyData)
        {
            var responsePkt = new RoomEnterResPacket();

            responsePkt.FromBytes(bodyData);

            userInfo.currentRoomId = responsePkt.RoomIndex;

            textBoxRoomNumber.Text = responsePkt.RoomIndex.ToString();

            roomInfoManager.IncreaseUserCountInRoom(responsePkt.RoomIndex);

            ResetRoomListTextWithRoomInfo();

            DevLog.Write($"방 입장 결과:  {(ERROR_CODE)responsePkt.Result}");
        }

        void PacketProcess_RoomUserListNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomUserListNtfPacket();
            notifyPkt.FromBytes(bodyData);

            for (int i = 0; i < notifyPkt.UserCount; ++i)
            {
                AddRoomUserList(notifyPkt.UserUniqueIdList[i], notifyPkt.UserIDList[i]);
            }

            DevLog.Write($"방의 기존 유저 리스트 받음");
        }

        void PacketProcess_RoomNewUserNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomNewUserNtfPacket();
            notifyPkt.FromBytes(bodyData);

			AddRoomUserList(notifyPkt.UserUniqueId, notifyPkt.UserID);
            
            DevLog.Write($"방에 새로 들어온 유저 받음");
        }


        void PacketProcess_RoomLeaveResponse(byte[] bodyData)
        {
            var responsePkt = new RoomLeaveResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"방 나가기 결과:  {(ERROR_CODE)responsePkt.Result}");
        }

        void PacketProcess_RoomLeaveUserNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomLeaveUserNtfPacket();
            notifyPkt.FromBytes(bodyData);

            RemoveRoomUserList(notifyPkt.UserUniqueId);

            DevLog.Write($"방에서 나간 유저 받음");
        }


        void PacketProcess_RoomChatResponse(byte[] bodyData)
        {
            var responsePkt = new RoomChatResPacket();
            responsePkt.FromBytes(bodyData);

            var errorCode = (ERROR_CODE)responsePkt.Result;
            var msg = $"방 채팅 요청 결과:  {(ERROR_CODE)responsePkt.Result}";
            if (errorCode == ERROR_CODE.ERROR_NONE)
            {
                DevLog.Write(msg, LOG_LEVEL.ERROR);
            }
            else
            {
                AddRoomChatMessageList(0, msg);
            }
        }
		void PacketProcess_RoomChatNotify(byte[] bodyData)
        {
            var responsePkt = new RoomChatNtfPacket();
            responsePkt.FromBytes(bodyData);

            AddRoomChatMessageList(responsePkt.UserUniqueId, responsePkt.Message);
        }

        void AddRoomChatMessageList(Int64 userUniqueId, string msgssage)
        {
            var msg = $"{userUniqueId}:  {msgssage}";

            if (listBoxRoomChatMsg.Items.Count > 512)
            {
                listBoxRoomChatMsg.Items.Clear();
            }

            listBoxRoomChatMsg.Items.Add(msg);
            listBoxRoomChatMsg.SelectedIndex = listBoxRoomChatMsg.Items.Count - 1;
        }


        void PacketProcess_RoomRelayNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomRelayNtfPacket();
            notifyPkt.FromBytes(bodyData);

            var stringData = Encoding.UTF8.GetString(notifyPkt.RelayData);
            DevLog.Write($"방에서 릴레이 받음. {notifyPkt.UserUniqueId} - {stringData}");
        }
    }
}
