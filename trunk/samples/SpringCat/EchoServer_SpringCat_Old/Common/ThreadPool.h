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
            struct Impl;
            Impl *impl_;

        public:
            ThreadPool(size_t size);
            virtual ~ThreadPool(void);

            size_t GetSize(void) const;

#if defined(_WIN32) || defined(_WIN64)
            bool BindCompletionCallback(HANDLE handle,
                BaseCat::System::ThreadPool::OverlappedCompletionEvent completionCallback);
#endif

            bool QueueWorkItem(BaseCat::System::ThreadPool::ThreadEvent callback, void *context);
            bool QueueWorkItem(BaseCat::System::ThreadPool::OverlappedCompletionEvent callback,
                unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context);

            BaseCat::System::ThreadPool::Handle GetHandle(void) const;

        public:
            static void *operator new(size_t size);
            static void operator delete(void *p) throw();
        };
    }
}

#endif
