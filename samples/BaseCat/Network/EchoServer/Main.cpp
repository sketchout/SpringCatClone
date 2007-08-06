//
// Network 예제
//      EchoServer: Acceptor를 이용한 TCP Echo Server
//

#include <BaseCat/System.h>
#include <BaseCat/Network.h>
#include <cstdio>

#include "Link.h"

using namespace BaseCat;
using namespace BaseCat::System;

namespace EchoServer
{
    Threading::CriticalSection cs;

    void __stdcall OnLinkDestroy(Network::Link::Handle link, void *)
    {
        LinkContext *context = static_cast<LinkContext *>(Network::Link::GetData(link));
        delete context;
    }

    void __stdcall OnAccepted(Network::Acceptor::Handle, Network::Link::Handle link, void *)
    {
        char ip[100];
        unsigned short port = 0;

        Network::Address::Handle address = Network::Link::GetRemoteAddress(link);
        Network::Address::GetIPAddress(address, ip, sizeof(ip) / sizeof(ip[0]));
        Network::Address::GetPort(address, &port);

        {
            Threading::ScopedLock<Threading::CriticalSection> lock(cs);
            printf_s("OnAccepted - %s:%d\n", ip, port);
        }


        Network::Link::SetData(link, new LinkContext(ip, port), &OnLinkDestroy, NULL);

        // 수신 요청: 여러 번 요청할 수 있다. 수신 순서 보장함.
        Network::Link::Recv(link, 1024);
        Network::Link::Recv(link, 1024);
    }

    void __stdcall OnReceived(Network::Acceptor::Handle, Network::Link::Handle link, void *)
    {
        Link tmpLink(link);

        LinkContext *context = tmpLink.GetContext();

        {
            Threading::ScopedLock<Threading::CriticalSection> lock(cs);
            printf_s("OnReceived - %s:%d\n", context->ip_.c_str(), context->port_);
        }

        // echo: SmartHeap::Block을 이용한 send는 zero-copy 구현되어 있음
        Stream::Handle stream = Network::Link::LockReadStream(link);
        SmartHeap::Block p = Stream::Detach(stream);
        tmpLink.Send(p);
        SmartHeap::Free(p);
        Network::Link::UnlockReadStream(link);

        // 수신 요청: OnReceived 한 번에 Recv 한 번만 요청한다.
        Network::Link::Recv(link, 1024);
    }

    void __stdcall OnClosed(Network::Acceptor::Handle, Network::Link::Handle link, void *)
    {
        Link tmpLink(link);

        LinkContext *context = tmpLink.GetContext();

        {
            Threading::ScopedLock<Threading::CriticalSection> lock(cs);
            printf_s("OnClosed - %s:%d\n", context->ip_.c_str(), context->port_);
        }
    }
}

int main(void)
{
    // 소켓 I/O에 사용할 스레드풀 생성
    ThreadPool::Handle threadPool = ThreadPool::CreateHandle(Platform::GetProcessorCount() + 3);

    // Acceptor 생성
    Network::Acceptor::Handle acceptor = Network::Acceptor::CreateHandle(threadPool);
    Network::Acceptor::SetOnAccepted(acceptor, &EchoServer::OnAccepted, NULL);
    Network::Acceptor::SetOnReceived(acceptor, &EchoServer::OnReceived, NULL);
    Network::Acceptor::SetOnClosed(acceptor, &EchoServer::OnClosed, NULL);
    Network::Acceptor::SetDefaultPort(acceptor, 9999);
    Network::Acceptor::Start(acceptor);

    printf_s("acceptor started\n");

    getchar();

    Network::Acceptor::DestroyHandle(acceptor);
    acceptor = NULL;

    printf_s("acceptor stopped\n");

    ThreadPool::DestroyHandle(threadPool);
    threadPool = NULL;

    return 0;
}
