#pragma once
#include "pch.h"

/*-----------------------------------------------------------
 *  NetAddress
 *
 *  Class that manages the IPV4 address system used in Socket
 ------------------------------------------------------------*/

class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(const std::string& address, const UINT16 port);
	NetAddress(const char* address, const UINT16 port);
	NetAddress(const UINT16 port);
	NetAddress(const SOCKADDR_IN& address);

public:
	bool SetAddress(const char* address, const UINT16 port);
	bool SetAddress(const UINT16 port);

public:
	SOCKADDR_IN GetAddress() const { return _address; }
	std::string GetIP() const;
	UINT16 GetPort() const { return ntohs(_address.sin_port); }

private:
	SOCKADDR_IN _address;
};

