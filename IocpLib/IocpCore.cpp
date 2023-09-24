#include "pch.h"
#include "IocpCore.h"
#include "SocketUtils.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
	_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if(_iocpHandle == nullptr)
	{
		SocketUtils::ErrorLog("CreateCompletionPort");
	}
	int a = INFINITE;
}

IocpCore::~IocpCore()
{
	CloseHandle(_iocpHandle);
}

bool IocpCore::RegisterHandle(HANDLE handle)
{
	const HANDLE ret = CreateIoCompletionPort(handle, _iocpHandle, 0, 0);
	if (ret != _iocpHandle)
	{
		SocketUtils::ErrorLog("CreateCompletionPort");
		return false;
	}
	return true;
}

void IocpCore::Dispatch(INT32 timeout)
{
	DWORD numBytes = 0;
	IocpEvent* event = nullptr;
	ULONG_PTR key = 0; // not used

	const int ret = GetQueuedCompletionStatus(
		_iocpHandle,
		&numBytes,
		&key,
		reinterpret_cast<LPOVERLAPPED*>(&event),
		timeout
	);

	ref<SessionNetOp> op = event->op; // prevent object from being destroyed
	op->Process(ret, numBytes);
}
