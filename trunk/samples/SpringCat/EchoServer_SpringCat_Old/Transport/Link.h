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
            Link(BaseCat::Network::Link::Handle link);
            virtual ~Link(void);

            int AddRef(void);
            int Release(void);

            bool Close(void);

            BaseCat::Network::Address::Handle GetLocalAddress(void);
            BaseCat::Network::Address::Handle GetRemoteAddress(void);

            bool Send(const void *data, size_t len);
            bool Send(const BaseCat::System::SmartHeap::Block data);
            bool Send(const BaseCat::System::SmartHeap::Block data, size_t len);
            bool Recv(size_t len);

            bool AddAttribute(const char *name, void *data);
            bool RemoveAttribute(const char *name);
            void *GetAttribute(const char *name);

            BaseCat::System::Stream::Handle LockReadStream(void);
            void UnlockReadStream(void);

            BaseCat::Network::Link::Handle GetHandle(void) const;

        public:
            static void *operator new(size_t size);
            static void operator delete(void *p) throw();
        };

        typedef BaseCat::System::TL::RefCountPtr<Link> LinkPtr;
    }
}

#endif
