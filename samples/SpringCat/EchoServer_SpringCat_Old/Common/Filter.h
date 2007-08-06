#ifndef __SpringCat_Common_Filter__
#define __SpringCat_Common_Filter__

#include <BaseCat/System.h>
#include <BaseCat/Network.h>

namespace SpringCat
{
    namespace Common
    {
        class FilterChain;

        class IFilter : private BaseCat::System::TL::NonCopyable
        {
        private:
            struct Impl;
            Impl *impl_;

        public:
            IFilter(FilterChain *chain, const char *name);
            virtual ~IFilter(void);

            BaseCat::Network::Filter::Handle GetHandle(void) const;

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link) = 0;
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link) = 0;
            virtual void OnSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size) = 0;
            virtual void OnReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size) = 0;

        public:
            static void *operator new(size_t size);
            static void operator delete(void *p) throw();
        };
    }
}

#endif
