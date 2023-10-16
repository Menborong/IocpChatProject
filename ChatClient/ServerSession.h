#pragma once
#include "Session.h"

class ServerSession : public ChatSession
{
public:
	ServerSession(ref<IocpCore>& iocpCore, const std::function<void()>& releaseCallback = nullptr)
		: ChatSession(iocpCore, releaseCallback)
	{}

	/* functions for application */
	void OnRecv() override;
	void OnSend() override;
	//void OnAccept() override;
	void OnConnect() override;
	void OnDisconnect() override;
	void OnError(int errCode) override;
};