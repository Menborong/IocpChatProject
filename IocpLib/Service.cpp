#include "pch.h"
#include "Service.h"
//
//#include "SocketUtils.h"
//#include "Listener.h"
//
//Service::Service(ServiceType type, NetAddress address, SessionFactory factory, UINT32 maxNumSession,
//                 UINT32 initNumSession)
//	:	_address(address), _sessionFactory(factory), _type(type),
//		_maxNumSession(maxNumSession), _initNumSession(initNumSession)
//{
//}
//
//bool Service::IsReady() const
//{
//	if (_sessionFactory == nullptr)
//		return false;
//	if (_numSession != 0)
//		return false;
//	if (_maxNumSession <= 0 || _initNumSession <= 0)
//		return false;
//	if (_maxNumSession < _initNumSession)
//		return false;
//
//	return true;
//}
//
//ref<Session> Service::CreateSession() const
//{
//	if (_sessionFactory == nullptr)
//		return nullptr;
//
//	ref<Session> session = _sessionFactory();
//	return session;
//}
//
//void Service::AddSession(ref<Session> session)
//{
//	std::lock_guard<std::mutex> lock(_sessionsLock);
//	_sessions.insert(session);
//	_numSession++;
//}
//
//void Service::RemoveSession(ref<Session> session)
//{
//	std::lock_guard<std::mutex> lock(_sessionsLock);
//	_numSession--;
//	_sessions.erase(session);
//}
//
//
//ServerService::ServerService(SessionFactory factory, NetAddress address, UINT32 maxNumSession, UINT32 initNumSession)
//	: Service(ServiceType::Server, address, factory, maxNumSession, initNumSession)
//{
//}
//
//bool ServerService::Start()
//{
//	if (IsReady() == false)
//		return false;
//
//	// Ready Listener
//	_listener = std::make_shared<Listener>();
//	if(_listener->Init(std::static_pointer_cast<ServerService>(shared_from_this())) == false)
//	{
//		SocketUtils::ErrorLog("Listener::Init");
//		return false;
//	}
//
//	// Create Sessions
//	for (UINT32 i = 0; i < _initNumSession; i++)
//	{
//		ref<Session> session = CreateSession();
//		if (session == nullptr)
//		{
//			SocketUtils::ErrorLog("CreateSession");
//		}
//		if(session->Accept() == false)
//		{
//			SocketUtils::ErrorLog("Session::Accept");
//			return false;
//		}
//	}
//
//	return true;
//}
//
//void ServerService::Stop()
//{
//	if(_stopping.exchange(true) == true)
//		return;
//
//	_listener->Stop();
//	for(ref<Session> session: _sessions)
//		session->Disconnect();
//}
//
//void ServerService::SetEventFromSession(ref<Session> session, IocpEvent* event)
//{
//	switch(event->type)
//	{
//	case IocpEventType::Accept:
//		_listener->SetAccpetEvent(static_cast<AcceptEvent*>(event));
//		break;
//	default:
//		break;
//	}
//}
//
//void ServerService::ReleaseSession(ref<Session> session)
//{
//	{
//		std::lock_guard<std::mutex> lock(_sessionsLock);
//		_sessions.erase(session);
//	}
//
//	// Service가 종료중이라면 더이상의 세션 생성을 하지 않는다.
//	if(_stopping.load() == true)
//		return;
//
//	// 실제로 Session을 해제하지 않고, 다시 정리해서 재사용
//	session->Reset();
//	if(session->Accept() == false)
//	{
//		SocketUtils::ErrorLog("Session::Accept");
//		return;
//	}
//}
//
//ClientService::ClientService(SessionFactory factory, NetAddress address, UINT32 maxNumSession, UINT32 initNumSession)
//	: Service(ServiceType::Client, address, factory, maxNumSession, initNumSession)
//{
//}
//
//bool ClientService::Start()
//{
//	if(IsReady() == false)
//		return false;
//
//	// Max session 갯수만큼 session 생성
//	for(UINT32 i = 0; i< _maxNumSession; i++)
//	{
//		ref<Session> session = CreateSession();
//		if (session == nullptr)
//		{
//			SocketUtils::ErrorLog("CreateSession");
//		}
//		if (session->Connect() == false)
//		{
//			SocketUtils::ErrorLog("Session::Connect");
//			return false;
//		}
//	}
//
//	return true;
//}
//
//void ClientService::Stop()
//{
//	if (_stopping.exchange(true) == true)
//		return;
//
//	for (ref<Session> session : _sessions)
//		session->Disconnect();
//}
//
//void ClientService::ReleaseSession(ref<Session> session)
//{
//	{
//		std::lock_guard<std::mutex> lock(_sessionsLock);
//		_sessions.erase(session);
//	}
//
//	// Service가 종료중이라면 더이상의 세션 생성을 하지 않는다.
//	if (_stopping.load() == true)
//		return;
//
//	// 실제로 Session을 해제하지 않고, 다시 정리해서 재사용
//	session->Reset();
//	if(session->Connect() == false)
//	{
//		SocketUtils::ErrorLog("Session::Connect");
//		return;
//	}
//}
