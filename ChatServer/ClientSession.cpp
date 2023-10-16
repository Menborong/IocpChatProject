#include "pch.h"
#include "ClientSession.h"
#include "User.h"

ClientSession::ClientSession(ref<IocpCore>& iocpCore, const std::function<void()>& releaseCallback): ChatSession(iocpCore, releaseCallback)
{
}

void ClientSession::OnRecv()
{
	while (true)
	{
		ref<Packet> packet = GetRecvPacket();
		if (packet == nullptr)
			break;

		std::string str(reinterpret_cast<const char*>(packet->GetBody()), packet->GetBodySize());
		//std::cout << "Recv message: " << str << std::endl;

		Send(packet);
	}

	Recv();
}

void ClientSession::OnSend()
{
}

void ClientSession::OnAccept()
{
	std::cout << "Client " << GetSocket() << " accepted" << std::endl;
	_userInfo = std::make_shared<User>(std::static_pointer_cast<Session>(shared_from_this()));

	Recv();
}

void ClientSession::OnDisconnect()
{
	std::cout << "Client " << GetSocket() << " is successfully disconnected" << std::endl;
	_userInfo = nullptr;
}

void ClientSession::OnError(int errCode)
{
	if (errCode == 0)
	{
		std::cout << "Client is disconnected" << std::endl;
	}
	else
	{
		if (errCode == 10057)
			std::cout << "CHECK" << std::endl;
		std::cout << "Client" << GetSocket() << " error: " << errCode << std::endl;
	}

	Disconnect();
}
