//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
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
