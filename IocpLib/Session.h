#pragma once
#include "IocpCore.h"
#include "IocpObject.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Disconnector.h"
#include "Sender.h"
#include "Receiver.h"

enum class SessionStatus
{
	Idle,			// Not connected and initialized
	Ready,			// Ready to connect
	Connecting,		// Connecting to remote (Accepting or Connecting)
	Running,		// Connected to remote
	Disconnecting,	// Disconnecting from remote
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
	Session(ref<IocpCore>& iocpCore, ref<Listener> listener, ref<Acceptor> acceptor, ref<Connector> connector, 
		ref<Disconnector> disconnector, ref<Sender> sender, ref<Receiver> receiver);
	~Session() = default;

public:
	void Init();

public:
	// functions for outside
	SOCKET GetSocket() const { return _socket; }
	void Accept();
	void Connect(NetAddress addr);
	void Disconnect();
	void Send(ref<SendBuffer>& sendBuffer);
	UINT32 GetRecvMessage(BYTE* buffer, UINT32 size) const { return _receiver->GetRecvMessage(buffer, size); }

protected:
	void Recv(); // This function cannot be called from outside

private:
	SOCKET _socket = INVALID_SOCKET;
	ref<IocpCore> _iocpCore;
	ref<Listener> _listener; // Only for server service (Client Session)

	// Session Status
	std::atomic<SessionStatus> _status{SessionStatus::Idle};
	std::atomic<UINT32> _eventCount{0};

	// Network operations
	ref<Acceptor> _acceptor; // Only for server service (Server Session)
	ref<Connector> _connector; // Only for client service (Client Session)
	ref<Disconnector> _disconnector;
	ref<Sender> _sender;
	ref<Receiver> _receiver;
};
