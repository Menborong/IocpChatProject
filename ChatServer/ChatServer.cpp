#include "pch.h"

#include "SocketUtils.h"
#include "Session.h"


class ClientSession : public Session
{
public:
	ClientSession(ref<IocpCore>& iocpCore, ref<Listener>& listener)
		: Session(
			iocpCore,
			std::make_shared<Acceptor>([this] { OnAccept(); }, [this](int errCode) { OnError(errCode); }),
			nullptr,
			std::make_shared<Disconnector>([this] { OnDisconnect(); }, [this](int errCode) { OnError(errCode); }),
			std::make_shared<Sender>([this] { OnSend(); }, [this](int errCode) { OnError(errCode); }),
			std::make_shared<Receiver>([this] { OnRecv(); }, [this](int errCode) { OnError(errCode); }, std::make_shared<RecvBuffer>(0x10000))
		)
	{
	}

	// Callback functions
	void OnAccept()
	{
		std::cout << "OnAccept" << std::endl;
		Recv();
	}
	//void Onconnect();
	void OnDisconnect()
	{
		std::cout << "OnDisconnect" << std::endl;
	}
	void OnSend()
	{
		std::cout << "OnSend" << std::endl;
	}
	void OnRecv()
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
	void OnError(int errCode)
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

	ClientSession session(iocpCore, listener);
	session.Init();

	std::vector<ref<Session>> sessions;
	for(int i=0; i<10; i++)
	{
		sessions.emplace_back(std::make_shared<ClientSession>(iocpCore, listener));
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
