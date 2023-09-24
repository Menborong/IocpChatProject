#include "pch.h"
#include "SendBuffer.h"

SendBufferChunk::SendBufferChunk(UINT32 bufferSize)
	: _bufferSize(bufferSize), _buffer(new BYTE[bufferSize]), _pos(0), _numRef(0)
{
}

SendBufferChunk::~SendBufferChunk()
{
	delete[] _buffer;
}


BYTE* SendBufferChunk::AllocateBuffer(UINT32 bufferSize)
{
	if(_bufferSize - _pos < bufferSize)
		return nullptr;

	BYTE* ret = _buffer + _pos;
	_pos += bufferSize;
	return ret;
}

void SendBufferChunk::Init()
{
	_bufferSize = 0;
	_pos = 0;
	_numRef = 0;
}

BYTE* SendBufferManager::GetNewBuffer(UINT32 bufferSize)
{
	// !!!!!: buffersize must be less than initial chunk size
	assert(bufferSize <= CHUNK_SIZE);

	BYTE* ret = nullptr;
	std::lock_guard<std::mutex> lock(_mutex);

	if(_bufferQueue.empty() == false)
		ret = _bufferQueue.front()->AllocateBuffer(bufferSize);

	if(ret == nullptr)
	{
		if(!_bufferQueue.empty())
			_bufferQueue.pop();
		if (_bufferQueue.empty())
		{
			_bufferQueue.push(new SendBufferChunk(CHUNK_SIZE));
			AddRef(_bufferQueue.front());
		}

		ret = _bufferQueue.front()->AllocateBuffer(bufferSize);
	}

	AddRef(_bufferQueue.front());
	_bufferMap[ret] = _bufferQueue.front();
	return ret;
}

void SendBufferManager::ReturnBuffer(BYTE* buffer)
{
	std::lock_guard<std::mutex> lock(_mutex);
	SubRef(_bufferMap[buffer]);
	_bufferMap.erase(buffer);
}

void SendBufferManager::AddRef(SendBufferChunk* chunk)
{
	chunk->AddRef();
}

void SendBufferManager::SubRef(SendBufferChunk* chunk)
{
	chunk->SubRef();
	if(chunk->GetNumRef() == 0)
		ReleaseChunk(chunk);
}

void SendBufferManager::ReleaseChunk(SendBufferChunk* chunk)
{
	chunk->Init();
	_bufferQueue.push(chunk);
}


SendBuffer::SendBuffer(UINT32 bufferSize)
	: _bufferSize(bufferSize)
{
	_buffer = SendBufferManager::GetInstance().GetNewBuffer(_bufferSize);
}

SendBuffer::~SendBuffer()
{
	SendBufferManager::GetInstance().ReturnBuffer(_buffer);
}

bool SendBuffer::Write(const void* data, UINT32 dataSize)
{
	if(_bufferSize - _writeSize < dataSize)
		return false;

	::memcpy(_buffer + _writeSize, data, dataSize);
	_writeSize += dataSize;
	return true;
}
