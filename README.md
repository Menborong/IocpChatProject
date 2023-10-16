# IocpChatting

## 프로젝트 설명
본 프로젝트는 개인 IOCP 및 OOP 설계 학습을 위한 프로젝트입니다.

해당 프로젝트는 인프런 강의 [C++과 언리얼로 만드는 MMORPG 게임 개발 시리즈]의 코드들을 기반으로 하되, 객체 지향 설계 관점에서 개선하려고 시도했습니다.

* 제작기간: 2023년 10월 ~ 현재

## IocpLib
Server/Client 공용으로 사용되는 IOCP 라이브러리

* Session의 각 Accept/Connect/Disconnect/Send/Receive 업무는 Session에서가 아닌 SessionNetOp(Acceptor/Connector/Disconnector/Sender/Receiver)로 위임
    해당 
* 각 SessionNetOp는 자신을 포함한 IocpEvent 객체를 IOCP에 등록
* I/O 작업이 완료된 Event의 SessionNetOp을 통해 나머지작업(Process) 호출
* SessionNetOp의 작업이 완료되면 콜백함수를 통해 Session에게 알려줌
* 콜백함수에 의해 호출받은 Session은 SessionNetOp 간의 상호작용이 필요한 처리들을 완료함

Q) 콜백 함수를 사용한 이유?

A) IOCP에서의 비동기 처리에 의한 코드 흐름을 동기 처리의 방식과 유사하게 처리하기 위해, 그리고 

    동기 처리: Session 호출 -> SessionNetOp 호출 -> I/O 작업 처리 -> SessionNetOp의 이후 실행 후 리턴 -> Session의 이후 실행 후 리턴
    
    비동기 처리: Session 호출 -> SessionNetOp 호출 -> I/O 작업 요청
                I/O 작업 완료 -> SessionNetOp의 이후 실행 -> Session의 콜백함수 호출, Session의 이후 실행


## TODO
* Echo Server -> Chatting Server로 확장
* Memory Pool 추가를 통한 최적화 (동적 할당에 의한 속도저하 방지, 메모리 파편화 방지)
* 
