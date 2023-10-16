#include "pch.h"
#include "User.h"

User::User(ref<Session> session)
	: _session(session)
{
}

void User::ProcessPacket(ref<Packet>& packet)
{
	if(packet->GetType() == PacketType::Chat)
	{
		// Add user name to packet
		const std::string newMessage = "[" + _userName + "] " + reinterpret_cast<const char*>(packet->GetBody());
		ref<Packet> newPacket = std::make_shared<Packet>(reinterpret_cast<const BYTE*>(newMessage.data()), newMessage.size(), PacketType::Chat);
		_room->ProcessPacket(packet);
	}
	else
	{
		// Not implemented
		std::cout << "Not implemented" << std::endl;
		assert(false);
	}
}

void User::SendPacket(ref<Packet>& packet)
{
	_session->Send(packet);
}
