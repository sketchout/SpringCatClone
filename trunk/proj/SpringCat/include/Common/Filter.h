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

#ifndef __SpringCat_Common_Filter__
#define __SpringCat_Common_Filter__

#include <SpringCat/Common/SmallObject.h>
#include <BaseCat/System.h>
#include <BaseCat/Network.h>

namespace SpringCat
{
    namespace Common
    {
        class IFilter : public SmallObject<IFilter>
        {
        private:
            BaseCat::Network::Filter::Handle filter_;
            BaseCat::System::TL::string name_;

        public:
            IFilter(const char *name)
                : filter_(BaseCat::Network::Filter::CreateHandle()), name_(name)
            {
                BaseCat::Network::Filter::SetOnLinkOpened(filter_, &OnLinkOpened, this);
                BaseCat::Network::Filter::SetOnLinkClosed(filter_, &OnLinkClosed, this);
                BaseCat::Network::Filter::SetOnSend(filter_, &OnLinkSend, this);
                BaseCat::Network::Filter::SetOnReceived(filter_, &OnLinkReceived, this);
            }
            virtual ~IFilter(void)
            {
                BaseCat::Network::Filter::DestroyHandle(filter_);
                filter_ = NULL;
            }

            BaseCat::Network::Filter::Handle GetHandle(void) const
            {
                return filter_;
            }

            const char *GetName(void) const
            {
                return name_.c_str();
            }

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link) = 0;
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link) = 0;
            virtual void OnSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size) = 0;
            virtual void OnReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size) = 0;

        private:
            static void __stdcall OnLinkOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link, void *data)
            {
                IFilter *filter = static_cast<IFilter *>(data);
                filter->OnOpened(next, link);
            }
            static void __stdcall OnLinkClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link, void *data)
            {
                IFilter *filter = static_cast<IFilter *>(data);
                filter->OnClosed(next, link);
            }
            static void __stdcall OnLinkSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size, void *data)
            {
                IFilter *filter = static_cast<IFilter *>(data);
                filter->OnSend(next, link, buffer, size);
            }
            static void __stdcall OnLinkReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size, void *data)
            {
                IFilter *filter = static_cast<IFilter *>(data);
                filter->OnReceived(next, link, buffer, size);
            }
        };
    }
}

#endif
