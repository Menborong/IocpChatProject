#pragma once

#include "Session.h"
#include "IUser.h"

class ClientSession : public ChatSession
{
public:
	ClientSession(ref<IocpCore>& iocpCore, const std::function<void()>& releaseCallback = nullptr);

	/* functions for application */
	void OnRecv() override;
	void OnSend() override;
	void OnAccept() override;
	//void OnConnect() override;
	void OnDisconnect() override;
	void OnError(int errCode) override;

private:
	ref<IUser> _userInfo = nullptr;

};