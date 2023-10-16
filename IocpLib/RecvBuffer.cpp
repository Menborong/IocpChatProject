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

UINT32 RecvBuffer::Write(UINT32 numBytes, BYTE* from)
{
	UINT32 bytesWritten = 0;
	while(bytesWritten < numBytes)
	{
		//const UINT32 available = GetNextWrite();
		const UINT32 available = min(GetNextWrite(), numBytes - bytesWritten);
		if(available == 0)
			break;
		memcpy(&_buffer[_writePos], &from[bytesWritten], available);
		bytesWritten += available;
		_writePos += available;
		if(_writePos == _capacity) // Circular buffer
			_writePos = 0;
	}

	return bytesWritten;
}

UINT32 RecvBuffer::Read(UINT32 numBytes, BYTE* to)
{
	UINT32 bytesRead = 0;
	while(bytesRead < numBytes)
	{
		//const UINT32 available = GetNextRead();
		const UINT32 available = min(GetNextRead(), numBytes - bytesRead);
		if(available == 0)
			break;
		memcpy(&to[bytesRead], &_buffer[_readPos], available);
		bytesRead += available;
		_readPos += available;
		if(_readPos == _capacity) // Circular buffer
			_readPos = 0;
	}

	return bytesRead;	
}

void RecvBuffer::Clear()
{
	_readPos = 0;
	_writePos = 0;
}

bool RecvBuffer::MoveWritePos(UINT32 numBytes)
{
	if(numBytes > GetNextWrite())
	return false;

	_writePos += numBytes;
	if(_writePos == _capacity) // Circular buffer
		_writePos = 0;

	return true;
}

bool RecvBuffer::MoveReadPos(UINT32 numBytes)
{
	if(numBytes > GetNextRead())
	return false;

	_readPos += numBytes;
	if(_readPos == _capacity) // Circular buffer
		_readPos = 0;

	return true;
}
