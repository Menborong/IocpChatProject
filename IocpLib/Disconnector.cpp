#include "pch.h"
#include "Disconnector.h"

#include "SocketUtils.h"

Disconnector::Disconnector(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError)
	: SessionNetOp(onProcees, onError)
{
}

Disconnector::~Disconnector()
{
}

void Disconnector::Register()
{
	_event.Init();
	_event.op = shared_from_this();
	_isRunning.store(true);

	const int ret = SocketUtils::DisconnectEx(
		_socket,
		&_event,
		TF_REUSE_SOCKET,
		0
	);

	if (ret == false)
	{
		const int errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			_event.op = nullptr; // release the reference
			_isRunning.store(false);
			_onError(errCode);
			return;
		}
	}
}

void Disconnector::Process(bool ret, DWORD numBytes)
{
	_event.op = nullptr; // release the reference
	_isRunning.store(false);
	_onProcess();
}
