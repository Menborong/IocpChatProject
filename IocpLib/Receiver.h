#pragma once
#include "SessionNetOp.h"
#include "RecvBuffer.h"
#include "IocpEvent.h"

class Receiver: public SessionNetOp
{
public:
	Receiver(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError);
	~Receiver();

	UINT32 GetRecvMessage(BYTE* buffer, UINT32 size);
	void Register() override;
	void Process(bool ret, DWORD numBytes) override;

private:
	RecvEvent _event = {};
	RecvBuffer _recvBuffer = RecvBuffer(0x10000);
};

