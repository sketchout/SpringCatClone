#include "ProtocolCodecFilter.h"

namespace SpringCat
{
    namespace Filter
    {
        struct ProtocolCodecFilter::Context
        {
            static const char * const filterName;
            static const char * const contextName;
        };

        const char * const ProtocolCodecFilter::Context::filterName = "ProtocolCodecFilter";
        const char * const ProtocolCodecFilter::Context::contextName = "ProtocolCodecFilterContext";

        ProtocolCodecFilter::ProtocolCodecFilter(Common::FilterChain *chain)
            : Common::IFilter(chain, Context::filterName)
        {}

        ProtocolCodecFilter::~ProtocolCodecFilter(void)
        {}

        void ProtocolCodecFilter::OnOpened(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link)
        {
            BaseCat::Network::Filter::DoOnLinkOpened(next, link);
        }

        void ProtocolCodecFilter::OnClosed(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link)
        {
            BaseCat::Network::Filter::DoOnLinkClosed(next, link);
        }

        void ProtocolCodecFilter::OnSend(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link,
            BaseCat::System::SmartHeap::Block buffer, size_t size)
        {
            BaseCat::Network::Filter::DoOnSend(next, link, buffer, size);
        }

        void ProtocolCodecFilter::OnReceived(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link,
            BaseCat::System::SmartHeap::Block buffer, size_t size)
        {
            BaseCat::Network::Filter::DoOnReceived(next, link, buffer, size);
        }
    }
}
