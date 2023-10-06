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
 * Socket 관리, 연결, 그리고 관련 통신을 담당하는 클래스
 * Accept(), Connect(), Disconnect(), Send() 함수를 통해 Remote와 통신처리
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
 * 실제 Chatting을 위한 Session Class
 * Session의 상태, Session간의 기본적인 통신 메커니즘,
 * 안정적인 Disconnect 처리를 해당 Class에서 처리한다.
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