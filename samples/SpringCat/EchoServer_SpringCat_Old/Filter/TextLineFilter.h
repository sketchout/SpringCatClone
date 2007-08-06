#ifndef __SpringCat_Filter_TextLineFilter__
#define __SpringCat_Filter_TextLineFilter__

#include "Common/Filter.h"

namespace SpringCat
{
    namespace Common
    {
        class FilterChain;
    }

    namespace Filter
    {
        class TextLineFilter : public Common::IFilter
        {
        private:
            struct Context;

        public:
            TextLineFilter(Common::FilterChain *chain);
            ~TextLineFilter(void);

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
