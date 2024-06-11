#include <iostream>
#include <thread>

#include "Server.h"

int main()
{
	Server server;
	server.Init();

	// Server.Run  내부에서 tcpNetwork.Run 을 호출하는데
	// 블로킹 함수 select 이기 때문에 멈출 수 있다. 따라서 별도의 thread 로 실행한 것이다.
	std::thread runThread([&]() { 		
		server.Run(); }
	);


	
	std::cout << "press any key to exit...";
	getchar();

	server.Stop();
	runThread.join();

	return 0;
}