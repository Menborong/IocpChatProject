#include "pch.h"
#include "Acceptor.h"

#include "SocketUtils.h"


Acceptor::Acceptor(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError)
		: SessionNetOp(onProcees, onError)
{
}

void Acceptor::Register(ref<IocpObject> owner)
{
	if (_isRunning.exchange(true) == true)
		return;

	_event.owner = owner;
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
			_event.Clear();
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
		_event.Clear();
		_isRunning.store(false);
		_onError(WSAGetLastError());
		return;
	}

	_event.op = nullptr; // release the reference
	_isRunning.store(false);
	_onProcess();
}
