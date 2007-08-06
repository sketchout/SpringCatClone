#ifndef __EchoServer_Link__
#define __EchoServer_Link__

#include <BaseCat/System.h>
#include <BaseCat/Network.h>
#include <string>

namespace EchoServer
{
    struct LinkContext
    {
        std::string ip_;
        unsigned short port_;

        LinkContext(const std::string &ip, unsigned short port)
            : ip_(ip), port_(port)
        {}

        // static
        static void *LinkContext::operator new(size_t size)
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
        static void LinkContext::operator delete(void *p) throw()
        {
            if (p == NULL)
            {
                return;
            }

            BaseCat::System::MPHeap::Free(NULL, p);
            p = NULL;
        }
    };

    class Link
    {
    private:
        BaseCat::Network::Link::Handle link_;

    public:
        Link(void)
            : link_(NULL)
        {}
        Link(BaseCat::Network::Link::Handle link)
            : link_(link)
        {
            BaseCat::Network::Link::AddRef(link_);
        }
        Link(const Link &rhs)
            : link_(rhs.link_)
        {
            BaseCat::Network::Link::AddRef(link_);
        }
        ~Link(void)
        {
            BaseCat::Network::Link::Release(link_);
            link_ = NULL;
        }

        BaseCat::Network::Link::Handle GetHandle(void) const
        {
            return link_;
        }

        void Close(void)
        {
            BaseCat::Network::Link::Close(link_);
        }
        LinkContext *GetContext(void) const
        {
            return static_cast<LinkContext *>(BaseCat::Network::Link::GetData(link_));
        }

        bool Send(void *data, size_t size)
        {
            return BaseCat::Network::Link::Send(link_, data, size);
        }

        bool Send(BaseCat::System::SmartHeap::Block data)
        {
            return BaseCat::Network::Link::Send(link_, data);
        }

        Link &operator=(const Link &rhs)
        {
            Link tmp(rhs);
            std::swap(link_, tmp.link_);

            return *this;
        }
    };
}

#endif
