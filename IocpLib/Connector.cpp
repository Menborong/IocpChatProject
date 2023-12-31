#include "pch.h"
#include "Connector.h"

#include "SocketUtils.h"


Connector::Connector(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError)
	: SessionNetOp(onProcees, onError)
{
}

Connector::~Connector()
{
}

void Connector::Register(ref<IocpObject> owner)
{
	// Socket reuse
	if(SocketUtils::SetReuseAddr(_socket, true) == false)
	{
		_onError(WSAGetLastError());
		return;
	}

	// Bind the socket to any address, any port
	if(SocketUtils::BindSocket(_socket, NetAddress(0)) == false)
	{
		_onError(WSAGetLastError());
		return;
	}

	if (_isRunning.exchange(true) == true)
		return;

	_event.owner = owner;
	_event.op = shared_from_this();

	DWORD bytes = 0;
	SOCKADDR_IN sockAddr = _netAddr.GetAddress();
	const int ret = SocketUtils::ConnectEx(
		_socket,
		reinterpret_cast<SOCKADDR*>(&sockAddr),
		sizeof(sockAddr),
		nullptr,
		0,
		&bytes,
		&_event
	);

	if (ret == false)
	{
		const int errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			_event.Clear();
			_isRunning.store(false);
			_onError(errCode);
			return;
		}
	}
}

void Connector::Process(bool ret, DWORD numBytes)
{
	_event.Clear();
	_isRunning.store(false);
	_onProcess();
}
