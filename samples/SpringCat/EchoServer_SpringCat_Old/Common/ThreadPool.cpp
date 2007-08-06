#include "ThreadPool.h"

namespace SpringCat
{
    namespace Common
    {
        struct ThreadPool::Impl
        {
            BaseCat::System::ThreadPool::Handle handle_;

            Impl(size_t size)
                : handle_(BaseCat::System::ThreadPool::CreateHandle(size))
            {
                if (handle_ == NULL)
                {
                    throw std::bad_alloc();
                }
            }
            ~Impl(void)
            {
                BaseCat::System::ThreadPool::DestroyHandle(handle_);
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

        ThreadPool::ThreadPool(size_t size)
            : impl_(new Impl(size))
        {}

        ThreadPool::~ThreadPool(void)
        {
            delete impl_;
            impl_ = NULL;
        }

        size_t ThreadPool::GetSize(void) const
        {
            return BaseCat::System::ThreadPool::GetPoolSize(impl_->handle_);
        }

#if defined(_WIN32) || defined(_WIN64)
        bool ThreadPool::BindCompletionCallback(HANDLE handle,
            BaseCat::System::ThreadPool::OverlappedCompletionEvent completionCallback)
        {
            return BaseCat::System::ThreadPool::BindCompletionCallback(
                impl_->handle_, handle, completionCallback);
        }
#endif

        bool ThreadPool::QueueWorkItem(BaseCat::System::ThreadPool::ThreadEvent callback, void *context)
        {
            return BaseCat::System::ThreadPool::QueueWorkItem(
                impl_->handle_, callback, context);
        }

        bool ThreadPool::QueueWorkItem(BaseCat::System::ThreadPool::OverlappedCompletionEvent callback,
            unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context)
        {
            return BaseCat::System::ThreadPool::QueueWorkItem(
                impl_->handle_, callback, errorCode, numberOfBytesTransferred, context);
        }

        BaseCat::System::ThreadPool::Handle ThreadPool::GetHandle(void) const
        {
            return impl_->handle_;
        }

        // static
        void *ThreadPool::operator new(size_t size)
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
        void ThreadPool::operator delete(void *p) throw()
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
