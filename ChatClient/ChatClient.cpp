#include "pch.h"

#include "SocketUtils.h"
#include "Session.h"
#include "ServerSession.h"


void threadMain(ref<IocpCore> iocpCore)
{
	while (true)
		iocpCore->Dispatch();
}

using namespace std::chrono_literals;

int main()
{
	SocketUtils::Init();

	std::this_thread::sleep_for(1s);

	ref<IocpCore> iocpCore = std::make_shared<IocpCore>();
	NetAddress address("127.0.0.1", 7777);

	std::vector<ref<Session>> sessions;
	for (int i = 0; i < 1000; i++)
	{
		sessions.emplace_back(std::make_shared<ServerSession>(iocpCore));
		sessions[i]->Init();
		sessions[i]->Connect(address);
	}


	std::vector<std::thread> threads;
	for (int i = 0; i < 8; i++)
	{
		threads.emplace_back(threadMain, iocpCore);
	}

	while(true)
	{
		std::string str = "Hello World!";
		ref<Packet> packet = std::make_shared<Packet>(reinterpret_cast<const BYTE*>(str.data()), static_cast<UINT32>(str.size()), PacketType::Chat);
		for(const auto& s: sessions)
		{
			s->Send(packet);
		}
		std::this_thread::sleep_for(100ms);
	}

	for (int i = 0; i < 8; i++)
	{
		threads[i].join();
	}

	SocketUtils::Clear();

	return 0;
}
