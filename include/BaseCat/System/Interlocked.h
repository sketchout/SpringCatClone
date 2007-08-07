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

#ifndef __BaseCat_Interlocked__
#define __BaseCat_Interlocked__

#include "SystemBase.h"

namespace BaseCat
{
    namespace System
    {
        namespace Threading
        {
            namespace Interlocked
            {
                SYSTEM_API int __stdcall Increment(volatile int *value);
                SYSTEM_API unsigned int __stdcall Increment(volatile unsigned int *value);

                SYSTEM_API int __stdcall Decrement(volatile int *value);
                SYSTEM_API unsigned int __stdcall Decrement(volatile unsigned int *value);

                SYSTEM_API int __stdcall Exchange(volatile int *dest, int value);
                SYSTEM_API unsigned int __stdcall Exchange(volatile unsigned int *dest, unsigned int value);

                SYSTEM_API int __stdcall CompareExchange(volatile int *dest, int value, int comperand);
                SYSTEM_API unsigned int __stdcall CompareExchange(volatile unsigned int *dest, unsigned int value, unsigned int comperand);
            }
        }
    }
}

#endif
