using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace csharp_test_client
{
    // Receive 를 통해 받은 Manager 를 여기에서 저장하고 있다.
    class PacketBufferManager
    {
        int BufferSize = 0;
        int ReadPos = 0;
        int WritePos = 0;

        int HeaderSize = 0;
        int MaxPacketSize = 0;
        byte[] PacketData;      // 바이너리 배열 -> 링 버퍼 처럼 사용하고 있다고 생각하기
        byte[] PacketDataTemp;

        
        public bool Init(int size, int headerSize, int maxPacketSize)
        {
            if (size < (maxPacketSize * 2) || size < 1 || headerSize < 1 || maxPacketSize < 1)
            {
                return false;
            }

            BufferSize = size;
            PacketData = new byte[size];
            PacketDataTemp = new byte[size];
            HeaderSize = headerSize;
            MaxPacketSize = maxPacketSize;

            return true;
        }
		/*
         * 서버에서 받은 데이터를 receive 함수를 통해 얻어오면
         * write 함수를 사용하여 PacketBufferManager 에 저장한다.
         * 
         * 그리고 이 데이터를 하나의 Packet 형태로 만들어서
         * Client Application 에 넘겨줘야 한다. 이 역할은 Read 함수가 한다.
         */
		public bool Write(byte[] data, int pos, int size)
        {
            if (data == null || (data.Length < (pos + size)))
            {
                return false;
            }

            var remainBufferSize = BufferSize - WritePos;

            if (remainBufferSize < size)
            {
                return false;
            }

            /*
             * data         : 원본 바이트 배열
             * pos          : 원본 바이트 배열로부터 pos 위치부터 복사될 것이다.
             * PacketData   : 복사될 대상 바이트 배열
             * WritePos     : PacketData 에서 복사될 위치
             * size         : 복사될 바이트 크기
             */
            Buffer.BlockCopy(data, pos, PacketData, WritePos, size);
            WritePos += size;

            if (NextFree() == false)
            {
                // 버퍼에서 남은 공간이 작으면 Buffer 를 Reallocate 한다.
                // 즉, 남은 공간이 최대 패킷 크기보다 작으면 rotate 시킨다.
                BufferRelocate();
            }
            return true;
        }

        public ArraySegment<byte> Read()
        {
            var enableReadSize = WritePos - ReadPos;

            if (enableReadSize < HeaderSize)
            {
                return new ArraySegment<byte>();
            }

            // 현재 읽고자 하는 전체 Packet Data Size
            Int16 packetDataSize = BitConverter.ToInt16(PacketData, ReadPos);

            if (enableReadSize < packetDataSize)
            {
                return new ArraySegment<byte>();
            }

			// 읽고자 하는 전체 패킷 binary data
			ArraySegment<byte> completePacketData = new ArraySegment<byte>(PacketData, ReadPos, packetDataSize);

            ReadPos += packetDataSize;
            return completePacketData;
        }

        bool NextFree()
        {
            var enableWriteSize = BufferSize - WritePos;

            if (enableWriteSize < MaxPacketSize)
            {
                return false;
            }

            return true;
        }

        void BufferRelocate()
        {
            var enableReadSize = WritePos - ReadPos;

            Buffer.BlockCopy(PacketData, ReadPos, PacketDataTemp, 0, enableReadSize);
            Buffer.BlockCopy(PacketDataTemp, 0, PacketData, 0, enableReadSize);

            ReadPos = 0;
            WritePos = enableReadSize;
        }
    }
}
