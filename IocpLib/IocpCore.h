#pragma once

/*-----------------------------------------------------------
 *  IocCore
 *
 *	Manage I/O Completion Port Handle, Thread Pool Main
 *	Each thread in the thread pool calls Dispatch() to get the event
 ------------------------------------------------------------*/

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

