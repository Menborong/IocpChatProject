#include "pch.h"
#include "Sender.h"


Sender::Sender(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError)
	: SessionNetOp(onProcees, onError)
{
}

Sender::~Sender()
{
}

void Sender::Push(const ref<Packet>& packet)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_packetQueue.push(packet);
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

		if(_packetQueue.empty())
		{
			_event.Clear();
			_isRunning.store(false);
			return;
		}

		while (!_packetQueue.empty())
		{
			_sendingPackets.emplace_back(_packetQueue.front());
			_packetQueue.pop();
		}
	}

	std::vector<WSABUF> wsaBufs;
	for(const auto& packet: _sendingPackets)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(packet->GetBYTE());
		wsaBuf.len = packet->GetSize();
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
			_sendingPackets.clear();
			_event.Clear();
			_isRunning.store(false);
			_onError(errCode);
			return;
		}
	}
}

void Sender::Process(bool ret, DWORD numBytes)
{
	_sendingPackets.clear();
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
