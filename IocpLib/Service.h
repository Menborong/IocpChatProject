//#pragma once
//#include "IocpCore.h"
//#include "NetAddress.h"
//#include "Session.h"
//
//class Listener;
//// Session factory
//using SessionFactory = std::function<ref<Session>()>;
//
//enum class ServiceType
//{
//	Server,
//	Client,
//};
//
//class Service : public std::enable_shared_from_this<Service>
//{
//public:
//	Service(ServiceType type, NetAddress address, SessionFactory factory, UINT32 maxNumSession = 1, UINT32 initNumSession = 1);
//	virtual ~Service() = default;
//
//public:
//	//HANDLE GetIocpHandle() const { return _iocpCore->GetHandle(); }
//	ref<IocpCore> GetIocpCore() const { return _iocpCore; }
//	NetAddress GetAddress() const { return _address; }
//
//public:
//	// Start / Stop the service
//	bool IsReady() const;
//	virtual bool Start() = 0;
//	virtual void Stop() = 0;
//
//public:
//	// Session management
//	void SetSessionFactory(SessionFactory sessionFactory) { _sessionFactory = sessionFactory; }
//	ref<Session> CreateSession() const;
//
//	void AddSession(ref<Session> session);
//	virtual void ReleaseSession(ref<Session> session) = 0;
//	void RemoveSession(ref<Session> session);
//
//	virtual void SetEventFromSession(ref<Session> session, IocpEvent* event) {}
//
//
//protected:
//	/* Service Status */
//	std::atomic<bool> _stopping{ false };
//	bool _isRunning = false;
//	ref<IocpCore> _iocpCore = nullptr;
//	NetAddress _address;
//
//protected:
//	// Session management
//	SessionFactory _sessionFactory = nullptr;
//	std::set<ref<Session>> _sessions;
//	std::mutex _sessionsLock;
//
//protected:
//	ServiceType _type;
//	UINT32 _maxNumSession = 0;
//	UINT32 _initNumSession = 0;
//	UINT32 _numSession = 0;
//
//protected:
//
//};
//
//
//class ServerService : public Service
//{
//public:
//	ServerService(SessionFactory factory, NetAddress address, UINT32 maxNumSession = 1, UINT32 initNumSession = 1);
//
//public:
//	// Override
//	bool Start() override;
//	void Stop() override;
//	void SetEventFromSession(ref<Session> session, IocpEvent* event) override;
//	void ReleaseSession(ref<Session> session) override;
//
//private:
//	ref<Listener> _listener = nullptr;
//};
//
//class ClientService : public Service
//{
//public:
//	ClientService(SessionFactory factory, NetAddress address, UINT32 maxNumSession = 1, UINT32 initNumSession = 1);
//	bool Start() override;
//	void Stop() override;
//	void ReleaseSession(ref<Session> session) override;
//};