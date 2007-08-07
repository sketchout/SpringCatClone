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

#ifndef __SpringCat_Common_FilterChain__
#define __SpringCat_Common_FilterChain__

#include <SpringCat/Common/SmallObject.h>
#include <SpringCat/Common/Filter.h>
#include <BaseCat/System.h>
#include <BaseCat/Network.h>

namespace SpringCat
{
    namespace Common
    {
        class FilterChain : public SmallObject<FilterChain>
        {
        private:
            typedef BaseCat::System::TL::vector<std::pair<BaseCat::System::TL::string, IFilter *> > Filters;

        private:
            BaseCat::Network::FilterChain::Handle handle_;
            Filters filters_;

        public:
            FilterChain(void)
                : handle_(BaseCat::Network::FilterChain::CreateHandle())
            {}
            ~FilterChain(void)
            {
                BaseCat::Network::FilterChain::DestroyHandle(handle_);
                handle_ = NULL;
            }

            bool Add(const char *name, IFilter *filter)
            {
                if (Find(name) != NULL)
                {
                    return false;
                }

                filters_.push_back(std::make_pair(name, filter));

                return BaseCat::Network::FilterChain::Add(GetHandle(), name, filter->GetHandle());
            }
            bool Remove(const char *name)
            {
                for (Filters::iterator it = filters_.begin();
                    it != filters_.end(); ++it)
                {
                    if ((*it).first == name)
                    {
                        filters_.erase(it);
                        break;
                    }
                }

                return BaseCat::Network::FilterChain::Remove(GetHandle(), name);
            }
            IFilter *Find(const char *name) const
            {
                IFilter *result = NULL;

                for (Filters::const_iterator it = filters_.begin();
                    it != filters_.end(); ++it)
                {
                    if ((*it).first == name)
                    {
                        result = (*it).second;
                        break;
                    }
                }

                return result;
            }

            BaseCat::Network::FilterChain::Handle GetHandle(void) const
            {
                return handle_;
            }
        };
    }
}

#endif
