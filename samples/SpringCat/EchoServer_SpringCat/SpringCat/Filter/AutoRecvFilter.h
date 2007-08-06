//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Filter_AutoRecvFilter__
#define __SpringCat_Filter_AutoRecvFilter__

#include <SpringCat/Common/Filter.h>

namespace SpringCat
{
    namespace Filter
    {
        class AutoRecvFilter : public Common::IFilter
        {
        private:
            struct Context
            {
                static const char * const filterName;
                static const char * const contextName;
            };

        public:
            AutoRecvFilter(void)
                : Common::IFilter(Context::filterName)
            {}
            virtual ~AutoRecvFilter(void)
            {}

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                BaseCat::Network::Link::Recv(link, 1024);
                BaseCat::Network::Link::Recv(link, 1024);

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
                BaseCat::Network::Link::Recv(link, 1024);

                BaseCat::Network::Filter::DoOnReceived(next, link, buffer, size);
            }
        };

        const char * const AutoRecvFilter::Context::filterName = "AutoRecvFilter";
        const char * const AutoRecvFilter::Context::contextName = "AutoRecvFilterContext";
    }
}

#endif
