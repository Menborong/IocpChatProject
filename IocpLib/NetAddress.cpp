#include "pch.h"
#include "NetAddress.h"

/*-----------------------------------------------------------
 *  NetAddress
 *
 ------------------------------------------------------------*/

NetAddress::NetAddress(const std::string& address, const UINT16 port)
{
	SetAddress(address.c_str(), port);
}

NetAddress::NetAddress(const char* address, const UINT16 port)
{
	SetAddress(address, port);
}

NetAddress::NetAddress(const UINT16 port)
{
	SetAddress(port);
}

NetAddress::NetAddress(const SOCKADDR_IN& address)
{
	_address = address;
}

bool NetAddress::SetAddress(const char* address, const UINT16 port)
{
	_address = {};
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
	if (inet_pton(AF_INET, address, &_address.sin_addr) != 1)
		return false;
	return true;
}

bool NetAddress::SetAddress(const UINT16 port)
{
	_address = {};
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = ::htonl(INADDR_ANY);
	return true;
}

std::string NetAddress::GetIP() const
{
	char ip[100];
	inet_ntop(AF_INET, &_address.sin_addr, ip, std::size(ip));
	return ip;
}
