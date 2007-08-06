//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Transport_Acceptor__
#define __SpringCat_Transport_Acceptor__

#include <SpringCat/Transport/Link.h>
#include <SpringCat/Transport/DefaultLinkFactory.h>
#include <SpringCat/Common/IOHandler.h>

namespace SpringCat
{
    namespace Common
    {
        class ThreadPool;
        class FilterChain;
    }

    namespace Transport
    {
        template<typename LinkT = Link,
            typename IOHandlerT = Common::IIOHandler<LinkT>,
            typename LinkFactoryT = DefaultLinkFactory<LinkT>
        >
        class Acceptor : private BaseCat::System::TL::NonCopyable
        {
        private:
            BaseCat::Network::Acceptor::Handle handle_;
            IOHandlerT *handler_;

        public:
            Acceptor(Common::ThreadPool *threadPool)
                : handle_(BaseCat::Network::Acceptor::CreateHandle(
                    threadPool->GetHandle())), handler_(NULL)
            {
                if (handle_ == NULL)
                {
                    throw std::bad_alloc();
                }

                BaseCat::Network::Acceptor::SetOnAccepted(handle_, &OnAccepted, this);
                BaseCat::Network::Acceptor::SetOnReceived(handle_, &OnReceived, this);
                BaseCat::Network::Acceptor::SetOnClosed(handle_, &OnClosed, this);
            }
            Acceptor(Common::ThreadPool *threadPool, Common::FilterChain *chain)
                : handle_(BaseCat::Network::Acceptor::CreateHandle(
                    threadPool->GetHandle(), chain->GetHandle())), handler_(NULL)
            {
                if (handle_ == NULL)
                {
                    throw std::bad_alloc();
                }

                BaseCat::Network::Acceptor::SetOnAccepted(handle_, &OnAccepted, this);
                BaseCat::Network::Acceptor::SetOnReceived(handle_, &OnReceived, this);
                BaseCat::Network::Acceptor::SetOnClosed(handle_, &OnClosed, this);
            }
            ~Acceptor(void)
            {
                BaseCat::Network::Acceptor::DestroyHandle(handle_);
                handle_ = NULL;
            }

            bool Start(void)
            {
                return BaseCat::Network::Acceptor::Start(GetHandle());
            }
            bool Stop(void)
            {
                return BaseCat::Network::Acceptor::Stop(GetHandle());
            }

            bool SetDefaultPort(unsigned short port)
            {
                return BaseCat::Network::Acceptor::SetDefaultPort(GetHandle(), port);
            }
            bool SetDefaultPort(unsigned short port, bool reuse)
            {
                return BaseCat::Network::Acceptor::SetDefaultPort(GetHandle(), port, reuse);
            }
            unsigned short GetDefaultPort(void) const
            {
                return BaseCat::Network::Acceptor::GetDefaultPort(GetHandle());
            }

            bool AddBinding(const char *ipAddress, unsigned short port)
            {
                return BaseCat::Network::Acceptor::AddBinding(GetHandle(), ipAddress, port);
            }
            bool AddBinding(const char *ipAddress, unsigned short port, bool reuse)
            {
                return BaseCat::Network::Acceptor::AddBinding(GetHandle(), ipAddress, port, reuse);
            }
            bool RemoveBinding(const char *ipAddress, unsigned short port)
            {
                return BaseCat::Network::Acceptor::RemoveBinding(GetHandle(), ipAddress, port);
            }
            bool ClearBindings(void)
            {
                return BaseCat::Network::Acceptor::ClearBindings(GetHandle());
            }

            //ActiveLinkList::Handle LockActiveLinks(void);
            //bool UnlockActiveLinks(void);

            void SetHandler(IOHandlerT *handler)
            {
                handler_ = handler;
            }

            BaseCat::Network::Acceptor::Handle GetHandle(void) const
            {
                return handle_;
            }

        private:
            static void __stdcall OnLinkDestroy(BaseCat::Network::Link::Handle link, void *)
            {
                LinkT *linkPtr = static_cast<LinkT *>(BaseCat::Network::Link::GetData(link));
                LinkFactoryT::Free(linkPtr);
                linkPtr = NULL;
            }
            static void __stdcall OnAccepted(BaseCat::Network::Acceptor::Handle,
                BaseCat::Network::Link::Handle link, void *data)
            {
                Acceptor *acceptor = static_cast<Acceptor *>(data);
                if (acceptor->handler_ != NULL)
                {
                    LinkT *linkPtr = LinkFactoryT::Alloc(link);
                    BaseCat::Network::Link::SetData(link, linkPtr, &OnLinkDestroy, acceptor);
                    acceptor->handler_->OnOpened(linkPtr);
                }
                else
                {
                    // TODO: error
                }
            }
            static void __stdcall OnReceived(BaseCat::Network::Acceptor::Handle,
                BaseCat::Network::Link::Handle link, void *data)
            {
                Acceptor *acceptor = static_cast<Acceptor *>(data);
                if (acceptor->handler_ != NULL)
                {
                    LinkT *linkPtr = static_cast<LinkT *>(BaseCat::Network::Link::GetData(link));
                    acceptor->handler_->OnReceived(linkPtr);
                }
                else
                {
                    // TODO: error
                }
            }
            static void __stdcall OnClosed(BaseCat::Network::Acceptor::Handle,
                BaseCat::Network::Link::Handle link, void *data)
            {
                Acceptor *acceptor = static_cast<Acceptor *>(data);
                if (acceptor->handler_ != NULL)
                {
                    LinkT *linkPtr = static_cast<LinkT *>(BaseCat::Network::Link::GetData(link));
                    acceptor->handler_->OnClosed(linkPtr);
                }
                else
                {
                    // TODO: error
                }
            }

        public:
            static void *operator new(size_t size)
            {
                if (size == 0)
                {
                    size = 1;
                }

                void *result = NULL;

#pragma warning(disable:4127)
                while (true)
#pragma warning(default:4127)
                {
                    result = BaseCat::System::MPHeap::Alloc(NULL, size);
                    if (result != NULL)
                    {
                        break;
                    }

                    std::new_handler newHandler = std::set_new_handler(0);
                    std::set_new_handler(newHandler);

                    if (newHandler != NULL)
                    {
                        (*newHandler)();
                    }
                    else
                    {
                        throw std::bad_alloc();
                    }
                }

                return result;
            }
            static void operator delete(void *p) throw()
            {
                if (p == NULL)
                {
                    return;
                }

                BaseCat::System::MPHeap::Free(NULL, p);
                p = NULL;
            }
        };
    }
}

#endif
