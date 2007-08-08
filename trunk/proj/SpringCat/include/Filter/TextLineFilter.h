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

#ifndef __SpringCat_Filter_TextLineFilter__
#define __SpringCat_Filter_TextLineFilter__

#include <SpringCat/Common/SmallObject.h>
#include <SPringCat/Common/Exceptions.h>
#include <SpringCat/Common/Filter.h>

namespace SpringCat
{
    namespace Filter
    {
        struct LineDelimiter
        {
            static const char * const AUTO;
            static const char * const WINDOWS;
            static const char * const UNIX;
            static const char * const MAC;
        };

        const char * const LineDelimiter::AUTO = "";
        const char * const LineDelimiter::WINDOWS = "\r\n";
        const char * const LineDelimiter::UNIX = "\n";
        const char * const LineDelimiter::MAC = "\r";

        //template<typename CharT = char>
        class TextLineFilter : public Common::IFilter
        {
        private:
            static const char * const FILTER_NAME;

        private:
            struct Context : public Common::SmallObject<Context>
            {
                static const char * const CONTEXT_NAME;

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

        private:
            const char * const delimiter_;

        public:
            TextLineFilter(void)
                : Common::IFilter(FILTER_NAME),
                delimiter_(LineDelimiter::AUTO)
            {}
            TextLineFilter(const char * const delimiter)
                : Common::IFilter(FILTER_NAME), delimiter_(delimiter)
            {
                if (delimiter_ == NULL)
                {
                    throw Common::NullPointerException("delimiter");
                }
            }
            virtual ~TextLineFilter(void)
            {}

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                using namespace BaseCat;

                Context *context = new Context;
                if (Network::Link::AddAttribute(link,
                    Context::CONTEXT_NAME, context) == false)
                {
                    // error.
                    delete context;
                    context = NULL;
                }

                Network::Filter::DoOnLinkOpened(next, link);
            }
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                using namespace BaseCat;

                Context *context = static_cast<Context *>(
                    Network::Link::GetAttribute(link, Context::CONTEXT_NAME));
                Network::Link::RemoveAttribute(link, Context::CONTEXT_NAME);
                delete context;
                context = NULL;

                BaseCat::Network::Filter::DoOnLinkClosed(next, link);
            }
            virtual void OnSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size)
            {
                using namespace BaseCat;

                // TODO: check line ending

                Network::Filter::DoOnSend(next, link, buffer, size);
            }
            virtual void OnReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size)
            {
                using namespace BaseCat;

                Context *context = static_cast<Context *>(
                    Network::Link::GetAttribute(link, Context::CONTEXT_NAME));

                System::Stream::Write(context->read_, buffer, size);
                System::SmartHeap::Free(buffer);
                buffer = NULL;

                typedef System::TL::vector<std::pair<System::SmartHeap::Block, size_t> > Lines;
                Lines lines;

                bool retry = false;
                do
                {
                    retry = false;
                    size_t streamLength = System::Stream::GetLength(context->read_);
                    for (size_t index = 0; index != streamLength; ++index)
                    {
                        const char *text = reinterpret_cast<const char *>(
                            System::Stream::GetRawPtr(context->read_));

                        if (*(text + index) == '\n')
                        {
                            size_t len = index + 1;
                            System::SmartHeap::Block line = System::SmartHeap::Alloc(len + 1);
                            memset(line, 0, len + 1);
                            System::Stream::Read(context->read_, line, len);
                            lines.push_back(std::make_pair(line, len + 1));
                            retry = true;
                            break;
                        }
                    }
                }
                while (retry == true);

                for (Lines::iterator it = lines.begin(); it != lines.end(); ++it)
                {
                    Network::Filter::DoOnReceived(next, link, (*it).first, (*it).second);
                }
            }
        };

        const char * const TextLineFilter::FILTER_NAME = "SpringCat::Filter::TextLineFilter";
        const char * const TextLineFilter::Context::CONTEXT_NAME = "SpringCat::Filter::TextLineFilter::Context";
    }
}

#endif
