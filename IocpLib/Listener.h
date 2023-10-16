#pragma once
#include "IocpCore.h"
#include "NetAddress.h"


/*-----------------------------------------------------------
 * Listener
 *
 * Listener 소켓을 관리하는 클래스
 *-----------------------------------------------------------*/

class Listener
{
public:
	Listener() = default;
	virtual ~Listener() = default;

public:
	bool Init(const ref<IocpCore>& iocpCore, NetAddress& addr);
	bool Stop();

public:
	SOCKET GetSocket() const { return _listenSocket; }

private:
	SOCKET _listenSocket = INVALID_SOCKET;
	ref<IocpCore> _iocpCore;
};

