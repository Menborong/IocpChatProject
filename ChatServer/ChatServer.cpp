#include "pch.h"
#include "ClientSession.h"
#include "SocketUtils.h"

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
	for (int i = 0; i < 1000; i++)
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
