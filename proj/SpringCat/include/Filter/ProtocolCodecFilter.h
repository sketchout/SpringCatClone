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

#ifndef __SpringCat_Filter_ProtocolCodecFilter__
#define __SpringCat_Filter_ProtocolCodecFilter__

#include <SpringCat/Common/Filter.h>

namespace SpringCat
{
    namespace Filter
    {
        class ProtocolCodecFilter : public Common::IFilter
        {
        private:
            static const char * const FILTER_NAME;

        private:
            struct Context
            {
                static const char * const CONTEXT_NAME;
            };

        public:
            ProtocolCodecFilter(void)
                : Common::IFilter(FILTER_NAME)
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

        const char * const ProtocolCodecFilter::FILTER_NAME = "SpringCat::Filter::ProtocolCodecFilter";
        const char * const ProtocolCodecFilter::Context::CONTEXT_NAME = "SpringCat::Filter::ProtocolCodecFilter::Context";
    }
}

#endif
