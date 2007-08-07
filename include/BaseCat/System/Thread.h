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

#ifndef __BaseCat_Thread__
#define __BaseCat_Thread__

#include "SystemBase.h"
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include "PlatformConfig.h"
    #include <pthread.h>
#endif

namespace BaseCat
{
    namespace System
    {
        namespace Threading
        {
            class SYSTEM_API Thread
            {
            private:
                struct Impl;
                Impl *impl_;

            public:
                Thread(void);
                virtual ~Thread(void);

                bool Start(void);
                void Stop(void);

#if defined(_WIN32) || defined(_WIN64)
                bool Suspend(void);
                bool Resume(void);
#endif
                void Terminate(void);
#if defined(_WIN32) || defined(_WIN64)
                bool WaitFor(unsigned int timeout = INFINITE);
#else
                bool WaitFor(void);
#endif

                void SetFreeOnTerminate(bool value);
                bool GetFreeOnTerminate(void) const;
                void SetFinished(bool value);
                bool GetFinished(void) const;
#if defined(_WIN32) || defined(_WIN64)
                bool SetPriority(int value);
                int GetPriority(void);
                void SetSuspended(bool value);
                bool GetSuspended(void) const;
#endif

#if defined(_WIN32) || defined(_WIN64)
                HANDLE GetHandle(void) const;
                unsigned int GetThreadID(void) const;
#else
                pthread_t GetHandle(void) const;
#endif

            protected:
                bool Terminated(void) const;
#if defined(_WIN32) || defined(_WIN64)
                void SetThreadName(const char *name);
#endif
                void SetReturnValue(int value);
                int GetReturnValue(void) const;

                virtual void Execute(void) = 0;

            private:
                static unsigned int __stdcall InternalThreadProc(Thread *thread);
            };

#if defined(_WIN32) || defined(_WIN64)
            SYSTEM_API bool __stdcall SetThreadName(unsigned int threadID, const char *name);
#endif
        }
    }
}

#endif
