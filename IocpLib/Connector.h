#pragma once
#include "SessionNetOp.h"
#include "IocpEvent.h"
#include "NetAddress.h"


/*-----------------------------------------------------------
 * Connector
 *
 *-----------------------------------------------------------*/

class Connector: public SessionNetOp
{
public:
	Connector(const std::function<void()>& onProcees, const std::function<void(int errCode)>& onError);
	~Connector();
	void Register(ref<IocpObject> owner) override;
	void Process(bool ret, DWORD numBytes) override;
	void SetAddress(NetAddress netAddr) { _netAddr = netAddr; }

private:
	ConnectEvent _event = {};
	NetAddress _netAddr = {};
};

