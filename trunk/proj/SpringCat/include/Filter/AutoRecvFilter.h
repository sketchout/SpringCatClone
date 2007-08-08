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
            static const char * const FILTER_NAME;

        private:
            struct Context
            {
                static const char * const CONTEXT_NAME;
            };

        private:
            size_t recvBufferSize_;
            size_t recvPendingCount_;

        public:
            AutoRecvFilter(void)
                : Common::IFilter(FILTER_NAME),
                recvBufferSize_(1024), recvPendingCount_(1)
            {}
            AutoRecvFilter(size_t recvBufferSize, size_t recvPendingCount)
                : Common::IFilter(FILTER_NAME),
                recvBufferSize_(recvBufferSize), recvPendingCount_(recvPendingCount)
            {
                if (recvBufferSize_ == 0)
                {
                    throw std::exception();
                }
                if (recvPendingCount_ == 0)
                {
                    throw std::exception();
                }
            }
            virtual ~AutoRecvFilter(void)
            {}

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                for (size_t i = 0; i != recvPendingCount_; ++i)
                {
                    BaseCat::Network::Link::Recv(link, recvBufferSize_);
                }

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
                BaseCat::Network::Link::Recv(link, recvBufferSize_);

                BaseCat::Network::Filter::DoOnReceived(next, link, buffer, size);
            }
        };

        const char * const AutoRecvFilter::FILTER_NAME = "SpringCat::Filter::AutoRecvFilter";
        const char * const AutoRecvFilter::Context::CONTEXT_NAME = "SpringCat::Filter::AutoRecvFilter::Context";
    }
}

#endif
