#ifndef __SpringCat_Transport_Link__
#define __SpringCat_Transport_Link__

#include <BaseCat/System.h>
#include <BaseCat/Network.h>

namespace SpringCat
{
    namespace Transport
    {
        class Link : private BaseCat::System::TL::NonCopyable
        {
        private:
            BaseCat::Network::Link::Handle link_;

        public:
            Link(BaseCat::Network::Link::Handle link)
                : link_(link)
            {}
            virtual ~Link(void)
            {}

            int AddRef(void)
            {
                return BaseCat::Network::Link::AddRef(link_);
            }
            int Release(void)
            {
                return BaseCat::Network::Link::Release(link_);
            }

            bool Close(void)
            {
                return BaseCat::Network::Link::Close(link_);
            }

            BaseCat::Network::Address::Handle GetLocalAddress(void)
            {
                return BaseCat::Network::Link::GetLocalAddress(link_);
            }
            BaseCat::Network::Address::Handle GetRemoteAddress(void)
            {
                return BaseCat::Network::Link::GetRemoteAddress(link_);
            }

            bool Send(const void *data, size_t len)
            {
                return BaseCat::Network::Link::Send(link_, data, len);
            }
            bool Send(const BaseCat::System::SmartHeap::Block data)
            {
                return BaseCat::Network::Link::Send(link_, data);
            }
            bool Send(const BaseCat::System::SmartHeap::Block data, size_t len)
            {
                return BaseCat::Network::Link::Send(link_, data, len);
            }
            bool Recv(size_t len)
            {
                return BaseCat::Network::Link::Recv(link_, len);
            }

            bool AddAttribute(const char *name, void *data)
            {
                return BaseCat::Network::Link::AddAttribute(link_, name, data);
            }
            bool RemoveAttribute(const char *name)
            {
                return BaseCat::Network::Link::RemoveAttribute(link_, name);
            }
            void *GetAttribute(const char *name)
            {
                return BaseCat::Network::Link::GetAttribute(link_, name);
            }

            BaseCat::System::Stream::Handle LockReadStream(void)
            {
                return BaseCat::Network::Link::LockReadStream(link_);
            }
            void UnlockReadStream(void)
            {
                BaseCat::Network::Link::UnlockReadStream(link_);
            }

            BaseCat::Network::Link::Handle GetHandle(void) const
            {
                return link_;
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

        typedef BaseCat::System::TL::RefCountPtr<Link> LinkPtr;
    }
}

#endif
