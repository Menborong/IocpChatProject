#include "pch.h"
#include "Receiver.h"

Receiver::Receiver(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError)
	: SessionNetOp(onProcees, onError)
{
}

Receiver::~Receiver()
{
}

UINT32 Receiver::GetRecvMessage(BYTE* buffer, UINT32 size)
{
	UINT32 totalLen = 0;
	while(size - totalLen)
	{
		BYTE* data = _recvBuffer.GetReadBuffer();
		const UINT32 len = _recvBuffer.Read(size - totalLen);
		if (len == 0)
			break;
		memcpy(buffer + totalLen, data, len);
		totalLen += len;
	}
	return totalLen;
}

void Receiver::Register(ref<IocpObject> owner)
{
	if (_isRunning.exchange(true) == true)
		return;

	_event.owner = owner;
	_event.op = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.GetWriteBuffer());
	wsaBuf.len = _recvBuffer.GetNextWrite();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	const int ret = ::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, &_event, nullptr);
	if (ret == SOCKET_ERROR)
	{
		int errorCode = ::WSAGetLastError();
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

	if (_recvBuffer.Write(numBytes) != numBytes)
	{
		// 문제 발생 (?)
		assert(false);
	}

	_event.Clear();
	_isRunning.store(false);
	_onProcess();
}
