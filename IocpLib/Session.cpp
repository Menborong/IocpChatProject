#include "pch.h"
#include "Session.h"

#include "SocketUtils.h"

Session::Session(ref<IocpCore>& iocpCore, ref<Acceptor> acceptor, ref<Connector> connector,
	ref<Disconnector> disconnector, ref<Sender> sender, ref<Receiver> receiver)
		: _iocpCore(iocpCore), _acceptor(acceptor), _connector(connector), _disconnector(disconnector),
 		_sender(sender), _receiver(receiver)
{
}

void Session::Init()
{
	_socket = SocketUtils::CreateSocket();
	// Socket reuse
	if (SocketUtils::SetReuseAddr(_socket, true) == false)
	{
		return;
	}

	_iocpCore->RegisterHandle(reinterpret_cast<HANDLE>(_socket));

	if(_acceptor)
		_acceptor->SetSocket(_socket);
	if(_connector)
		_connector->SetSocket(_socket);
	if(_disconnector)
		_disconnector->SetSocket(_socket);
	if(_sender)
		_sender->SetSocket(_socket);
	if(_receiver)
		_receiver->SetSocket(_socket);
}


void Session::Accept(ref<Listener>& listener)
{
	if (_acceptor)
	{
		_acceptor->SetListener(listener);
		_acceptor->Register(GetRef());
	}
}

void Session::Connect(NetAddress addr)
{
	if (_connector)
	{
		_connector->SetAddress(addr);
		_connector->Register(GetRef());
	}
}

void Session::Disconnect()
{
	if (_disconnector)
		_disconnector->Register(GetRef());
}

void Session::Send(ref<Packet>& packet)
{
	if (_sender)
	{
		_sender->Push(packet);
		_sender->Register(GetRef());
	}
}

UINT32 Session::GetNumActiveOperation() const
{
	// 대상 연산이 많지 않으므로 모든 연산을 검사
	UINT32 num = 0;
	if(_acceptor)
		num += _acceptor->IsRunning();
	if(_connector)
		num += _connector->IsRunning();
	if(_disconnector)
		num += _disconnector->IsRunning();
	if(_sender)
		num += _sender->IsRunning();
	if(_receiver)
		num += _receiver->IsRunning();
	return num;
}

void Session::Recv()
{
	if (_receiver)
		_receiver->Register(GetRef());
}
