#pragma once
#include "Packet.h"
#include "Types.h"
#include "IUser.h"

class IRoom
{
public:
	virtual ~IRoom() = default;
	virtual void Enter(ref<IUser> user) = 0;
	virtual void Leave(ref<IUser> user) = 0;
	virtual void ProcessPacket(ref<Packet> packet) = 0;
};
