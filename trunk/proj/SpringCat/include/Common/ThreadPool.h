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

#ifndef __SpringCat_Common_ThreadPool__
#define __SpringCat_Common_ThreadPool__

#include <SpringCat/Common/SmallObject.h>
#include <BaseCat/System.h>

namespace SpringCat
{
    namespace Common
    {
        class ThreadPool : public SmallObject<ThreadPool>
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
        };
    }
}

#endif
