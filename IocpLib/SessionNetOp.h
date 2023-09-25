#pragma once
#include <atomic>
#include "iocpObject.h"

/*-----------------------------------------------------------
 *  SessionNetOp
 *
 *	Session������ ��Ʈ��ũ �۾��� ���� �߻� Ŭ����
 *
 *	Register(): ��Ʈ��ũ �۾��� IOCP�� ���
 *	Process(): ��Ʈ��ũ �۾��� �Ϸ�Ǿ��� �� ȣ��Ǵ� �Լ�
 *
 *	OnProcess(): ��Ʈ��ũ �۾��� �Ϸ�Ǿ��� �� ȣ��Ǵ� �ݹ� �Լ�
 *	OnError(): ��Ʈ��ũ �۾��� �������� �� ȣ��Ǵ� �ݹ� �Լ�
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
