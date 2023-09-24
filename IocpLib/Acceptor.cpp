#include "pch.h"
#include "Acceptor.h"

#include "SocketUtils.h"


Acceptor::Acceptor(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError,
	ref<Listener>& listener)
		: SessionNetOp(onProcees, onError), _listener(listener)
{
}

void Acceptor::Register()
{
	_isRunning.store(true);
	_event.Init();
	_event.op = shared_from_this();

	DWORD bytes = 0;
	const int ret = SocketUtils::AcceptEx(
		_listener->GetSocket(),
		_socket,
		_acceptBuffer,
		0,
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16,
		&bytes,
		&_event
	);

	if(ret == false)
	{
		const int errCode = WSAGetLastError();
		if(errCode != WSA_IO_PENDING)
		{
			_event.op = nullptr; // release the reference
			_isRunning.store(false);
			_onError(errCode);
			return;
		}
	}
}

void Acceptor::Process(bool ret, DWORD numBytes)
{
	// Get socket option from the listen socket
	if (SocketUtils::SetUpdateAcceptSocket(_socket, _listener->GetSocket()) == false)
	{
		_event.op = nullptr; // release the reference
		_isRunning.store(false);
		_onError(WSAGetLastError());
		return;
	}

	_event.op = nullptr; // release the reference
	_isRunning.store(false);
	_onProcess();
}
