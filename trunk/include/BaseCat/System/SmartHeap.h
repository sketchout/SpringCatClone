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

#ifndef __BaseCat_SmartHeap__
#define __BaseCat_SmartHeap__

#include "SystemBase.h"
#include "PlatformConfig.h"
#include <cstddef>

namespace BaseCat
{
    namespace System
    {
        namespace SmartHeap
        {
            typedef struct _Block
            {
            } *Block;

            typedef void (__stdcall *Event)(const Block ptr, void *context);

            SYSTEM_API Block __stdcall Alloc(size_t size);
            SYSTEM_API Block __stdcall Alloc(size_t size, Event onFree, void *context);
            SYSTEM_API void __stdcall Free(Block ptr);
            SYSTEM_API Block __stdcall Realloc(Block ptr, size_t newSize);
            SYSTEM_API Block __stdcall Realloc(Block ptr, size_t newSize, Event onFree, void *context);
            SYSTEM_API size_t __stdcall GetSize(const Block ptr);
            SYSTEM_API Block __stdcall Copy(const Block ptr);
            SYSTEM_API Block __stdcall Clone(const Block ptr);
        }
    }
}

#endif
