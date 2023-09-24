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
 * Socket 관리, 연결, 그리고 관련 통신을 담당하는 클래스
 * Accept(), Connect(), Disconnect(), Send() 함수를 통해 Remote와 통신처리 가능
 * 해당 클래스의 상속을 통해 콜백함수를 정의하고 통신 이후의 처리를 구현 가능
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
