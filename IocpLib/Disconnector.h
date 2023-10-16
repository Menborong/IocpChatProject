#pragma once
#include "SessionNetOp.h"
#include "IocpEvent.h"


/*-----------------------------------------------------------
 * Disconnector
 *
 *-----------------------------------------------------------*/

class Disconnector: public SessionNetOp
{
public:
	Disconnector(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError);
	~Disconnector();
	void Register(ref<IocpObject> owner) override;
	void Process(bool ret, DWORD numBytes) override;

private:
	DisconnectEvent _event = {};
};

