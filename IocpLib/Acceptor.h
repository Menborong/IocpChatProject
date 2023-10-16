#pragma once
#include "IocpEvent.h"
#include "SessionNetOp.h"
#include "LIstener.h"

/*-----------------------------------------------------------
 * Acceptor
 *
 *-----------------------------------------------------------*/

class Acceptor: public SessionNetOp
{
public:
	Acceptor(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError);
	~Acceptor() = default;
	void Register(ref<IocpObject> owner) override;
	void Process(bool ret, DWORD numBytes) override;
	void SetListener(const ref<Listener>& listener) { _listener = listener; }

private:
	AcceptEvent _event = {};
	ref<Listener> _listener;
	char _acceptBuffer[256] = {0};
};
