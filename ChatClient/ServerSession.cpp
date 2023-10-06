#include "pch.h"
#include "ServerSession.h"

void ServerSession::OnRecv()
{
	while(true)
	{
		ref<Packet> packet = GetRecvPacket();
		if (packet == nullptr)
			break;

		std::string str(reinterpret_cast<const char*>(packet->GetBody()), packet->GetBodySize());
		std::cout << "Recv message: " << str << std::endl;
	}

	Recv();
}

void ServerSession::OnSend()
{
}

void ServerSession::OnConnect()
{
	std::cout << "Server is connected" << std::endl;
	Recv();
}

void ServerSession::OnDisconnect()
{
	std::cout << "Server is disconnected" << std::endl;

}

void ServerSession::OnError(int errCode)
{
	if(errCode == 0)
	{
		std::cout << "Server is disconnected" << std::endl;
	}
	else
	{
		std::cout << "Server" << GetSocket() << " error: " << errCode << std::endl;
	}

	Disconnect();
}
