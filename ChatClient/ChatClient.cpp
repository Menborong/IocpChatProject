#include "pch.h"

#include "SocketUtils.h"
#include "Session.h"
#include "RecvBuffer.h"
#include "ServerSession.h"


//class ServerSession : public ConnectableSession
//{
//public:
//	ServerSession(ref<IocpCore>& iocpCore)
//		: ConnectableSession(iocpCore)
//	{}
//
//	// Callback functions
//	// void OnAccept();
//	void OnConnect() override
//	{
//		std::cout << "OnConnect" << std::endl;
//		Recv();
//		/*std::string str = "Hello World!";
//		ref<Packet> packet = std::make_shared<Packet>(reinterpret_cast<const BYTE*>(str.data()), static_cast<UINT32>(str.size()), PacketType::Chat);
//		Send(packet);*/
//	}
//	void OnDisconnect() override
//	{
//		std::cout << "OnDisconnect" << std::endl;
//	}
//	void OnSend() override
//	{
//		std::cout << "OnSend" << std::endl;
//	}
//	void OnRecv() override
//	{
//		std::cout << "OnRecv" << std::endl;
//		Recv();
//	}
//	void OnError(int errCode) override
//	{
//		std::cout << "OnError: " << errCode << std::endl;
//		Disconnect();
//	}
//};

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

	ServerSession session(iocpCore);
	session.Init();

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
