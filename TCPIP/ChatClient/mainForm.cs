using Microsoft.VisualBasic.ApplicationServices;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static csharp_test_client.RoomInfoManager;

namespace csharp_test_client
{
	public class UserInfo
	{
		enum USER_STATE
		{
            NONE,
            LOBBY,
            ROOM,
        }

        USER_STATE userState = USER_STATE.NONE;
        bool cureInLobby = false;
		short currentLobbyId = 0;
		short currentRoomId  = 0; // room index
		public bool IsInLobby()
		{
			return cureInLobby;
		}

		public void EnterLobby(short lobbyId)
		{
            userState = USER_STATE.LOBBY;
            currentLobbyId = lobbyId;
			cureInLobby = true;
        }

		public void LeaveLobby()
		{

		}

		public void EnterRoom(short roomId)
		{
            userState = USER_STATE.ROOM;
            currentRoomId = roomId;
        }
	}

	public class RoomInfoManager
	{
		public struct CurrentRoomInfo
		{
            public CurrentRoomInfo(string roomTitle, short roomIndex, short userCount, short maxUserCount)
			{
				this.roomTitle = roomTitle;
                this.roomIndex = roomIndex;
                this.userCount = userCount;
                this.maxUserCount = maxUserCount;
            }

			public string roomTitle;
            public short roomIndex;
            public short userCount;
            public short maxUserCount;
        }

        public void IncreaseUserCountInRoom(short roomIndex)
		{
			  for (int index = 0; index < currentRoomInfos.Count; ++index)
			{
                  if (currentRoomInfos[index].roomIndex == roomIndex)
				{
                    var roomInfo = currentRoomInfos[index];
                    roomInfo.userCount += 1;
                    currentRoomInfos[index] = roomInfo;
                } 
              }
		}

        public void DecreaseUserCountInRoom(short roomIndex)
        {
            for (int index = 0; index < currentRoomInfos.Count; ++index)
            {
                if (currentRoomInfos[index].roomIndex == roomIndex)
                {
                    var roomInfo = currentRoomInfos[index];
                    roomInfo.userCount -= 1;
                    currentRoomInfos[index] = roomInfo;
                }
            }
        }

        public List<CurrentRoomInfo> currentRoomInfos = new List<CurrentRoomInfo>();
	}

	public partial class mainForm : Form
	{
		UserInfo userInfo = new UserInfo();
        RoomInfoManager roomInfoManager = new RoomInfoManager();

		ClientSimpleTcp Network = new ClientSimpleTcp();

		bool IsNetworkThreadRunning = false;
		bool IsBackGroundProcessRunning = false;

		// 2개의 쓰레드를 만든다.
		// 1) receive 하는 녀석
		// 2) send 하는 녀석 -> 사실 send 는 우리가 언제 보낼지 안다. 우리가 데이터를 보내는 순간 send 이니까
		// 따라서 Send Thread 는 사실 없어도 된다.
		// receive 는 언제 올지 모른다. 따라서 별도 쓰레드를 만들어서 구성해야 한다.
		System.Threading.Thread NetworkReadThread = null;
		System.Threading.Thread NetworkSendThread = null;

		// 서버로부터 데이터가 오게 되면 PacketBufferManager에 데이터가 쌓이게 된다.
		// Read 함수를 호출하면 해당 데이터에서 일부를 잘라서 Packet 데이터 형태로 만들어서 반환한다.
		// 그리고 그 정보가 아래의 RecvPacketQueue 에 저장된다.
		// 만약 해당 Queue 에 데이터가 있다는 것은, 클라이언트가 서버로부터 받은 데이터를 가지고 있다는 것을 의미하게 된다.
		PacketBufferManager PacketBuffer = new PacketBufferManager();
		Queue<PacketData> RecvPacketQueue = new Queue<PacketData>();

		// Send 를 할 경우, 일단 아래의 Queue에 담기고
		// Send Thread 가 아래의 Queue 에 있는 바이너리 데이터를 서버로 보낸다.
		Queue<byte[]> SendPacketQueue = new Queue<byte[]>();

		// UI 는 기본적으로 Work Thread 가 접근할 수 없게 c# 이 막아놨다.
		// 그런데 Receive 를 하고 나면 UI 가 변경되어야 한다. 즉, Receive Thgrad 는 데이터 받아서
		// Packe Queue 에 넣어두는 작업 까지만 한다. 그리고 아래의 time 마다 메인 쓰레드에서 해당 queue 로부터
		// 패킷 정보를 받아와서 UI 를 update 시켜줄 것이다.
		Timer dispatcherUITimer;
		public mainForm()
		{
			InitializeComponent();
		}
		private void mainForm_Load(object sender, EventArgs e)
		{
			// 1024 : 패킷 하나의 최대 크기
			// 그래서 client ~ server 간에 해당 크기 이상의 패킷은 절대 보내지 않기로 약속이 되어 있는 것이다.
			PacketBuffer.Init((8096 * 10), PacketDef.PACKET_HEADER_SIZE, 1024);

			IsNetworkThreadRunning = true;
			NetworkReadThread = new System.Threading.Thread(this.NetworkReadProcess);
			NetworkReadThread.Start();
			NetworkSendThread = new System.Threading.Thread(this.NetworkSendProcess);
			NetworkSendThread.Start();

			// UI 조작은 메인 쓰레드에서 !
			IsBackGroundProcessRunning = true;
			dispatcherUITimer = new Timer();
			dispatcherUITimer.Tick += new EventHandler(BackGroundProcess);
			dispatcherUITimer.Interval = 100;
			dispatcherUITimer.Start();

			// "접속 끊기" 에 해당하는 버튼
			// 접속이 안되어 있다면 해당 버튼도 활성화 X
			btnDisconnect.Enabled = false;

			SetPacketHandler();
			DevLog.Write("프로그램 시작 !!!", LOG_LEVEL.INFO);
		}

		private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			// 쓰레드 종료시키기
			// IsNetworkThreadRunning, IsBackGroundProcessRunning 변수를 Send, Receive Thread 에서 계속해서 체크 중이다.
			// 따라서 해당 변수 값들을 false 로 만들어서 쓰레드 종료 ? 시킨다.
			IsNetworkThreadRunning = false;
			IsBackGroundProcessRunning = false;

			Network.Close();
		}

		private void btnConnect_Click(object sender, EventArgs e)
		{
			string address = textBoxIP.Text;

			if (checkBoxLocalHostIP.Checked)
			{
				address = "127.0.0.1";
			}

			int port = Convert.ToInt32(textBoxPort.Text);

			if (Network.Connect(address, port))
			{
				labelStatus.Text = string.Format("{0}. 서버에 접속 중", DateTime.Now);
				btnConnect.Enabled = false;
				btnDisconnect.Enabled = true;

				DevLog.Write($"서버에 접속 중", LOG_LEVEL.INFO);
			}
			else
			{
				labelStatus.Text = string.Format("{0}. 서버에 접속 실패", DateTime.Now);
			}
		}

		private void btnDisconnect_Click(object sender, EventArgs e)
		{
			SetDisconnectd();
			Network.Close();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(textSendText.Text))
			{
				MessageBox.Show("보낼 텍스트를 입력하세요");
				return;
			}

			List<byte> dataSource = new List<byte>();
			dataSource.AddRange(Encoding.UTF8.GetBytes(textSendText.Text));

			SendPacketQueue.Enqueue(dataSource.ToArray());
		}

		// Receive Thread 에서 계속해서 실행 중인 Thread
		void NetworkReadProcess()
		{
			const Int16 PacketHeaderSize = PacketDef.PACKET_HEADER_SIZE;

			while (IsNetworkThreadRunning)
			{
				if (Network.IsConnected() == false)
				{
					// 네트워크가 ㅇ
					System.Threading.Thread.Sleep(1);
					continue;
				}

				// receive 함수를 통해서 Socket 의 입력 버퍼에 있는 데이터를 가져온다.
				Tuple<int, byte[]> recvData = Network.Receive();

				if (recvData != null)
				{
					// PacketBufferManager 쪽에 복사해준다.
					PacketBuffer.Write(recvData.Item2, 0, recvData.Item1);

					while (true)
					{
						// PacketBufferManager 에 있는 내용들을 Packet Data 형태로 재구성해주고
						// Packet Queue 에 넣어준다.
						ArraySegment<byte> data = PacketBuffer.Read();
						if (data.Count < 1)
						{
							break;
						}

						var packet = new PacketData();
						packet.DataSize = (short)(data.Count - PacketHeaderSize);
						/*
                         * struct PacketHeader
	                     *  {
		                 *     short TotalSize;     // -> data.Offset + 2 인 이유는, TotalSize 를 의미하는 데이터 크기가 2 byte 이기 때문이다.
		                 *                          //     해당 부분만큼 건너뛰고 ID 에 해당하는 부분을 읽고자 하는 것이다.
		                 *     short Id;
		                 *     unsigned char Reserve;
	                     *  };
                         */
						packet.PacketID = BitConverter.ToInt16(data.Array, data.Offset + 2);
						// Type : C++ 코드에서 Reserve 변수에 해당한다.
						packet.Type = (SByte)data.Array[(data.Offset + 4)];
						packet.BodyData = new byte[packet.DataSize];
						Buffer.BlockCopy(data.Array, (data.Offset + PacketHeaderSize), packet.BodyData, 0, (data.Count - PacketHeaderSize));

						// 이때 중요한 것은 "Lock" 을 걸어두는 것
						lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
						{
							RecvPacketQueue.Enqueue(packet);
						}
					}
					//DevLog.Write($"받은 데이터: {recvData.Item2}", LOG_LEVEL.INFO);
				}
				else
				{
					Network.Close();
					SetDisconnectd();
					DevLog.Write("서버와 접속 종료 !!!", LOG_LEVEL.INFO);
				}
			}
		}

		// Send Thread 에서 계속해서 실행 중인 Thread
		void NetworkSendProcess()
		{
			while (IsNetworkThreadRunning)
			{
				System.Threading.Thread.Sleep(1);

				if (Network.IsConnected() == false)
				{
					continue;
				}

				// SendPacketQueue 에 있는 내용을 꺼내서 Socket.Send 호출한다.
				// 이때 중요한 것은 "Lock" 을 걸어두는 것
				lock (((System.Collections.ICollection)SendPacketQueue).SyncRoot)
				{
					if (SendPacketQueue.Count > 0)
					{
						var packet = SendPacketQueue.Dequeue();
						Network.Send(packet);
					}
				}
			}
		}

		void BackGroundProcess(object sender, EventArgs e)
		{
			// Timer 에서 호출되는 함수이다.
			// 특정 Time 마다 해당 함수를 호출할 수 있게 된다.

			// 1) Log 정보를 처리한다.
			ProcessLog();

			try
			{
				var packet = new PacketData();

				lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
				{
					if (RecvPacketQueue.Count() > 0)
					{
						packet = RecvPacketQueue.Dequeue();
					}
				}

				if (packet.PacketID != 0)
				{
					// 2) Packet 처리를 한다.
					PacketProcess(packet);
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(string.Format("ReadPacketQueueProcess. error:{0}", ex.Message));
			}
		}
		private void ProcessLog()
		{
			// 너무 이 작업만 할 수 없으므로 일정 작업 이상을 하면 일단 패스한다.
			int logWorkCount = 0;

			while (IsBackGroundProcessRunning)
			{
				System.Threading.Thread.Sleep(1);

				string msg;

				if (DevLog.GetLog(out msg))
				{
					++logWorkCount;

					if (listBoxLog.Items.Count > 512)
					{
						listBoxLog.Items.Clear();
					}

					listBoxLog.Items.Add(msg);
					listBoxLog.SelectedIndex = listBoxLog.Items.Count - 1;
				}
				else
				{
					break;
				}

				if (logWorkCount > 8)
				{
					break;
				}
			}
		}

		public void SetDisconnectd()
		{
			if (btnConnect.Enabled == false)
			{
				btnConnect.Enabled = true;
				btnDisconnect.Enabled = false;
			}

			SendPacketQueue.Clear();

			listBoxRoomChatMsg.Items.Clear();
			listBoxRoomUserList.Items.Clear();

			labelStatus.Text = "서버 접속이 끊어짐";
		}
		public void PostSendPacket(PACKET_ID packetID, byte[] bodyData)
		{
			if (Network.IsConnected() == false)
			{
				DevLog.Write("서버 연결이 되어 있지 않습니다", LOG_LEVEL.ERROR);
				return;
			}

			Int16 bodyDataSize = 0;
			if (bodyData != null)
			{
				bodyDataSize = (Int16)bodyData.Length;
			}
			var packetSize = bodyDataSize + PacketDef.PACKET_HEADER_SIZE;

			List<byte> dataSource = new List<byte>();
			dataSource.AddRange(BitConverter.GetBytes((Int16)packetSize));
			dataSource.AddRange(BitConverter.GetBytes((Int16)packetID));
			dataSource.AddRange(new byte[] { (byte)0 });

			if (bodyData != null)
			{
				dataSource.AddRange(bodyData);
			}

			// 메인 쓰레드에서 Send 를 호출할 때, 바로 Socket.Send 를 호출하는 것이 아니라
			// SendPacketQueue 에 해당 데이터를 넣어두고, Send Thread 에서 SendPacketQueue 에 있는 데이터를 처리한다.
			SendPacketQueue.Enqueue(dataSource.ToArray());
		}

		void RefreshLobbyListInfo()
		{
			listBoxLobby.Items.Clear();
		}

		void AddLobbyListInfo(short lobbyId, short lobbyUserCnt, short lobbyMaxUserCnt)
		{
			var msg = $"lobby Id: {lobbyId}, user count : {lobbyUserCnt} / {lobbyMaxUserCnt}";
			listBoxLobby.Items.Add(msg);
		}

		void AddRoomUserList(Int64 userUniqueId, string userID)
		{
			var msg = $"{userUniqueId}: {userID}";
			listBoxRoomUserList.Items.Add(msg);
		}

		void ResetRoomListTextWithRoomInfo()
        {
            listBoxRoomList.Items.Clear();

            foreach (var room in roomInfoManager.currentRoomInfos)
			{
                AddRoomListInfoToUI(room.roomTitle, room.roomIndex, room.userCount, room.maxUserCount);
            }
        }

		void AddRoomListInfoToUI(string roomTitle, short roomIndex, short roomUserCnt, short roomMaxUserCnt)
		{
			var msg = $"{roomTitle}, idx: {roomIndex}, user : {roomUserCnt} / {roomMaxUserCnt}";
			listBoxRoomList.Items.Add(msg);
        }

		void RemoveRoomUserList(Int64 userUniqueId)
		{
			object removeItem = null;

			foreach (var user in listBoxRoomUserList.Items)
			{
				var items = user.ToString().Split(":");
				if (items[0].ToInt64() == userUniqueId)
				{
					removeItem = user;
					return;
				}
			}

			if (removeItem != null)
			{
				listBoxRoomUserList.Items.Remove(removeItem);
			}
		}

		// 로그인 요청
		private void login_Click(object sender, EventArgs e)
		{
			var loginReq = new LoginReqPacket();
			loginReq.SetValue(textBoxUserID.Text, textBoxUserPW.Text);

			PostSendPacket(PACKET_ID.LOGIN_REQ, loginReq.ToBytes());
			DevLog.Write($"로그인 요청:  {textBoxUserID.Text}, {textBoxUserPW.Text}");
		}

		// Room List 요청
		private void btn_RoomList_Click(object sender, EventArgs e)
		{
			PostSendPacket(PACKET_ID.ROOM_LIST_REQ, null);
			DevLog.Write($"Room List 요청");
		}

		private void btn_RoomEnter_Click(object sender, EventArgs e)
		{
            if (listBoxRoomList.Items.Count == 0)
            {
                MessageBox.Show("방을 선택하세요. 혹은 방을 생성하세요");
                return;
            }
            var requestPkt = new RoomEnterReqPacket();
            requestPkt.SetValue(listBoxRoomList.SelectedIndex);
            PostSendPacket(PACKET_ID.ROOM_ENTER_REQ, requestPkt.ToBytes());
            DevLog.Write($"방 입장 요청:  {textBoxRoomNumber.Text} 번");

		}
        private void btn_RoomCreate_Click(object sender, EventArgs e)
        {
			if (roomTitleText.Text.IsEmpty() == true)
			{
                MessageBox.Show("방 이름을 입력하세요");
                return;
            }

			// Lobby 에 Enter 한 상태가 아니라면 X
			if (userInfo.IsInLobby() == false)
			{
				return;
			}

            var requestPkt = new RoomCreateReqPacket();
            requestPkt.SetValue(roomTitleText.Text);
            PostSendPacket(PACKET_ID.ROOM_CREATE_REQ, requestPkt.ToBytes());
            DevLog.Write($"새로운 방 생성:  {roomTitleText.Text} 번");
        }

        private void btn_RoomLeave_Click(object sender, EventArgs e)
		{
			PostSendPacket(PACKET_ID.ROOM_LEAVE_REQ, null);
			DevLog.Write($"방 입장 요청:  {textBoxRoomNumber.Text} 번");
		}

		private void btnRoomChat_Click(object sender, EventArgs e)
		{
			if (textBoxRoomSendMsg.Text.IsEmpty())
			{
				MessageBox.Show("채팅 메시지를 입력하세요");
				return;
			}

			var requestPkt = new RoomChatReqPacket();
			requestPkt.SetValue(textBoxRoomSendMsg.Text);

			PostSendPacket(PACKET_ID.ROOM_CHAT_REQ, requestPkt.ToBytes());
			DevLog.Write($"방 채팅 요청");
		}

		private void btnRoomRelay_Click(object sender, EventArgs e)
		{
			//if( textBoxRelay.Text.IsEmpty())
			//{
			//    MessageBox.Show("릴레이 할 데이터가 없습니다");
			//    return;
			//}

			//var bodyData = Encoding.UTF8.GetBytes(textBoxRelay.Text);
			//PostSendPacket(PACKET_ID.PACKET_ID_ROOM_RELAY_REQ, bodyData);
			//DevLog.Write($"방 릴레이 요청");
		}

		// 로비 리스트 요청
		private void buttonLobbyListReq_Click(object sender, EventArgs e)
		{
			PostSendPacket(PACKET_ID.LOBBY_LIST_REQ, null);
			DevLog.Write($"Lobby List 요청");
		}

		// 로비 입장 요청
		private void btnLobbyEnterReq_Click(object sender, EventArgs e)
		{
			if (listBoxLobby.SelectedItem == null)
			{
				DevLog.Write($"입장할 Lobby 번호를 선택하세요");
				return;
			}

			// 이미 입장한 Lobby 가 있다면 그 Lobby 에서 나가야 한다.
			if (curLobbyLabelNum.Text.IsEmpty() == false)
			{
				DevLog.Write($"현재 로비에서 나가세요");
				return;
			}

			var requestPkt = new LobbyEnterReqPacket();

			// 현재 클릭한 Lobby 정보
			string lobbyInfo = listBoxLobby.SelectedItem.ToString();

			// 여기서는 Lobby Index 가 곧 listBoxLobby 에서의 index 가 될 것이다.
			short selectedLobbyIndex = (short)listBoxLobby.SelectedIndex;

			requestPkt.SetValue(selectedLobbyIndex);

			PostSendPacket(PACKET_ID.LOBBY_ENTER_REQ, requestPkt.ToBytes());

			DevLog.Write($"로비 입장 요청:  {textBoxRoomNumber.Text} 번");
		}

		// 로비 나가기 요청
		private void btnLobbyLeaveReq_Click(object sender, EventArgs e)
		{
			// Lobby 의 Room, Chat 내용들 모두 Clear 해줘야 한다.
		}
		private void btnRoomListReq_Click(object sender, EventArgs e)
		{
			var requestPkt = new RoomListReqPacket();
			PostSendPacket(PACKET_ID.ROOM_LIST_REQ, requestPkt.ToBytes());
			DevLog.Write($"방 목록 요청");
		}
	}
}
