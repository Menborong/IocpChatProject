#pragma once

class IUser
{
public:
	IUser() = default;
	virtual ~IUser() = default;
public:
	virtual std::string GetUserName() = 0;
	virtual void SetUserName(const std::string& userName) = 0;
	virtual void ProcessPacket(ref<Packet>& packet) = 0;
	virtual void SendPacket(ref<Packet>& packet) = 0;
};