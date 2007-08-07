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

#ifndef __BaseCat_ThreadPool__
#define __BaseCat_ThreadPool__

#include "SystemBase.h"
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include "PlatformConfig.h"
#endif
#include <cstddef>

namespace BaseCat
{
    namespace System
    {
        namespace ThreadPool
        {
            typedef struct _Handle
            {
            } *Handle;

            typedef unsigned int (__stdcall *ThreadEvent)(void *context);
            typedef void (__stdcall *OverlappedCompletionEvent)(unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context);

            SYSTEM_API Handle __stdcall CreateHandle(size_t size);
            SYSTEM_API void __stdcall DestroyHandle(Handle threadPool);

            SYSTEM_API size_t __stdcall GetPoolSize(Handle threadPool);

#if defined(_WIN32) || defined(_WIN64)
            SYSTEM_API bool __stdcall BindCompletionCallback(Handle threadPool, HANDLE handle, OverlappedCompletionEvent completionCallback);
#endif

            SYSTEM_API bool __stdcall QueueWorkItem(Handle threadPool, ThreadEvent callback, void *context);
            SYSTEM_API bool __stdcall QueueWorkItem(Handle threadPool, OverlappedCompletionEvent callback, unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context);
        }
    }
}

#endif
