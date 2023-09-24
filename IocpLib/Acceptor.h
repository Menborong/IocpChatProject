#pragma once
#include "IocpEvent.h"
#include "SessionNetOp.h"
#include "LIstener.h"

class Acceptor: public SessionNetOp
{
public:
	Acceptor(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError, ref<Listener>& listener);
	~Acceptor() = default;
	void Register() override;
	void Process(bool ret, DWORD numBytes) override;

private:
	AcceptEvent _event = {};
	ref<Listener> _listener;
	char _acceptBuffer[256] = {0};
};
