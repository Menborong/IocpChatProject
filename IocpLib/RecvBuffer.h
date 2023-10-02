#pragma once

/*----------------------------------
 * RecvBuffer
 *
 * !!! Recv buffer is not thread safe
 *
------------------------------------ */


class RecvBuffer
{
public:
	RecvBuffer(UINT32 capacity);
	~RecvBuffer();

public:
	UINT32 GetNextWrite() const;
	UINT32 GetNextRead() const;
	UINT32 Write(UINT32 numBytes, BYTE* from);
	UINT32 Read(UINT32 numBytes, BYTE* to);
	void Clear();

	BYTE* GetWriteBuffer() const { return &_buffer[_writePos]; }
	BYTE* GetReadBuffer() const { return &_buffer[_readPos]; }

	bool MoveWritePos(UINT32 numBytes);
	bool MoveReadPos(UINT32 numBytes);

private:
	UINT32 _capacity = 0;
	UINT32 _writePos = 0;
	UINT32 _readPos = 0;
	BYTE* _buffer = nullptr;
};

