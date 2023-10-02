#include "pch.h"
#include "Packet.h"

Packet::Packet()
{
}

Packet::Packet(const BYTE* data, UINT16 size, PacketType type)
{
	_header.size = size + _headerSize;
	_header.type = type;
	_data = new BYTE[_header.size];
	memcpy(_data, &_header, _headerSize);
	memcpy(_data + _headerSize, data, size);
	_isWriteComplete = true;
}

Packet::~Packet()
{
	delete[] _data;
}

//bool Packet::Write(const BYTE* data, UINT16 size, PacketType type)
//{
//	if (_isWriteComplete)
//		return false;
//
//	if (_header.size) // if header is already written
//		return false;
//
//	_header.size = size;
//	_header.type = type;
//	_body = new BYTE[size];
//	memcpy(_body, data, size);
//	_isWriteComplete = true;
//
//	return true;
//}

bool Packet::Write(ref<RecvBuffer>& recvBuffer)
{
	if (_isWriteComplete)
		return false;

	// check if header is already written
	if(_cur < _headerSize)
	{
		const UINT16 size = recvBuffer->Read(_headerSize - _cur, reinterpret_cast<BYTE*>(&_header) + _cur);
		_cur += size;
		if(_cur < _headerSize)
			return false;

		_data = new BYTE[_header.size];
		memcpy(_data, &_header, _headerSize);
	}

	assert(_cur >= _headerSize);
	assert(_data != nullptr);
	assert(_header.size <= _maxSize);

	const UINT16 size = recvBuffer->Read(_header.size - _cur, _data + _cur);
	_cur += size;
	if(_cur < _header.size)
		return false;

	_isWriteComplete = true;
	return true;
}
