#include "TextLineFilter.h"
#include <vector>
#include <utility>

namespace SpringCat
{
    namespace Filter
    {
        struct TextLineFilter::Context
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

            // static
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

            // static
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

        const char * const TextLineFilter::Context::filterName = "TextLineFilter";
        const char * const TextLineFilter::Context::contextName = "TextLineFilterContext";

        TextLineFilter::TextLineFilter(Common::FilterChain *chain)
            : Common::IFilter(chain, Context::filterName)
        {}

        TextLineFilter::~TextLineFilter(void)
        {}

        void TextLineFilter::OnOpened(BaseCat::Network::Filter::Handle next,
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

        void TextLineFilter::OnClosed(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link)
        {
            Context *context = static_cast<Context *>(
                BaseCat::Network::Link::GetAttribute(link, Context::contextName));
            BaseCat::Network::Link::RemoveAttribute(link, Context::contextName);
            delete context;
            context = NULL;

            BaseCat::Network::Filter::DoOnLinkClosed(next, link);
        }

        void TextLineFilter::OnSend(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link,
            BaseCat::System::SmartHeap::Block buffer, size_t size)
        {
            BaseCat::Network::Filter::DoOnSend(next, link, buffer, size);
        }

        void TextLineFilter::OnReceived(BaseCat::Network::Filter::Handle next,
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
    }
}
