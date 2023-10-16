#include "pch.h"
#include "Receiver.h"

Receiver::Receiver(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError, ref<RecvBuffer> recvBuffer)
	: SessionNetOp(onProcees, onError), _recvBuffer(recvBuffer)
{
}

ref<Packet> Receiver::GetRecvPacket()
{
	if(_tempPacket == nullptr)
	{
		_tempPacket = std::make_shared<Packet>();
	}

	ref<Packet> packet = _tempPacket;
	packet->Write(_recvBuffer);

	if(packet->IsWriteComplete())
	{
		_tempPacket = nullptr;
		return packet;
	}

	return nullptr;
}

void Receiver::Register(ref<IocpObject> owner)
{
	if (_isRunning.exchange(true) == true)
		return;

	_event.owner = owner;
	_event.op = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer->GetWriteBuffer());
	wsaBuf.len = _recvBuffer->GetNextWrite();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	const int ret = ::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, &_event, nullptr);
	if (ret == SOCKET_ERROR)
	{
		const int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_event.Clear();
			_isRunning.store(false);
			_onError(errorCode);
		}
	}
}

void Receiver::Process(bool ret, DWORD numBytes)
{
	if (numBytes == 0)
	{
		// Client: normal disconnect
		_event.Clear();
		_isRunning.store(false);
		_onError(0);
		return;
	}

	if(_recvBuffer->MoveWritePos(numBytes) == false)
	{
		// 문제 발생 (?)
		assert(false);
	}

	_event.Clear();
	_isRunning.store(false);
	_onProcess();
}
