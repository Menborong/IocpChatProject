#include "pch.h"
#include "SessionNetOp.h"

SessionNetOp::SessionNetOp(const std::function<void()>& onProcess, const std::function<void(int errCode)>& onError)
	: _onProcess(onProcess), _onError(onError)
{
}