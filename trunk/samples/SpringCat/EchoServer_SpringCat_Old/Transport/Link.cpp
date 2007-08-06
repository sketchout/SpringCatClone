#include "Link.h"

namespace SpringCat
{
    namespace Transport
    {
        Link::Link(BaseCat::Network::Link::Handle link)
            : link_(link)
        {}

        Link::~Link(void)
        {
            // TODO: on destroy
        }

        int Link::AddRef(void)
        {
            return BaseCat::Network::Link::AddRef(link_);
        }

        int Link::Release(void)
        {
            return BaseCat::Network::Link::Release(link_);
        }

        bool Link::Close(void)
        {
            return BaseCat::Network::Link::Close(link_);
        }

        BaseCat::Network::Address::Handle Link::GetLocalAddress(void)
        {
            return BaseCat::Network::Link::GetLocalAddress(link_);
        }

        BaseCat::Network::Address::Handle Link::GetRemoteAddress(void)
        {
            return BaseCat::Network::Link::GetRemoteAddress(link_);
        }

        bool Link::Send(const void *data, size_t len)
        {
            return BaseCat::Network::Link::Send(link_, data, len);
        }

        bool Link::Send(const BaseCat::System::SmartHeap::Block data)
        {
            return BaseCat::Network::Link::Send(link_, data);
        }

        bool Link::Send(const BaseCat::System::SmartHeap::Block data, size_t len)
        {
            return BaseCat::Network::Link::Send(link_, data, len);
        }

        bool Link::Recv(size_t len)
        {
            return BaseCat::Network::Link::Recv(link_, len);
        }

        bool Link::AddAttribute(const char *name, void *data)
        {
            return BaseCat::Network::Link::AddAttribute(link_, name, data);
        }

        bool Link::RemoveAttribute(const char *name)
        {
            return BaseCat::Network::Link::RemoveAttribute(link_, name);
        }

        void *Link::GetAttribute(const char *name)
        {
            return BaseCat::Network::Link::GetAttribute(link_, name);
        }

        BaseCat::System::Stream::Handle Link::LockReadStream(void)
        {
            return BaseCat::Network::Link::LockReadStream(link_);
        }

        void Link::UnlockReadStream(void)
        {
            BaseCat::Network::Link::UnlockReadStream(link_);
        }

        BaseCat::Network::Link::Handle Link::GetHandle(void) const
        {
            return link_;
        }

        // static
        void *Link::operator new(size_t size)
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

        // static
        void Link::operator delete(void *p) throw()
        {
            if (p == NULL)
            {
                return;
            }

            BaseCat::System::MPHeap::Free(NULL, p);
            p = NULL;
        }
    }
}
