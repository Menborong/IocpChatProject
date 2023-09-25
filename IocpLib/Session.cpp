#include "pch.h"
#include "Session.h"

#include "SocketUtils.h"

Session::Session(ref<IocpCore>& iocpCore, ref<Listener> listener, ref<Acceptor> acceptor, ref<Connector> connector,
	ref<Disconnector> disconnector, ref<Sender> sender, ref<Receiver> receiver)
		: _iocpCore(iocpCore), _listener(listener), _acceptor(acceptor), _connector(connector), _disconnector(disconnector),
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


void Session::Accept()
{
	if (_acceptor)
		_acceptor->Register(GetRef());
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

void Session::Send(ref<SendBuffer>& sendBuffer)
{
	if (_sender)
	{
		_sender->Push(sendBuffer);
		_sender->Register(GetRef());
	}
}

void Session::Recv()
{
	if (_receiver)
		_receiver->Register(GetRef());
}
