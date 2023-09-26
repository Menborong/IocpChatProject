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
	Running,		// Connected to remote
	Stop, 			// Stopped (Registration is not possible except for Disconnect)
};

/*---------------------------------------------------------------------------
 * Session
 *
 * Socket 관리, 연결, 그리고 관련 통신을 담당하는 클래스
 * Accept(), Connect(), Disconnect(), Send() 함수를 통해 Remote와 통신처리 가능
 * 해당 클래스의 상속을 통해 콜백함수를 정의하고 통신 이후의 처리를 구현 가능
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
	UINT32 GetNumActiveOperation() const;

protected:
	void Recv(); // This function cannot be called from outside

private:
	SOCKET _socket = INVALID_SOCKET;
	ref<IocpCore> _iocpCore;
	ref<Listener> _listener; // Only for server service (Client Session)

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
