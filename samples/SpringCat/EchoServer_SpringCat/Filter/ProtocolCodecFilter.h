#ifndef __SpringCat_Filter_ProtocolCodecFilter__
#define __SpringCat_Filter_ProtocolCodecFilter__

#include "Common/Filter.h"

namespace SpringCat
{
    namespace Filter
    {
        class ProtocolCodecFilter : public Common::IFilter
        {
        private:
            struct Context
            {
                static const char * const filterName;
                static const char * const contextName;
            };

        public:
            ProtocolCodecFilter(void)
                : Common::IFilter(Context::filterName)
            {}
            virtual ~ProtocolCodecFilter(void)
            {}

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                BaseCat::Network::Filter::DoOnLinkOpened(next, link);
            }
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                BaseCat::Network::Filter::DoOnLinkClosed(next, link);
            }
            virtual void OnSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size)
            {
                BaseCat::Network::Filter::DoOnSend(next, link, buffer, size);
            }
            virtual void OnReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size)
            {
                BaseCat::Network::Filter::DoOnReceived(next, link, buffer, size);
            }
        };

        const char * const ProtocolCodecFilter::Context::filterName = "ProtocolCodecFilter";
        const char * const ProtocolCodecFilter::Context::contextName = "ProtocolCodecFilterContext";
    }
}

#endif
