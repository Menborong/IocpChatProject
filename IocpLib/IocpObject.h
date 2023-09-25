#pragma once

class IocpObject: public std::enable_shared_from_this<IocpObject>
{
public:
	IocpObject() = default;
	virtual ~IocpObject() = default;

	ref<IocpObject> GetRef() { return shared_from_this(); }
};

