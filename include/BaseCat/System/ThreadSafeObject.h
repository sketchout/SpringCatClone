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

#ifndef __BaseCat_ThreadSafeObject__
#define __BaseCat_ThreadSafeObject__

#include "SyncObject.h"
#include "NonCopyable.h"

namespace BaseCat
{
    namespace System
    {
        namespace TL
        {
#if (_MSC_VER >= 1301) || defined(_UNIX_)
            namespace Private
            {
                template<typename T>
                struct AddPointer
                {
                    typedef T* PointerType;
                    typedef T PointeeType;
                };
                template<typename T>
                struct AddPointer<T *>
                {
                    typedef T* PointerType;
                    typedef T PointeeType;
                };
            }

            template<typename T>
            class ThreadSafeObject : private NonCopyable
            {
            private:
                typename Private::AddPointer<T>::PointeeType obj_;
                Threading::CriticalSection lock_;

            public:
                ThreadSafeObject(void)
                {
                }
                ~ThreadSafeObject(void)
                {
                }

                typename Private::AddPointer<T>::PointerType Lock(void)
                {
                    lock_.Enter();

                    return &obj_;
                }
                void Unlock(void)
                {
                    lock_.Leave();
                }
            };

            template<typename T>
            class RWThreadSafeObject : private NonCopyable
            {
            private:
                typename Private::AddPointer<T>::PointeeType obj_;
                Threading::RWCriticalSection lock_;

            public:
                RWThreadSafeObject(void)
                {
                }
                ~RWThreadSafeObject(void)
                {
                }

                typename Private::AddPointer<T>::PointerType ReadLock(void)
                {
                    lock_.ReadLock();

                    return &obj_;
                }
                void ReadUnlock(void)
                {
                    lock_.ReadUnlock();
                }
                typename Private::AddPointer<T>::PointerType WriteLock(void)
                {
                    lock_.WriteLock();

                    return &obj_;
                }
                void WriteUnlock(void)
                {
                    lock_.WriteUnlock();
                }
            };
#else
            template<typename T>
            class ThreadSafeObject : private NonCopyable
            {
            private:
                typename T obj_;
                Threading::CriticalSection lock_;

            public:
                ThreadSafeObject(void)
                {
                }
                ~ThreadSafeObject(void)
                {
                }

                typename T *Lock(void)
                {
                    lock_.Enter();

                    return &obj_;
                }
                void Unlock(void)
                {
                    lock_.Leave();
                }
            };

            template<typename T>
            class RWThreadSafeObject : private NonCopyable
            {
            private:
                typename T obj_;
                Threading::RWCriticalSection lock_;

            private:
                RWThreadSafeObject(const RWThreadSafeObject &rhs);
                RWThreadSafeObject &operator=(const RWThreadSafeObject &rhs);

            public:
                RWThreadSafeObject(void)
                {
                }
                ~RWThreadSafeObject(void)
                {
                }

                typename T *ReadLock(void)
                {
                    lock_.ReadLock();

                    return &obj_;
                }
                void ReadUnlock(void)
                {
                    lock_.ReadUnlock();
                }
                typename T *WriteLock(void)
                {
                    lock_.WriteLock();

                    return &obj_;
                }
                void WriteUnlock(void)
                {
                    lock_.WriteUnlock();
                }
            };
#endif
        }
    }
}

#endif
