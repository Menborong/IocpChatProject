#pragma once
#include "SessionNetOp.h"
#include "SendBuffer.h"
#include "IocpEvent.h"

class Sender: public SessionNetOp
{
public:
	Sender(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError);
	~Sender();

	void Push(ref<SendBuffer>& buffer);
	void Register() override;
	void Process(bool ret, DWORD numBytes) override;

private:
	SendEvent _event = {};
	std::mutex _mutex;
	std::queue<ref<SendBuffer>> _sendQueue;
	std::vector<ref<SendBuffer>> _sendingBufs;

};

