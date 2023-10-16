#pragma once
#include "IRoom.h"
#include "User.h"

class Room: public IRoom
{
public:
	Room() = default;
	~Room() = default;

public:
	void Enter(ref<IUser> user) override;
	void Leave(ref<IUser> user) override;
	void ProcessPacket(ref<Packet> packet) override;

private:
	void SendAll(ref<Packet> packet);

private:
	std::set<ref<IUser>> _users;
	std::recursive_mutex _mutex;
};

