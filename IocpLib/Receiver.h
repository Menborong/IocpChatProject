#pragma once
#include "SessionNetOp.h"
#include "RecvBuffer.h"
#include "IocpEvent.h"
#include "Packet.h"


/*-----------------------------------------------------------
 * Receiver
 *
 *-----------------------------------------------------------*/

class Receiver: public SessionNetOp
{
public:
	Receiver(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError, ref<RecvBuffer> recvBuffer);
	~Receiver() = default;

	//UINT32 GetRecvMessage(BYTE* buffer, UINT32 size);
	ref<Packet> GetRecvPacket();
	void Register(ref<IocpObject> owner) override;
	void Process(bool ret, DWORD numBytes) override;

private:
	RecvEvent _event = {};
	ref<RecvBuffer> _recvBuffer = nullptr;
	ref<Packet> _tempPacket = nullptr;
};

