//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Filter_TextLineFilter__
#define __SpringCat_Filter_TextLineFilter__

#include <SpringCat/Common/SmallObject.h>
#include <SpringCat/Common/Filter.h>

namespace SpringCat
{
    namespace Filter
    {
        class TextLineFilter : public Common::IFilter
        {
        private:
            struct Context : public Common::SmallObject<Context>
            {
                static const char * const filterName;
                static const char * const contextName;

                BaseCat::System::Stream::Handle read_;

                Context(void)
                    : read_(BaseCat::System::Stream::CreateHandle())
                {}
                ~Context(void)
                {
                    BaseCat::System::Stream::DestroyHandle(read_);
                    read_ = NULL;
                }
            };

        public:
            TextLineFilter(void)
                : Common::IFilter(Context::filterName)
            {}
            virtual ~TextLineFilter(void)
            {}

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                Context *context = new Context;
                if (BaseCat::Network::Link::AddAttribute(link,
                    Context::contextName, context) == false)
                {
                    // error.
                    delete context;
                    context = NULL;
                }

                BaseCat::Network::Filter::DoOnLinkOpened(next, link);
            }
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                Context *context = static_cast<Context *>(
                    BaseCat::Network::Link::GetAttribute(link, Context::contextName));
                BaseCat::Network::Link::RemoveAttribute(link, Context::contextName);
                delete context;
                context = NULL;

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
                Context *context = static_cast<Context *>(
                    BaseCat::Network::Link::GetAttribute(link, Context::contextName));

                BaseCat::System::Stream::Write(context->read_, buffer, size);
                BaseCat::System::SmartHeap::Free(buffer);
                buffer = NULL;

                typedef BaseCat::System::TL::vector<std::pair<BaseCat::System::SmartHeap::Block, size_t> > Lines;
                Lines lines;

                bool retry = false;
                do
                {
                    retry = false;
                    for (size_t index = 0;
                        index != BaseCat::System::Stream::GetLength(context->read_); ++index)
                    {
                        const char *text = reinterpret_cast<const char *>(
                            BaseCat::System::Stream::GetRawPtr(context->read_));

                        if (*(text + index) == '\n')
                        {
                            size_t len = index + 1;
                            BaseCat::System::SmartHeap::Block line = BaseCat::System::SmartHeap::Alloc(len + 1);
                            memset(line, 0, len + 1);
                            BaseCat::System::Stream::Read(context->read_, line, len);
                            lines.push_back(std::make_pair(line, len + 1));
                            retry = true;
                            break;
                        }
                    }
                }
                while (retry == true);

                for (Lines::iterator it = lines.begin(); it != lines.end(); ++it)
                {
                    BaseCat::Network::Filter::DoOnReceived(next, link, (*it).first, (*it).second);
                }
            }
        };

        const char * const TextLineFilter::Context::filterName = "TextLineFilter";
        const char * const TextLineFilter::Context::contextName = "TextLineFilterContext";
    }
}

#endif