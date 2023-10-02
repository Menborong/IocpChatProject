//#pragma once
//
//
//
//class SendBufferChunk
//{
//public:
//	SendBufferChunk(UINT32 bufferSize);
//	virtual ~SendBufferChunk();
//	BYTE* AllocateBuffer(UINT32 bufferSize);
//	void Init();
//	void AddRef() { _numRef++; }
//	void SubRef() { _numRef--; }
//	UINT32 GetNumRef() const { return _numRef; }
//private:
//	UINT32 _bufferSize = 0;
//	BYTE* _buffer = nullptr;
//	UINT32 _pos = 0;
//	UINT32 _numRef = 0;
//
//
//};
//
//
///*-----------------------------------------------------------------
// * SendBufferManager
// *
// * A class that manages the creation and reuse of SendBufferChunks.
// *
// * Implemented using the Singleton Pattern to accommodate
// * potential usage across multiple services.
// *
// * This class is thread-safe.
// *
//-------------------------------------------------------------------- */
//
//class SendBufferManager
//{
//	// Meyers Singleton
//protected:
//	SendBufferManager() = default;
//	virtual ~SendBufferManager() = default;
//
//public:
//	SendBufferManager(const SendBufferManager&) = delete;
//	SendBufferManager& operator=(const SendBufferManager&) = delete;
//
//	static SendBufferManager& GetInstance()
//	{
//		static SendBufferManager instance;
//		return instance;
//	}
//
//public:
//	enum { CHUNK_SIZE = 0x10000 };
//
//	BYTE* GetNewBuffer(UINT32 bufferSize);
//	void ReturnBuffer(BYTE* buffer);
//
//private:
//	void AddRef(SendBufferChunk* chunk);
//	void SubRef(SendBufferChunk* chunk);
//	void ReleaseChunk(SendBufferChunk* chunk);
//
//private:
//	std::mutex _mutex;
//	std::queue<SendBufferChunk*> _bufferQueue;
//	std::map<BYTE*, SendBufferChunk*> _bufferMap;
//};
//
//
//
//class SendBuffer
//{
//public:
//	SendBuffer(UINT32 bufferSize);
//	virtual ~SendBuffer();
//
//	BYTE* GetBuffer() const { return _buffer; }
//	bool Write(const void* data, UINT32 dataSize);
//	UINT32 GetWriteSize() const { return _writeSize; }
//
//private:
//	BYTE* _buffer = nullptr;
//	UINT32 _bufferSize = 0;
//	UINT32 _writeSize = 0;
//};
