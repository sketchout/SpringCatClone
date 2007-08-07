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

#ifndef __BaseCat_Timer__
#define __BaseCat_Timer__

#include "SystemBase.h"
#include "ThreadPool.h"

namespace BaseCat
{
    namespace System
    {
        namespace Timers
        {
            namespace TimerQueue
            {
                typedef struct _Handle
                {
                } *Handle;

                SYSTEM_API Handle __stdcall CreateHandle(ThreadPool::Handle threadPool);
                SYSTEM_API void __stdcall DestroyHandle(Handle timerQueue);
            }

            namespace Timer
            {
                typedef void (__stdcall *Event)(void *context);

                typedef struct _Handle
                {
                } *Handle;

                SYSTEM_API Handle __stdcall CreateHandle(TimerQueue::Handle timerQueue, unsigned int interval, Event callback, void *context);
                SYSTEM_API void __stdcall DestroyHandle(Handle timer);
            }
        }
    }
}

#endif
