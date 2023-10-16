#pragma once

/*------------------------------------------------------
 * RecvBuffer
 *
 * Recv() 요청에 의해 수신된 데이터를 저장하는 버퍼
 * !!! Recv buffer is not thread safe
 *
 * Write(UINT32, BYTE*) : from의 데이터를 버퍼에 가져옴
 * Read(UINT32, BYTE*) : 버퍼에 저장된 데이터를 to로 보냄
 * GetWriteBuffer() : 버퍼의 쓰기 위치를 반환
 * GetReadBuffer() : 버퍼의 읽기 위치를 반환
 * GetNextWrite() : 다음 쓰기 가능 길이 반환
 * GetNextRead() : 다음 읽기 가능 길이 반환
---------------------------------------------------------- */


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

