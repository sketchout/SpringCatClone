//
// Copyright (c) 2007 Jaepil Jeong
// http://www.springcat.net/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
