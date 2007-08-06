//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Common_Filter__
#define __SpringCat_Common_Filter__

#include <BaseCat/System.h>
#include <BaseCat/Network.h>

namespace SpringCat
{
    namespace Common
    {
        class IFilter : private BaseCat::System::TL::NonCopyable
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

        public:
            static void *operator new(size_t size)
            {
                if (size == 0)
                {
                    size = 1;
                }

                void *result = NULL;

#pragma warning(disable:4127)
                while (true)
#pragma warning(default:4127)
                {
                    result = BaseCat::System::MPHeap::Alloc(NULL, size);
                    if (result != NULL)
                    {
                        break;
                    }

                    std::new_handler newHandler = std::set_new_handler(0);
                    std::set_new_handler(newHandler);

                    if (newHandler != NULL)
                    {
                        (*newHandler)();
                    }
                    else
                    {
                        throw std::bad_alloc();
                    }
                }

                return result;
            }
            static void operator delete(void *p) throw()
            {
                if (p == NULL)
                {
                    return;
                }

                BaseCat::System::MPHeap::Free(NULL, p);
                p = NULL;
            }
        };
    }
}

#endif
