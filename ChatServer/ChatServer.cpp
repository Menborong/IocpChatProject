#include "pch.h"

#include "SocketUtils.h"
#include "Session.h"


class ClientSession : public AcceptableSession
{
public:
	ClientSession(ref<IocpCore>& iocpCore)
		: AcceptableSession(iocpCore)
	{
	}

	// Callback functions
	void OnAccept() override
	{
		std::cout << "OnAccept" << std::endl;
		Recv();
	}
	//void Onconnect();
	void OnDisconnect() override
	{
		std::cout << "OnDisconnect" << std::endl;
	}
	void OnSend() override
	{
		std::cout << "OnSend" << std::endl;
	}
	void OnRecv() override
	{
		std::cout << "OnRecv: ";
		while(true)
		{
			ref<Packet> packet = GetRecvPacket();
			if (packet == nullptr)
				break;

			std::string str(reinterpret_cast<const char*>(packet->GetBody()), packet->GetBodySize());
			std::cout << str << "// ";
		}
		std::cout << std::endl;
		
		Recv();
	}
	void OnError(int errCode) override
	{
		std::cout << "OnError: " << errCode << std::endl;
		Disconnect();
	}
};


void threadMain(ref<IocpCore> iocpCore)
{
	while (true)
		iocpCore->Dispatch();
}

int main()
{
	SocketUtils::Init();

	ref<IocpCore> iocpCore = std::make_shared<IocpCore>();
	ref<Listener> listener = std::make_shared<Listener>();
	NetAddress address(7777);
	listener->Init(iocpCore, address);

	ClientSession session(iocpCore);
	session.Init();

	std::vector<ref<Session>> sessions;
	for (int i = 0; i < 100; i++)
	{
		sessions.emplace_back(std::make_shared<ClientSession>(iocpCore));
		sessions[i]->Init();
		sessions[i]->Accept(listener);
	}

	std::vector<std::thread> threads;
	for (int i = 0; i < 8; i++)
	{
		threads.emplace_back(threadMain, iocpCore);
	}

	for (int i = 0; i < 8; i++)
	{
		threads[i].join();
	}

	SocketUtils::Clear();

	return 0;
}
