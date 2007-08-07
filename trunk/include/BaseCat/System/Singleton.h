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

#ifndef __BaseCat_Singleton__
#define __BaseCat_Singleton__

#include "SyncObject.h"
#include <stdexcept>

namespace BaseCat
{
    namespace System
    {
        namespace TL
        {
            template<typename T>
            class Singleton
            {
            private:
                static volatile T *instance_;
                static bool destroyed_;
                static Threading::CriticalSection lock_;

            public:
                inline static T &Instance(void)
                {
                    if (instance_ == NULL)
                    {
                        MakeInstance();
                    }

                    return const_cast<T &>(*instance_);
                }

            private:
                Singleton(void)
                {
                }

                static void MakeInstance(void)
                {
                    Threading::ScopedLock<Threading::CriticalSection> lock(lock_);

                    if (instance_ == NULL)
                    {
                        if (destroyed_ == true)
                        {
                            throw std::logic_error("[BaseCat::System::TL::Singleton] Dead reference detected.");
                        }

                        instance_ = new T;
                        if (NULL == instance_)
                        {
                            throw std::bad_alloc();
                        }

                        atexit(DestroySingleton);
                    }
                }
                static void DestroySingleton(void)
                {
                    delete instance_;
                    instance_ = NULL;

                    destroyed_ = true;
                }
            };

            template<typename T> volatile T *Singleton<T>::instance_ = NULL;
            template<typename T> bool Singleton<T>::destroyed_ = false;
            template<typename T> Threading::CriticalSection Singleton<T>::lock_;
        }
    }
}

#endif
