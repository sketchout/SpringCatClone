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

#ifndef __BaseCat_Stream__
#define __BaseCat_Stream__

#include "SystemBase.h"
#include "SmartHeap.h"

namespace BaseCat
{
    namespace System
    {
        namespace Stream
        {
            typedef struct _Handle
            {
            } *Handle;

            SYSTEM_API Handle __stdcall CreateHandle(void);
            SYSTEM_API void __stdcall DestroyHandle(Handle stream);

            SYSTEM_API size_t __stdcall Read(Handle stream, void *p, size_t size);
            SYSTEM_API size_t __stdcall Write(Handle stream, const void *p, size_t size);
            SYSTEM_API size_t __stdcall Peek(Handle stream, void *p, size_t size);
            SYSTEM_API size_t __stdcall RemoveLeft(Handle stream, size_t size);
            SYSTEM_API size_t __stdcall RemoveRight(Handle stream, size_t size);
            SYSTEM_API bool __stdcall Clear(Handle stream);
            SYSTEM_API size_t __stdcall GetLength(Handle stream);

            SYSTEM_API bool __stdcall Attach(Handle stream, SmartHeap::Block p);
            SYSTEM_API SmartHeap::Block __stdcall Detach(Handle stream);
            SYSTEM_API SmartHeap::Block __stdcall GetRawPtr(Handle stream);

            SYSTEM_API Handle __stdcall Clone(Handle stream);
        }
    }
}

#endif
