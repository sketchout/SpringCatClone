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

#ifndef __BaseCat_MPHeap__
#define __BaseCat_MPHeap__

#include "SystemBase.h"
#include "PlatformConfig.h"
#include <cstddef>

namespace BaseCat
{
    namespace System
    {
        namespace MPHeap
        {
            typedef struct _Handle
            {
            } *Handle;

            SYSTEM_API Handle __stdcall CreateHandle(size_t numberOfConcurrentThreads);
            SYSTEM_API void __stdcall DestroyHandle(Handle heap);

            SYSTEM_API void * __stdcall Alloc(Handle heap, size_t size);
            SYSTEM_API void __stdcall Free(Handle heap, void *ptr);
            SYSTEM_API void * __stdcall Realloc(Handle heap, void *ptr, size_t newSize);
            SYSTEM_API size_t __stdcall GetSize(Handle heap, const void *ptr);
        }
    }
}

#endif
