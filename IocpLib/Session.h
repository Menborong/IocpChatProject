#pragma once
#include "IocpCore.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Disconnector.h"
#include "Sender.h"
#include "Receiver.h"

/*---------------------------------------------------------------------------
 * Session
 *
 * Socket ����, ����, �׸��� ���� ����� ����ϴ� Ŭ����
 * Accept(), Connect(), Disconnect(), Send() �Լ��� ���� Remote�� ���ó�� ����
 * �ش� Ŭ������ ����� ���� �ݹ��Լ��� �����ϰ� ��� ������ ó���� ���� ����
 ----------------------------------------------------------------------------*/

class Session
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
	void Accept() const;
	void Connect(NetAddress addr) const;
	void Disconnect() const;
	void Send(ref<SendBuffer>& sendBuffer) const;

protected:
	void Recv(); // This function cannot be called from outside

private:
	SOCKET _socket = INVALID_SOCKET;
	ref<IocpCore> _iocpCore;
	ref<Listener> _listener; // Only for server service (Client Session)

	ref<Acceptor> _acceptor; // Only for server service (Server Session)
	ref<Connector> _connector; // Only for client service (Client Session)
	ref<Disconnector> _disconnector;
	ref<Sender> _sender;
	ref<Receiver> _receiver;
};
