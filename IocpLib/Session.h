#pragma once
#include "IocpCore.h"
#include "IocpObject.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Disconnector.h"
#include "Sender.h"
#include "Receiver.h"
#include "Packet.h"

enum class SessionStatus
{
	Idle,			// Not connected and initialized
	Ready,			// Ready to connect
	Running,		// Connected to remote
	Stop, 			// Stopped (Registration is not possible except for Disconnect)
};

/*---------------------------------------------------------------------------
 * Session
 *
 * Socket ����, ����, �׸��� ���� ����� ����ϴ� Ŭ����
 * Accept(), Connect(), Disconnect(), Send() �Լ��� ���� Remote�� ���ó�� ����
 * �ش� Ŭ������ ����� ���� �ݹ��Լ��� �����ϰ� ��� ������ ó���� ���� ����
 ----------------------------------------------------------------------------*/

class Session: public IocpObject
{
public:
	Session(ref<IocpCore>& iocpCore, ref<Acceptor> acceptor, ref<Connector> connector, 
		ref<Disconnector> disconnector, ref<Sender> sender, ref<Receiver> receiver);
	~Session() = default;

public:
	void Init();

public:
	// functions for outside
	SOCKET GetSocket() const { return _socket; }
	void Accept(ref<Listener>& listener);
	void Connect(NetAddress addr);
	void Disconnect();
	void Send(ref<Packet>& packet);
	ref<Packet> GetRecvPacket() const { return _receiver->GetRecvPacket(); }
	UINT32 GetNumActiveOperation() const;

protected:
	void Recv(); // This function cannot be called from outside

private:
	SOCKET _socket = INVALID_SOCKET;
	ref<IocpCore> _iocpCore;

	// Session Status
	std::atomic<SessionStatus> _status{SessionStatus::Idle};
	//std::atomic<UINT32> _eventCount{0};

	// Network operations
	ref<Acceptor> _acceptor; // Only for server service (Server Session)
	ref<Connector> _connector; // Only for client service (Client Session)
	ref<Disconnector> _disconnector;
	ref<Sender> _sender;
	ref<Receiver> _receiver;

	// Callback functions
	std::function<void(ref<Session>)> _onRelease; // Called when session is disconnected
};


class AcceptableSession : public Session
{
public:
	AcceptableSession(ref<IocpCore>& iocpCore);

protected:
	virtual void OnRecv() {}
	virtual void OnSend() {}
	virtual void OnAccept() {}
	virtual void OnConnect() {}
	virtual void OnDisconnect() {}
	virtual void OnError(int errCode) {}

private:
	void AcceptCallback();
	void DisconnectCallback();
	void SendCallback();
	void RecvCallback();
	void ErrorCallback(int errCode);
};

class ConnectableSession : public Session
{
public:
	ConnectableSession(ref<IocpCore>& iocpCore);

protected:
	virtual void OnRecv() {}
	virtual void OnSend() {}
	virtual void OnConnect() {}
	virtual void OnDisconnect() {}
	virtual void OnError(int errCode) {}

private:
	void ConnectCallback();
	void DisconnectCallback();
	void SendCallback();
	void RecvCallback();
	void ErrorCallback(int errCode);
};