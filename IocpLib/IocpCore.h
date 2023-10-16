#pragma once

/*---------------------------------------------------------------------------------------
 *  IocpEvent
 *
 *  Socket I/O Completion Port Event
 *  This Event is registered with ICOP through the register() methods of object.
 *  and when the Event is received from IOCP, the remaining task is performed through op->process().
 ---------------------------------------------------------------------------------------*/

class IocpCore : std::enable_shared_from_this<IocpCore>
{
public:
	IocpCore();
	virtual ~IocpCore();

public:
	HANDLE GetHandle() const { return _iocpHandle; }

public:
	bool RegisterHandle(HANDLE handle);
	void Dispatch(INT32 timeout = INFINITE); // thread pool main

private:
	HANDLE _iocpHandle;
};

