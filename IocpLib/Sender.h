#pragma once
#include "SessionNetOp.h"
#include "Packet.h"
#include "IocpEvent.h"

class Sender: public SessionNetOp
{
public:
	Sender(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError);
	~Sender();

	void Push(const ref<Packet>& packet);
	void Register(ref<IocpObject> owner) override;
	void Process(bool ret, DWORD numBytes) override;

private:
	SendEvent _event = {};
	std::mutex _mutex;
	std::queue<ref<Packet>> _packetQueue;
	std::vector<ref<Packet>> _sendingPackets;
};

