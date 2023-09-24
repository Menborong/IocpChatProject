#pragma once

#include "SessionNetOp.h"

/*---------------------------------------------------------------------------------------
 *  IocpEvent
 *
 *  Socket I/O Completion Port Event
 *  This Event is registered with ICOP through the register() methods of object.
 *  and when the Event is received from IOCP, the remaining task is performed through the object.
 ---------------------------------------------------------------------------------------*/


enum class IocpEventType
{
	Accept,
	Connect,
	Disconnect,
	Recv,
	Send,
};

class IocpEvent: public OVERLAPPED
{
public:
	IocpEvent(IocpEventType type);
	void Init();
public:
	IocpEventType type;
	ref<SessionNetOp> op;
};;


/*-----------------------------------------------------------
 *  AcceptEvent
 *
 *	Socket I/O Completion Port Accept Event
------------------------------------------------------------*/

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(IocpEventType::Accept) {}
	~AcceptEvent() = default;
};


/*-----------------------------------------------------------
 *  ConnectEvent
 *
 *	Socekt I/O Completion Port Connect Event
------------------------------------------------------------*/

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(IocpEventType::Connect) {}
	~ConnectEvent() = default;
};


/*-----------------------------------------------------------
 *  DisconnectEvent
 *
 *	Socket I/O Completion Port Disconnect Event
------------------------------------------------------------*/

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(IocpEventType::Disconnect) {}
	~DisconnectEvent() = default;
};


/*-----------------------------------------------------------
 *  RecvEvent
 *
 *	Socket I/O Completion Port Recv Event
------------------------------------------------------------*/

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(IocpEventType::Recv) {}
	~RecvEvent() = default;
};


/*-----------------------------------------------------------
 *  SendEvent
 *
 *	Socket I/O Completion Port Send Event
------------------------------------------------------------*/

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(IocpEventType::Send) {}
	~SendEvent() = default;
};