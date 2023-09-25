#include "pch.h"
#include "IocpEvent.h"


IocpEvent::IocpEvent(IocpEventType type)
	: type(type), op(nullptr)
{
	Clear();
}

void IocpEvent::Clear()
{
	// Clear Overlapped
	OVERLAPPED::hEvent = nullptr;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;

	// Clear IocpEvent
	op = nullptr;
	owner = nullptr;
}
