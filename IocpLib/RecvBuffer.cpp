#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(UINT32 capacity)
	: _capacity(capacity), _buffer(new BYTE[capacity])
{
}

RecvBuffer::~RecvBuffer()
{
	delete[] _buffer;
}

UINT32 RecvBuffer::GetNextWrite() const
{
	if(_writePos >= _readPos)
		return _capacity - _writePos;
	else
		return _readPos - _writePos;
}

UINT32 RecvBuffer::GetNextRead() const
{
	if(_writePos >= _readPos)
		return _writePos - _readPos;
	else
		return _capacity - _readPos;
}

UINT32 RecvBuffer::Write(UINT32 numBytes)
{
	const UINT32 bytesToWrite = (std::min)(GetNextWrite(), numBytes);
	_writePos += bytesToWrite;
	if(_writePos == _capacity)
		_writePos = 0;

	return bytesToWrite;
}

UINT32 RecvBuffer::Read(UINT32 numBytes)
{
	const UINT32 bytesToRead = (std::min)(GetNextRead(), numBytes);
	_readPos += bytesToRead;
	if(_readPos == _capacity)
		_readPos = 0;

	return bytesToRead;
}

void RecvBuffer::Clear()
{
	_readPos = 0;
	_writePos = 0;
}
