#include "pch.h"
#include "IocpEvent.h"


IocpEvent::IocpEvent(IocpEventType type)
	: type(type), op(nullptr)
{
}

void IocpEvent::Init()
{
	// Initialize Overlapped
	OVERLAPPED::hEvent = nullptr;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}
