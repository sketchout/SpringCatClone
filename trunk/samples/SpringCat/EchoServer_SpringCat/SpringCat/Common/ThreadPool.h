//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Common_ThreadPool__
#define __SpringCat_Common_ThreadPool__

#include <BaseCat/System.h>

namespace SpringCat
{
    namespace Common
    {
        class ThreadPool : private BaseCat::System::TL::NonCopyable
        {
        private:
            BaseCat::System::ThreadPool::Handle handle_;

        public:
            ThreadPool(size_t size)
                : handle_(BaseCat::System::ThreadPool::CreateHandle(size))
            {}
            ~ThreadPool(void)
            {
                BaseCat::System::ThreadPool::DestroyHandle(handle_);
                handle_ = NULL;
            }

            size_t GetSize(void) const
            {
                return BaseCat::System::ThreadPool::GetPoolSize(handle_);
            }

#if defined(_WIN32) || defined(_WIN64)
            bool BindCompletionCallback(HANDLE handle,
                BaseCat::System::ThreadPool::OverlappedCompletionEvent completionCallback)
            {
                return BaseCat::System::ThreadPool::BindCompletionCallback(
                    handle_, handle, completionCallback);
            }
#endif

            bool QueueWorkItem(BaseCat::System::ThreadPool::ThreadEvent callback, void *context)
            {
                return BaseCat::System::ThreadPool::QueueWorkItem(handle_, callback, context);
            }
            bool QueueWorkItem(BaseCat::System::ThreadPool::OverlappedCompletionEvent callback,
                unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context)
            {
                return BaseCat::System::ThreadPool::QueueWorkItem(
                    handle_, callback, errorCode, numberOfBytesTransferred, context);
            }

            BaseCat::System::ThreadPool::Handle GetHandle(void) const
            {
                return handle_;
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
