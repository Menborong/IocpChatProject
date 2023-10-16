#pragma once

#include <string>

#include "Packet.h"
#include "Room.h"
#include "Types.h"
#include "Session.h"
#include "IUser.h"

class User: public IUser
{
public:
	// TODO: 기존의 User 정보 불러오기 등등
	User(ref<Session> session);
	~User() override = default;

public:
	std::string GetUserName() override { return _userName; }
	void SetUserName(const std::string& userName) override { _userName = userName; }
	void ProcessPacket(ref<Packet>& packet) override;
	void SendPacket(ref<Packet>& packet) override;

private:
	std::string _userName;
	ref<Session> _session;
	ref<IRoom> _room;

};

