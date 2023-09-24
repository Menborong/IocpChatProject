#include "pch.h"

#include "SocketUtils.h"
#include "Session.h"


class ServerSession : public Session
{
public:
	ServerSession(ref<IocpCore>& iocpCore)
		: Session(
			iocpCore, nullptr,
			nullptr,
			std::make_shared<Connector>([this] { OnConnect(); }, [this](int errCode) { OnError(errCode); }),
			std::make_shared<Disconnector>([this] { OnDisconnect(); }, [this](int errCode) { OnError(errCode); }),
			std::make_shared<Sender>([this] { OnSend(); }, [this](int errCode) { OnError(errCode); }),
			std::make_shared<Receiver>([this] { OnRecv(); }, [this](int errCode) { OnError(errCode); })
		)
	{}

	// Callback functions
	/*void OnAccept();*/
	void OnConnect()
	{
		std::cout << "OnConnect" << std::endl;
		Recv();
		std::string str = "Hello World!";
		ref<SendBuffer> sendBuffer = std::make_shared<SendBuffer>(str.size());
		sendBuffer->Write(str.c_str(), static_cast<UINT32>(str.size()));
		Send(sendBuffer);
	}
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
		std::cout << "OnRecv" << std::endl;
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
	for (int i = 0; i < 10; i++)
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

	for (int i = 0; i < 8; i++)
	{
		threads[i].join();
	}

	SocketUtils::Clear();

	return 0;
}
