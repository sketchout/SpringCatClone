#include "Filter.h"
#include "FilterChain.h"

namespace SpringCat
{
    namespace Common
    {
        struct IFilter::Impl
        {
            BaseCat::Network::FilterChain::Handle chain_;
            BaseCat::Network::Filter::Handle filter_;
            BaseCat::System::TL::string name_;

            Impl(FilterChain *chain, const char *name)
                : chain_(chain->GetHandle()),
                filter_(BaseCat::Network::Filter::CreateHandle()), name_(name)
            {
                if (filter_ == NULL)
                {
                    throw std::bad_alloc();
                }
            }

            ~Impl(void)
            {}

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

        IFilter::IFilter(FilterChain *chain, const char *name)
            : impl_(new Impl(chain, name))
        {
            BaseCat::Network::Filter::SetOnLinkOpened(impl_->filter_, &Impl::OnLinkOpened, this);
            BaseCat::Network::Filter::SetOnLinkClosed(impl_->filter_, &Impl::OnLinkClosed, this);
            BaseCat::Network::Filter::SetOnSend(impl_->filter_, &Impl::OnLinkSend, this);
            BaseCat::Network::Filter::SetOnReceived(impl_->filter_, &Impl::OnLinkReceived, this);

            BaseCat::Network::FilterChain::Add(impl_->chain_, impl_->name_.c_str(), impl_->filter_);
        }

        IFilter::~IFilter(void)
        {
            BaseCat::Network::FilterChain::Remove(impl_->chain_, impl_->name_.c_str());

            BaseCat::Network::Filter::DestroyHandle(impl_->filter_);
            impl_->filter_ = NULL;

            delete impl_;
            impl_ = NULL;
        }

        BaseCat::Network::Filter::Handle IFilter::GetHandle(void) const
        {
            return impl_->filter_;
        }

        // static
        void *IFilter::operator new(size_t size)
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
        void IFilter::operator delete(void *p) throw()
        {
            if (p == NULL)
            {
                return;
            }

            BaseCat::System::MPHeap::Free(NULL, p);
            p = NULL;
        }
    }
}
