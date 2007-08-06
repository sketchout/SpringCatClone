#include "AutoRecvFilter.h"

namespace SpringCat
{
    namespace Filter
    {
        struct AutoRecvFilter::Context
        {
            static const char * const filterName;
            static const char * const contextName;
        };

        const char * const AutoRecvFilter::Context::filterName = "AutoRecvFilter";
        const char * const AutoRecvFilter::Context::contextName = "AutoRecvFilterContext";

        AutoRecvFilter::AutoRecvFilter(Common::FilterChain *chain)
            : Common::IFilter(chain, Context::filterName)
        {}

        AutoRecvFilter::~AutoRecvFilter(void)
        {}

        void AutoRecvFilter::OnOpened(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link)
        {
            BaseCat::Network::Link::Recv(link, 1024);
            BaseCat::Network::Link::Recv(link, 1024);

            BaseCat::Network::Filter::DoOnLinkOpened(next, link);
        }

        void AutoRecvFilter::OnClosed(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link)
        {
            BaseCat::Network::Filter::DoOnLinkClosed(next, link);
        }

        void AutoRecvFilter::OnSend(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link,
            BaseCat::System::SmartHeap::Block buffer, size_t size)
        {
            BaseCat::Network::Filter::DoOnSend(next, link, buffer, size);
        }

        void AutoRecvFilter::OnReceived(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link,
            BaseCat::System::SmartHeap::Block buffer, size_t size)
        {
            BaseCat::Network::Link::Recv(link, 1024);

            BaseCat::Network::Filter::DoOnReceived(next, link, buffer, size);
        }
    }
}
