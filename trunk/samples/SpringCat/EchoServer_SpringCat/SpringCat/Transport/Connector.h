//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Transport_Connector__
#define __SpringCat_Transport_Connector__

#include <SpringCat/Common/SmallObject.h>
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
        class Connector : public Common::SmallObject<Connector<LinkT, IOHandlerT, LinkFactoryT> >
        {
        private:
            BaseCat::Network::Connector::Handle handle_;
            IOHandlerT *handler_;

        public:
            Connector(Common::ThreadPool *threadPool)
                : handle_(BaseCat::Network::Connector::CreateHandle(
                threadPool->GetHandle())), handler_(NULL)
            {
                if (handle_ == NULL)
                {
                    throw std::bad_alloc();
                }

                BaseCat::Network::Connector::SetOnConnected(handle_, &OnConnected, this);
                BaseCat::Network::Connector::SetOnConnectedEx(handle_, &OnConnectedEx, this);
                BaseCat::Network::Connector::SetOnReceived(handle_, &OnReceived, this);
                BaseCat::Network::Connector::SetOnDisconnected(handle_, &OnDisconnected, this);
            }
            Connector(Common::ThreadPool *threadPool, Common::FilterChain *chain)
                : handle_(BaseCat::Network::Connector::CreateHandle(
                threadPool->GetHandle(), chain->GetHandle())), handler_(NULL)
            {
                if (handle_ == NULL)
                {
                    throw std::bad_alloc();
                }

                BaseCat::Network::Connector::SetOnConnected(handle_, &OnConnected, this);
                BaseCat::Network::Connector::SetOnConnectedEx(handle_, &OnConnectedEx, this);
                BaseCat::Network::Connector::SetOnReceived(handle_, &OnReceived, this);
                BaseCat::Network::Connector::SetOnDisconnected(handle_, &OnDisconnected, this);
            }
            ~Connector(void)
            {
                BaseCat::Network::Connector::DestroyHandle(handle_);
                handle_ = NULL;
            }

            bool Connect(const char *ipAddress, unsigned short port)
            {
                return BaseCat::Network::Connector::Connect(GetHandle(), ipAddress, port);
            }
            bool Connect(const char *ipAddress, unsigned short port, int timeout)
            {
                return BaseCat::Network::Connector::Connect(GetHandle(), ipAddress, port, timeout);
            }

            bool ConnectEx(const char *ipAddress, unsigned short port, void *context)
            {
                return BaseCat::Network::Connector::ConnectEx(GetHandle(), ipAddress, port, context);
            }
            bool ConnectEx(const char *ipAddress, unsigned short port, int timeout, void *context)
            {
                return BaseCat::Network::Connector::ConnectEx(GetHandle(), ipAddress, port, timeout, context);
            }

            //ActiveLinkList::Handle LockActiveLinks(void);
            //bool UnlockActiveLinks(void);

            void SetHandler(IOHandlerT *handler)
            {
                handler_ = handler;
            }

            BaseCat::Network::Connector::Handle GetHandle(void) const
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
            static void __stdcall OnConnected(BaseCat::Network::Connector::Handle,
                BaseCat::Network::Link::Handle link, void *data)
            {
                Connector *connector = static_cast<Connector *>(data);
                if (connector->handler_ != NULL)
                {
                    LinkT *linkPtr = LinkFactoryT::Alloc(link);
                    BaseCat::Network::Link::SetData(link, linkPtr, &OnLinkDestroy, NULL);
                    connector->handler_->OnOpened(linkPtr);
                }
                else
                {
                    // TODO: error
                }
            }
            static void __stdcall OnConnectedEx(BaseCat::Network::Connector::Handle,
                BaseCat::Network::Link::Handle link, void *data, void *context)
            {
                Connector *connector = static_cast<Connector *>(data);
                if (connector->handler_ != NULL)
                {
                    connector->handler_->OnOpened(link, context);
                }
                else
                {
                    // TODO: error
                }
            }
            static void __stdcall OnReceived(BaseCat::Network::Connector::Handle,
                BaseCat::Network::Link::Handle link, void *data)
            {
                Connector *connector = static_cast<Connector *>(data);
                if (connector->handler_ != NULL)
                {
                    LinkT *linkPtr = static_cast<LinkT *>(BaseCat::Network::Link::GetData(link));
                    connector->handler_->OnReceived(linkPtr);
                }
                else
                {
                    // TODO: error
                }
            }
            static void __stdcall OnDisconnected(BaseCat::Network::Connector::Handle,
                BaseCat::Network::Link::Handle link, void *data)
            {
                Connector *connector = static_cast<Connector *>(data);
                if (connector->handler_ != NULL)
                {
                    LinkT *linkPtr = static_cast<LinkT *>(BaseCat::Network::Link::GetData(link));
                    connector->handler_->OnClosed(linkPtr);
                }
                else
                {
                    // TODO: error
                }
            }
        };
    }
}

#endif
