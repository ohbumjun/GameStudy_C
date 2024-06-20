using System;
using System.Net.Sockets;
using System.Net;

namespace csharp_test_client
{
    // 동기 IO 방식의 통신
    public class ClientSimpleTcp
    {
        public Socket Sock = null;   
        public string LatestErrorMsg;
        

        //소켓연결  -> Client 가 서버로 연결을 요청할 때 아래의 함수를 사용한다.      
        public bool Connect(string ip, int port)
        {
            try
            {
                IPAddress serverIP = IPAddress.Parse(ip);
                int serverPort = port;

                Sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                Sock.Connect(new IPEndPoint(serverIP, serverPort));

                if (Sock == null || Sock.Connected == false)
                {
                    return false;
                }

                return true;
            }
            catch (Exception ex)
            {
                LatestErrorMsg = ex.Message;
                return false;
            }
        }

        // 서버가 받은 데이터를 받고 싶을 때
        // 단, 동기 IO 를 사용하기 때문에 Sock.Receive() 함수가 호출되면 블로킹에 걸린다.
        // 따라서 Receive 함수는 worker thread 를 만들어서 호출해야 한다.
        public Tuple<int,byte[]> Receive()
        {

            try
            {
                byte[] ReadBuffer = new byte[2048];
                var nRecv = Sock.Receive(ReadBuffer, 0, ReadBuffer.Length, SocketFlags.None);

                if (nRecv == 0)
                {
                    return null;
                }

                return Tuple.Create(nRecv,ReadBuffer);
            }
            catch (SocketException se)
            {
                LatestErrorMsg = se.Message;
            }

            return null;
        }

        //스트림에 쓰기
        // Send 함수는 별도로 쓰레드를 사용할 필요는 없다.
        // 다만, 여기에서는 값을 보내고자 하면, 버퍼에 내용이 쌓이고
        // 별도의 worker thread 가 해당 버퍼를 실제 보내는 역할을 한다.
        public void Send(byte[] sendData)
        {
            try
            {
                if (Sock != null && Sock.Connected) //연결상태 유무 확인
                {
                    Sock.Send(sendData, 0, sendData.Length, SocketFlags.None);
                }
                else
                {
                    LatestErrorMsg = "먼저 채팅서버에 접속하세요!";
                }
            }
            catch (SocketException se)
            {
                LatestErrorMsg = se.Message;
            }
        }

        //소켓과 스트림 닫기
        public void Close()
        {
            if (Sock != null && Sock.Connected)
            {
                //Sock.Shutdown(SocketShutdown.Both);
                Sock.Close();
            }
        }

        public bool IsConnected() { return (Sock != null && Sock.Connected) ? true : false; }
    }
}
