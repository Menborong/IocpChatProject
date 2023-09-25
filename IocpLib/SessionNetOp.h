#pragma once
#include <atomic>
#include "iocpObject.h"

/*-----------------------------------------------------------
 *  SessionNetOp
 *
 *	Session에서의 네트워크 작업에 대한 추상 클래스
 *
 *	Register(): 네트워크 작업을 IOCP에 등록
 *	Process(): 네트워크 작업이 완료되었을 때 호출되는 함수
 *
 *	OnProcess(): 네트워크 작업이 완료되었을 때 호출되는 콜백 함수
 *	OnError(): 네트워크 작업이 실패했을 때 호출되는 콜백 함수
 *-----------------------------------------------------------*/

class SessionNetOp : public std::enable_shared_from_this<SessionNetOp>
{
public:
	SessionNetOp(const std::function<void()>& onProcess, const std::function<void(int errCode)>& onError);
	virtual ~SessionNetOp() = default;
	void SetSocket(SOCKET socket) { _socket = socket; }

	virtual void Register(ref<IocpObject> owner) = 0;
	virtual void Process(bool ret, DWORD numBytes) = 0;

	void OnProcess() const { if(_onProcess) _onProcess(); }
	void OnError(int errCode) const { if(_onError) _onError(errCode); }

	bool IsRunning() const { return _isRunning.load(); }

protected:
	SOCKET _socket = INVALID_SOCKET;
	std::function<void()> _onProcess;
	std::function<void(int errCode)> _onError;
	std::atomic<bool> _isRunning{false};
};
