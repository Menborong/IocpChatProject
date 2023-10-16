#include "pch.h"
#include "Listener.h"

#include "SocketUtils.h"

bool Listener::Init(const ref<IocpCore>& iocpCore, NetAddress& addr)
{
	_iocpCore = iocpCore;
	_listenSocket = SocketUtils::CreateSocket();
	if(_listenSocket == INVALID_SOCKET)
	{
		SocketUtils::ErrorLog("socket");
		return false;
	}

	// Register the listen socket to the IOCP
	if(_iocpCore->RegisterHandle(reinterpret_cast<HANDLE>(_listenSocket)) == false)
		return false;

	// Socket Settings
	if (SocketUtils::SetReuseAddr(_listenSocket, true) == false)
		return false;
	if (SocketUtils::SetLinger(_listenSocket, false, 0) == false)
		return false;

	// Bind & Listen
	if(SocketUtils::BindSocket(_listenSocket, addr) == false)
		return false;
	if(SocketUtils::ListenSocket(_listenSocket) == false)
		return false;

	return true;
}


bool Listener::Stop()
{
	if(_listenSocket != INVALID_SOCKET)
	{
		closesocket(_listenSocket);
		_listenSocket = INVALID_SOCKET;
	}

	return true;
}
