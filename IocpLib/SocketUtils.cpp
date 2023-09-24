#include "pch.h"
#include "SocketUtils.h"

LPFN_CONNECTEX SocketUtils::ConnectEx = nullptr;
LPFN_ACCEPTEX SocketUtils::AcceptEx = nullptr;
LPFN_DISCONNECTEX SocketUtils::DisconnectEx = nullptr;

bool SocketUtils::Init()
{
	WSADATA wsaData;
	assert(::WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);

	SOCKET dummy = CreateSocket();
	assert(InitExFunc(dummy, reinterpret_cast<LPVOID*>(&ConnectEx), WSAID_CONNECTEX));
	assert(InitExFunc(dummy, reinterpret_cast<LPVOID*>(&DisconnectEx), WSAID_DISCONNECTEX));
	assert(InitExFunc(dummy, reinterpret_cast<LPVOID*>(&AcceptEx), WSAID_ACCEPTEX));
	CloseSocket(dummy);

	return true;
}

void SocketUtils::Clear()
{
	::WSACleanup();
}

bool SocketUtils::InitExFunc(SOCKET socket, PVOID* func, GUID guid)
{
	DWORD bytes = 0;
	const int ret = ::WSAIoctl(
		socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guid, sizeof(guid),
		func, sizeof(*func),
		&bytes, nullptr, nullptr
	);
	if (ret == SOCKET_ERROR)
	{
		ErrorLog("WSAIoctl", true);
		return false;
	}
	return true;
}

SOCKET SocketUtils::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
}

void SocketUtils::CloseSocket(SOCKET socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}

bool SocketUtils::BindSocket(SOCKET socket, uint16_t port)
{
	NetAddress address(port);
	return BindSocket(socket, address);
}

bool SocketUtils::BindSocket(SOCKET socket, NetAddress address)
{
	SOCKADDR_IN addr = address.GetAddress();
	const int ret = ::bind(socket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
	if(ret == SOCKET_ERROR)
	{
		ErrorLog("bind", true);
		return false;
	}
	return true;
}

bool SocketUtils::ListenSocket(SOCKET socket, int32_t backlog)
{
	const int ret = ::listen(socket, backlog);
	if(ret == SOCKET_ERROR)
	{
		ErrorLog("listen", true);
		return false;
	}
	return true;
}

template <typename T>
bool SocketUtils::SetSockOpt(SOCKET socket, INT32 level, INT32 option, T value)
{
	const int ret = ::setsockopt(socket, level, option, reinterpret_cast<const char*>(&value), sizeof(T));
	if(ret == SOCKET_ERROR)
	{
		ErrorLog("setsockopt", true);
		return false;
	}
	return true;
}

bool SocketUtils::SetLinger(SOCKET socket, UINT16 onoff, UINT16 linger)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, LINGER{ onoff, linger });
}

bool SocketUtils::SetReuseAddr(SOCKET socket, UINT16 flag)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBuffSize(SOCKET socket, UINT32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBuffSize(SOCKET socket, UINT32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTCPNoDelay(SOCKET socket, UINT16 flag)
{
	return SetSockOpt(socket, IPPROTO_TCP, TCP_NODELAY, flag);
}

bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

void SocketUtils::ErrorLog(const char* msg, bool requestErrorCode)
{
	if(requestErrorCode)
		std::cout << msg << " Error Code: " << WSAGetLastError() << std::endl;
	else
		std::cout << msg << " Error" << std::endl;
}
