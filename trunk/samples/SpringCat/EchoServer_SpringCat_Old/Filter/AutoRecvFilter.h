#ifndef __SpringCat_Filter_AutoRecvFilter__
#define __SpringCat_Filter_AutoRecvFilter__

#include "Common/Filter.h"

namespace SpringCat
{
    namespace Common
    {
        class FilterChain;
    }

    namespace Filter
    {
        class AutoRecvFilter : public Common::IFilter
        {
        private:
            struct Context;

        public:
            AutoRecvFilter(Common::FilterChain *chain);
            ~AutoRecvFilter(void);

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link);
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link);
            virtual void OnSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size);
            virtual void OnReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size);
        };
    }
}

#endif
