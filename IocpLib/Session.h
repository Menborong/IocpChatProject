#pragma once
#include "IocpCore.h"
#include "IocpObject.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Disconnector.h"
#include "Sender.h"
#include "Receiver.h"
#include "Packet.h"

/*---------------------------------------------------------------------------
 * Session
 *
 * Socket ����, ����, �׸��� ���� ����� ����ϴ� Ŭ����
 * Accept(), Connect(), Disconnect(), Send() �Լ��� ���� Remote�� ���ó��
 *
 * Accept(ref<Listener>): Remote���� ���� ��� ���
 * Connect(NetAddress): Remote���� ���� �õ� ���
 * Disconnect(): Remote���� ���� ���� ���
 * Send(ref<Packet>): Remote���� Packet ���� ���
 * GetRecvPacket(): Remote�κ��� ���ŵ� Packet ��ȯ, ���н� nullptr ��ȯ
 *
 * Recv(): Remote�κ��� Packet ���� ��� (�� �Լ��� �ܺο��� ȣ���� �� ����)
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

	virtual void Accept(ref<Listener>& listener);
	virtual void Connect(NetAddress addr);
	virtual void Disconnect();
	virtual void Send(ref<Packet>& packet);
	ref<Packet> GetRecvPacket() const { return _receiver->GetRecvPacket(); }

protected:
	virtual void Recv(); // This function cannot be called from outside

private:
	SOCKET _socket = INVALID_SOCKET;
	ref<IocpCore> _iocpCore;

	// Network operators
	ref<Acceptor> _acceptor; // Only for server service (Server Session)
	ref<Connector> _connector; // Only for client service (Client Session)
	ref<Disconnector> _disconnector;
	ref<Sender> _sender;
	ref<Receiver> _receiver;
};


/*---------------------------------------------------------------------------
 * ChatSession
 *
 * ���� Chatting�� ���� Session Class
 * Session�� ����, Session���� �⺻���� ��� ��Ŀ����,
 * �������� Disconnect ó���� �ش� Class���� ó���Ѵ�.
 *
 * ������ �Լ����� �������̵��Ͽ� IOCP ��û ����� ó���� �� ����:
 * OnRecv(), OnSend(), OnAccept(), OnConnect(), OnDisconnect(), OnError()
 ----------------------------------------------------------------------------*/

enum class SessionStatus
{
	Idle,			// Not connected and initialized
	Ready,			// Ready to connect
	Running,		// Connected to remote
	Stop, 			// Stopped (All registers are banned)
};

class ChatSession: public Session
{
public:
	ChatSession(ref<IocpCore>& iocpCore, const std::function<void()>& releaseCallback);

public:
	void Accept(ref<Listener>& listener);
	void Connect(NetAddress addr);
	void Disconnect();
	void Send(ref<Packet>& packet);

protected:
	void Recv(); // This function cannot be called from outside


protected:
	/* functions for application */
	virtual void OnRecv() {}
	virtual void OnSend() {}
	virtual void OnAccept() {}
	virtual void OnConnect() {}
	virtual void OnDisconnect() {}
	virtual void OnError(int errCode) {}

private:
	/* Callback functions for Network Operators */
	void AcceptCallback();
	void ConnectCallback();
	void DisconnectCallback();
	void SendCallback();
	void RecvCallback();
	void ErrorCallback(int errCode);

private:
	/* Status functions */
	void AddActiveOperator();
	void SubActiveOperator();

private:
	std::atomic<SessionStatus> _status{ SessionStatus::Idle };
	std::atomic<UINT8> _numActiveOperators{ 0 };

	std::function<void()> _releaseCallback = nullptr;
	
};
