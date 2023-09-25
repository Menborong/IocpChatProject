#include "pch.h"
#include "Sender.h"


Sender::Sender(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError)
	: SessionNetOp(onProcees, onError)
{
}

Sender::~Sender()
{
}

void Sender::Push(ref<SendBuffer>& buffer)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_sendQueue.emplace(buffer);
}

void Sender::Register(ref<IocpObject> owner)
{
	// Caution: Thread-Safety
	if(_isRunning.exchange(true) == true)
		return;

	_event.owner = owner;
	_event.op = shared_from_this();

	// Scatter-Gather I/O
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if(_sendQueue.empty())
		{
			_event.Clear();
			_isRunning.store(false);
			return;
		}

		while (!_sendQueue.empty())
		{
			_sendingBufs.emplace_back(_sendQueue.front());
			_sendQueue.pop();
		}
	}

	std::vector<WSABUF> wsaBufs;
	for (const auto& buffer : _sendingBufs)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(buffer->GetBuffer());
		wsaBuf.len = buffer->GetWriteSize();
		wsaBufs.emplace_back(wsaBuf);
	}

	DWORD bytes = 0;
	const int ret = WSASend(
		_socket,
		wsaBufs.data(),
		static_cast<DWORD>(wsaBufs.size()),
		&bytes,
		0,
		&_event,
		nullptr
	);

	if (ret == SOCKET_ERROR)
	{
		const int errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			_sendingBufs.clear();
			_event.Clear();
			_isRunning.store(false);
			_onError(errCode);
			return;
		}
	}
}

void Sender::Process(bool ret, DWORD numBytes)
{
	_sendingBufs.clear();

	if (numBytes == 0)
	{
		// normal disconnect from remote
		_event.Clear();
		_isRunning.store(false);
		_onError(0);
	}

	_event.Clear();
	_isRunning.store(false);
	_onProcess();
}
