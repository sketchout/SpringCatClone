#include "FilterChain.h"
#include "Filter.h"
#include <utility>

namespace SpringCat
{
    namespace Common
    {
        struct FilterChain::Impl
        {
            typedef BaseCat::System::TL::vector<std::pair<BaseCat::System::TL::string, IFilter *> > Filters;

            BaseCat::Network::FilterChain::Handle handle_;
            Filters filters_;

            Impl(void)
                : handle_(BaseCat::Network::FilterChain::CreateHandle())
            {
                if (handle_ == NULL)
                {
                    throw std::bad_alloc();
                }
            }
            ~Impl(void)
            {
                BaseCat::Network::FilterChain::DestroyHandle(handle_);
                handle_ = NULL;
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

        FilterChain::FilterChain(void)
            : impl_(new Impl)
        {}

        FilterChain::~FilterChain(void)
        {
            delete impl_;
            impl_ = NULL;
        }

        bool FilterChain::Add(const char *name, IFilter *filter)
        {
            if (Find(name) != NULL)
            {
                return false;
            }

            impl_->filters_.push_back(std::make_pair(name, filter));

            return BaseCat::Network::FilterChain::Add(GetHandle(), name, filter->GetHandle());
        }

        bool FilterChain::Remove(const char *name)
        {
            for (Impl::Filters::iterator it = impl_->filters_.begin();
                it != impl_->filters_.end(); ++it)
            {
                if ((*it).first == name)
                {
                    impl_->filters_.erase(it);
                    break;
                }
            }

            return BaseCat::Network::FilterChain::Remove(GetHandle(), name);
        }

        IFilter *FilterChain::Find(const char *name) const
        {
            IFilter *result = NULL;

            for (Impl::Filters::iterator it = impl_->filters_.begin();
                it != impl_->filters_.end(); ++it)
            {
                if ((*it).first == name)
                {
                    result = (*it).second;
                    break;
                }
            }

            return result;
        }

        BaseCat::Network::FilterChain::Handle FilterChain::GetHandle(void) const
        {
            return impl_->handle_;
        }

        // static
        void *FilterChain::operator new(size_t size)
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
        void FilterChain::operator delete(void *p) throw()
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
