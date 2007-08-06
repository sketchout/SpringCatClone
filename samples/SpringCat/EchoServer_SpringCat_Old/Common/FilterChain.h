#ifndef __SpringCat_Common_FilterChain__
#define __SpringCat_Common_FilterChain__

#include <BaseCat/System.h>
#include <BaseCat/Network.h>

namespace SpringCat
{
    namespace Common
    {
        class IFilter;

        class FilterChain : private BaseCat::System::TL::NonCopyable
        {
        private:
            struct Impl;
            Impl *impl_;

        public:
            FilterChain(void);
            virtual ~FilterChain(void);

            bool Add(const char *name, IFilter *filter);
            bool Remove(const char *name);
            IFilter *Find(const char *name) const;

            BaseCat::Network::FilterChain::Handle GetHandle(void) const;

        public:
            static void *operator new(size_t size);
            static void operator delete(void *p) throw();
        };
    }
}

#endif
