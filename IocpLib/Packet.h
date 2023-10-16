#pragma once
#include "RecvBuffer.h"

enum class PacketType : UINT16
{
	Chat,
	Error,
	System,
};

class PacketHeader
{
public:
	UINT16 size = 0;
	PacketType type = PacketType::Error;
};

class Packet
{
private:
	PacketHeader _header;
	constexpr static UINT16 _headerSize = sizeof(PacketHeader);
	constexpr static UINT16 _maxSize = 0xffff;

public:
	Packet();
	Packet(const BYTE* data, UINT16 msgSize, PacketType type);

public:
	//static ref<Packet> Create();

public:
	~Packet();

public:
	//bool Write(const BYTE* data, UINT16 size, PacketType type);
	bool Write(ref<RecvBuffer>& recvBuffer);

	UINT16 GetSize() const { return _header.size; }
	UINT16 GetBodySize() const { return _header.size - _headerSize; }
	PacketType GetType() const { return _header.type; }
	BYTE* GetBYTE() const { return _data; }
	BYTE* GetBody() const { return _data + _headerSize; }

	bool IsWriteComplete() const { return _isWriteComplete; }

private:
	BYTE* _data = nullptr;
	UINT16 _cur = 0;
	bool _isWriteComplete = false;
};

