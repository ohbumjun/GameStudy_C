using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace csharp_test_client
{
    // Packet Header 와 Body 가 합쳐진 부분이다.
    struct PacketData
    {
        public Int16 DataSize;
        public Int16 PacketID;
        public SByte Type;
        public byte[] BodyData;
    }

    public class PacketDump
    {
        public static string Bytes(byte[] byteArr)
        {
            StringBuilder sb = new StringBuilder("[");
            for (int i = 0; i < byteArr.Length; ++i)
            {
                sb.Append(byteArr[i] + " ");
            }
            sb.Append("]");
            return sb.ToString();
        }
    }
    

    // 아래 클래스들은 모두 "Packet Body" 에 해당하는 Packet 정보들이다.
    public class ErrorNtfPacket
    {
        public ERROR_CODE Error;

        // 서버로부터 받은 바이너리 데이터를 변환해주는 함수
        public bool FromBytes(byte[] bodyData)
        {
            Error = (ERROR_CODE)BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }
    

    public class LoginReqPacket
    {
        byte[] UserID = new byte[PacketDef.MAX_USER_ID_BYTE_LENGTH];
        byte[] UserPW = new byte[PacketDef.MAX_USER_PW_BYTE_LENGTH];

        public void SetValue(string userID, string userPW)
        {
            Encoding.UTF8.GetBytes(userID).CopyTo(UserID, 0);
            Encoding.UTF8.GetBytes(userPW).CopyTo(UserPW, 0);
        }

        // Binary Data 로 만들어주는 함수
        // 보통 Header, Body 를 결합해서 보낸다.
        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(UserID);
            dataSource.AddRange(UserPW);
            return dataSource.ToArray();
        }
    }

    public class LoginResPacket
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }


	// Lobby
	public struct LobbyListInfo
	{
		public Int16 Result;
		public short LobbyId;
		public short LobbyUserCount;
		public short LobbyMaxUserCount;
	};

	public class LobbyListResPacket
	{
		public short LobbyCount = 0;
		public LobbyListInfo[] LobbyList = new LobbyListInfo[20];

        // Error Code
		public Int16 Result;

		public bool FromBytes(byte[] bodyData)
		{
            var readPos = 0;
			Result = BitConverter.ToInt16(bodyData, 0);
            readPos += 2;

			 // if (bodyData.Length < 4) // 최소한 LobbyCount를 읽을 수 있는지 확인
			 // 	return false;
			 // 
			LobbyCount = (short)bodyData[readPos];
			readPos += 2;

			for (int i = 0; i < LobbyCount; ++i)
			{
				LobbyList[i].LobbyId = (short)bodyData[readPos];
				readPos += 2;

				LobbyList[i].LobbyUserCount = (short)bodyData[readPos];
				readPos += 2;

				LobbyList[i].LobbyMaxUserCount = (short)bodyData[readPos];
				readPos += 2;
			}

			return true;
		}
	}

    public class LobbyEnterReqPacket
    {
		short LobbyId;

		public void SetValue(short lobbyId)
		{
			LobbyId = lobbyId;
		}
		public byte[] ToBytes()
		{
			List<byte> dataSource = new List<byte>();
			dataSource.AddRange(BitConverter.GetBytes(LobbyId));
			return dataSource.ToArray();
		}
	}
	public class LobbyEnterResPacket
	{
		short MaxUserCount;
		short MaxRoomCount;
		public bool FromBytes(byte[] bodyData)
		{
			MaxUserCount = BitConverter.ToInt16(bodyData, 0);
			MaxRoomCount = BitConverter.ToInt16(bodyData, 2);
			return true;
		}
	}

	public class LobbyLeaveReqPacket
	{
	}
	public class LobbyLeaveResPacket
	{
	}
	public struct RoomListInfo
	{
		public short RoomIndex;        // Lobby 내 Room Index
		public short RoomUserCount;    // Lobby 내 User Count
		public short RoomMaxUserCount; // Lobby 내 최대 User Count
	};

	public class RoomListReqPacket
	{
		public byte[] ToBytes()
		{
			List<byte> dataSource = new List<byte>();
			return dataSource.ToArray();
		}
	}
	public class RoomListResPacket
	{
		// Error Code
		public Int16 Result;
		public short RoomCount = 0;
		public RoomListInfo[] RoomList = new RoomListInfo[20];
		public bool FromBytes(byte[] bodyData)
		{
			var readPos = 0;
			Result = BitConverter.ToInt16(bodyData, 0);
			readPos += 2;

			// if (bodyData.Length < 4) // 최소한 LobbyCount를 읽을 수 있는지 확인
			// 	return false;
			// 
			RoomCount = (short)bodyData[readPos];
			readPos += 2;

			for (int i = 0; i < RoomCount; ++i)
			{
				RoomList[i].RoomIndex = (short)bodyData[readPos];
				readPos += 2;

				RoomList[i].RoomUserCount = (short)bodyData[readPos];
				readPos += 2;

				RoomList[i].RoomMaxUserCount = (short)bodyData[readPos];
				readPos += 2;
			}

			return true;
		}
	}

	public class RoomEnterReqPacket
    {
        int RoomNumber;
        public void SetValue(int roomNumber)
        {
            RoomNumber = roomNumber;
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(RoomNumber));
            return dataSource.ToArray();
        }
    }

    public class RoomEnterResPacket
    {
        public Int16 Result;
        public Int64 RoomUserUniqueId;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            RoomUserUniqueId = BitConverter.ToInt64(bodyData, 2);
            return true;
        }
    }

    public class RoomUserListNtfPacket
    {
        public int UserCount = 0;
        public List<Int64> UserUniqueIdList = new List<Int64>();
        public List<string> UserIDList = new List<string>();

        public bool FromBytes(byte[] bodyData)
        {
            var readPos = 0;
            var userCount = (SByte)bodyData[readPos];
            ++readPos;

            for (int i = 0; i < userCount; ++i)
            {
                var uniqeudId = BitConverter.ToInt64(bodyData, readPos);
                readPos += 8;

                var idlen = (SByte)bodyData[readPos];
                ++readPos;

                var id = Encoding.UTF8.GetString(bodyData, readPos, idlen);
                readPos += idlen;

                UserUniqueIdList.Add(uniqeudId);
                UserIDList.Add(id);
            }

            UserCount = userCount;
            return true;
        }
    }

    public class RoomNewUserNtfPacket
    {
        public Int64 UserUniqueId;
        public string UserID;
        public bool FromBytes(byte[] bodyData)
        {
            var readPos = 0;

            UserUniqueId = BitConverter.ToInt64(bodyData, readPos);
            readPos += 8;

            var idlen = (SByte)bodyData[readPos];
            ++readPos;

            UserID = Encoding.UTF8.GetString(bodyData, readPos, idlen);
            readPos += idlen;

            return true;
        }
    }


    public class RoomChatReqPacket
    {
        Int16 MsgLen;
        byte[] Msg;//= new byte[PacketDef.MAX_USER_ID_BYTE_LENGTH];

        public void SetValue(string message)
        {
            Msg = Encoding.UTF8.GetBytes(message);
            MsgLen = (Int16)Msg.Length;
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(MsgLen));
            dataSource.AddRange(Msg);
            return dataSource.ToArray();
        }
    }

    public class RoomChatResPacket
    {
        public Int16 Result;
        
        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomChatNtfPacket
    {
        public Int64 UserUniqueId;
        public string Message;

        public bool FromBytes(byte[] bodyData)
        {
            UserUniqueId = BitConverter.ToInt64(bodyData, 0);

            var msgLen = BitConverter.ToInt16(bodyData, 8);
            byte[] messageTemp = new byte[msgLen];
            Buffer.BlockCopy(bodyData, 8 + 2, messageTemp, 0, msgLen);
            Message = Encoding.UTF8.GetString(messageTemp);
            return true;
        }
    }


     public class RoomLeaveResPacket
    {
        public Int16 Result;
        
        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomLeaveUserNtfPacket
    {
        public Int64 UserUniqueId;

		public bool FromBytes(byte[] bodyData)
		{
			UserUniqueId = BitConverter.ToInt64(bodyData, 0);
			return true;
		}

	}



	public class RoomRelayNtfPacket
    {
        public Int64 UserUniqueId;
        public byte[] RelayData;

        public bool FromBytes(byte[] bodyData)
        {
            UserUniqueId = BitConverter.ToInt64(bodyData, 0);

            var relayDataLen = bodyData.Length - 8;
            RelayData = new byte[relayDataLen];
            Buffer.BlockCopy(bodyData, 8, RelayData, 0, relayDataLen);
            return true;
        }
    }


    public class PingRequest
    {
        public Int16 PingNum;

        public byte[] ToBytes()
        {
            return BitConverter.GetBytes(PingNum);
        }

    }
}
