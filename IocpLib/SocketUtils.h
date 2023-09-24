#pragma once
#include "NetAddress.h"

/*-----------------------------------------------------------
 *  SocketUtils
 *
 *	Useful functions for WSA / Socket
 *	Init() must be called before using SocketUtils
 ------------------------------------------------------------*/

class SocketUtils
{
public:
	static bool Init(); // Call this function before using IocpUtils
	static void Clear();
	static bool InitExFunc(SOCKET socket, PVOID* func, GUID guid);

public:
	// SOCKET Creation / Close
	static SOCKET CreateSocket();
	static void CloseSocket(SOCKET socket);

	// Listen Socket Setting
	static bool BindSocket(SOCKET socket, uint16_t port);
	static bool BindSocket(SOCKET socket, NetAddress address);
	static bool ListenSocket(SOCKET socket, int32_t backlog = SOMAXCONN);

public:
	// SOCKET Setting
	template<typename T>
	static bool SetSockOpt(SOCKET socket, INT32 level, INT32 option, T value);
	static bool SetLinger(SOCKET socket, UINT16 onoff, UINT16 linger);
	static bool SetReuseAddr(SOCKET socket, UINT16 flag);
	static bool SetRecvBuffSize(SOCKET socket, UINT32 size);
	static bool SetSendBuffSize(SOCKET socket, UINT32 size);
	static bool SetTCPNoDelay(SOCKET socket, UINT16 flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

public:
	// Error Log
	static void ErrorLog(const char* msg, bool requestErrorCode = true);

public:
	static LPFN_ACCEPTEX AcceptEx;
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_DISCONNECTEX DisconnectEx;
};
