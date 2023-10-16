#include "pch.h"
#include "Room.h"

void Room::Enter(ref<IUser> user)
{
	const std::string message = user->GetUserName() + " has entered the room";
	UINT16 size = message.size();
	const ref<Packet> packet = std::make_shared<Packet>(reinterpret_cast<const BYTE*>(message.data()), size, PacketType::Chat);
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		_users.insert(user);
	}
	SendAll(packet);
}

void Room::Leave(ref<IUser> user)
{
	const std::string message = user->GetUserName() + " has left the room";
	UINT16 size = message.size();
	const ref<Packet> packet = std::make_shared<Packet>(reinterpret_cast<const BYTE*>(message.data()), size, PacketType::Chat);
	{
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		_users.erase(user);
	}
	SendAll(packet);
}

void Room::ProcessPacket(ref<Packet> packet)
{
	// TODO: Packet processing
	if(packet->GetType() == PacketType::Chat)
	{
		SendAll(packet);
	}
	else
	{
		// Not implemented
		std::cout << "Not implemented" << std::endl;
		assert(false);
	}
}

void Room::SendAll(ref<Packet> packet)
{
	std::lock_guard<std::recursive_mutex> lock(_mutex);
	for(auto& user: _users)
	{
		user->SendPacket(packet);
	}
}
