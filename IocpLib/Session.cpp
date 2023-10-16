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

void Session::Recv()
{
	if (_receiver)
		_receiver->Register(GetRef());
}

ChatSession::ChatSession(ref<IocpCore>& iocpCore, const std::function<void()>& releaseCallback)
	: Session(iocpCore,
		std::make_shared<Acceptor>([this] {AcceptCallback(); }, [this](int errCode) {ErrorCallback(errCode); }),
		std::make_shared<Connector>([this] {ConnectCallback(); }, [this](int errCode) {ErrorCallback(errCode); }),
		std::make_shared<Disconnector>([this] {DisconnectCallback(); }, [this](int errCode) {ErrorCallback(errCode); }),
		std::make_shared<Sender>([this] {SendCallback(); }, [this](int errCode) {ErrorCallback(errCode); }),
		std::make_shared<Receiver>([this] {RecvCallback(); }, [this](int errCode) {ErrorCallback(errCode); }, std::make_shared<RecvBuffer>(0x10000))
	),
	_releaseCallback(releaseCallback)
{
}

void ChatSession::Accept(ref<Listener>& listener)
{
	if(_status != SessionStatus::Idle)
		return;

	AddActiveOperator();
	Session::Accept(listener);
}

void ChatSession::Connect(NetAddress addr)
{
	if (_status != SessionStatus::Idle)
		return;

	AddActiveOperator();
	Session::Connect(addr);
}

void ChatSession::Disconnect()
{
	if(_status != SessionStatus::Running)
		return;

	AddActiveOperator();
	Session::Disconnect();
}

void ChatSession::Send(ref<Packet>& packet)
{
	if(_status != SessionStatus::Running)
		return;

	AddActiveOperator();
	Session::Send(packet);
}

void ChatSession::Recv()
{
	if (_status != SessionStatus::Running)
		return;

	AddActiveOperator();
	Session::Recv();
}

void ChatSession::AcceptCallback()
{
	_status = SessionStatus::Running;
	OnAccept();
	SubActiveOperator();
}

void ChatSession::ConnectCallback()
{
	_status = SessionStatus::Running;
	OnConnect();
	SubActiveOperator();
}

void ChatSession::DisconnectCallback()
{
	_status = SessionStatus::Stop;
	OnDisconnect();
	SubActiveOperator();
}

void ChatSession::SendCallback()
{
	OnSend();
	SubActiveOperator();
}

void ChatSession::RecvCallback()
{
	OnRecv();
	SubActiveOperator();

}

void ChatSession::ErrorCallback(int errCode)
{
	OnError(errCode);
	SubActiveOperator();
}

void ChatSession::AddActiveOperator()
{
	_numActiveOperators.fetch_add(1);
}

void ChatSession::SubActiveOperator()
{
	_numActiveOperators.fetch_sub(1);
	if(_numActiveOperators.load() == 0 && _status.load() == SessionStatus::Stop)
	{
		_status.store(SessionStatus::Idle);
		if(_releaseCallback)
			_releaseCallback();
	}
}


