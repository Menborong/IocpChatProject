#pragma once
#include "RecvBuffer.h"

/*-----------------------------------------------------------
 * Packet
 *
 * Server <-> Client 간의 통신을 위한 패킷 클래스
 *
 * Write(ref<RecvBuffer>&): 현재 RecvBuffer에 있는 데이터를 패킷에 작성
 * GetSize(): 패킷의 전체 크기 반환
 * GetBodySize(): 패킷의 바디 크기 반환
 * GetType(): 패킷의 타입 반환
 * GetBYTE(): 패킷의 바이트 배열 반환
 * GetBody(): 패킷의 바디 반환
 * IsWriteComplete(): 패킷의 작성이 완료되었는지 여부 반환
 *-----------------------------------------------------------*/

enum class PacketType : UINT16
{
	Chat,
	Error,
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

