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

#ifndef __BaseCat_SyncObject__
#define __BaseCat_SyncObject__

#include "SystemBase.h"
#include "NonCopyable.h"
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include "PlatformConfig.h"
    #include <cstddef>
#endif
#include <cassert>

namespace BaseCat
{
    namespace System
    {
        namespace Threading
        {
            class SYSTEM_API CriticalSection
            {
            private:
                struct Impl;
                Impl *impl_;

            public:
#if defined(_WIN32) || defined(_WIN64)
                explicit CriticalSection(unsigned int spinCount = 4000);
#else
                CriticalSection(void);
#endif
                ~CriticalSection(void);

                void Enter(void);
                bool TryEnter(void);
                void Leave(void);

                void Lock(void);
                void Unlock(void);

            private:
                CriticalSection(const CriticalSection &);
                const CriticalSection &operator=(const CriticalSection &);
            };

            class SYSTEM_API RWCriticalSection
            {
            private:
                struct Impl;
                Impl *impl_;

            public:
                RWCriticalSection(void);
                ~RWCriticalSection(void);

                void ReadLock(void);
                void ReadUnlock(void);
                void WriteLock(void);
                void WriteUnlock(void);

            private:
                void InternalUnlock(void);

            private:
                RWCriticalSection(const RWCriticalSection &);
                const RWCriticalSection &operator=(const RWCriticalSection &);
            };

            class SYSTEM_API Mutex
            {
            private:
                struct Impl;
                Impl *impl_;

            public:
#if defined(_WIN32) || defined(_WIN64)
                Mutex(bool initialOwner = false, const char *name = NULL, LPSECURITY_ATTRIBUTES sa = NULL);
                Mutex(const char *nameToOpen, unsigned int desiredAccess = MUTEX_ALL_ACCESS, bool inheritHandle = false);
#else
                Mutex(bool initialOwner = false, const char *name = NULL);
#endif
                ~Mutex(void);

#if defined(_WIN32) || defined(_WIN64)
                bool Release(void);

                bool Wait(unsigned int milliseconds = INFINITE);
                bool WaitEx(bool alertable, unsigned int milliseconds = INFINITE);

                bool Lock(unsigned int milliseconds = INFINITE);
                bool Unlock(void);
#else
                bool Lock(void);
                bool Unlock(void);
#endif

            private:
                Mutex(const Mutex &);
                const Mutex &operator=(const Mutex &);
            };

            class SYSTEM_API Semaphore
            {
            private:
                struct Impl;
                Impl *impl_;

            public:
#if defined(_WIN32) || defined(_WIN64)
                Semaphore(int initialCount, int maximumCount, const char *name = NULL, LPSECURITY_ATTRIBUTES sa = NULL);
                Semaphore(const char *nameToOpen, unsigned int desiredAccess = SEMAPHORE_ALL_ACCESS, bool inheritHandle = false);
#else
                Semaphore(int initialCount, int maximumCount);
#endif
                ~Semaphore(void);

#if defined(_WIN32) || defined(_WIN64)
                bool Release(int releaseCount, int *previousCount = NULL);

                bool Wait(unsigned int milliseconds = INFINITE);
                bool WaitEx(bool alertable, unsigned int milliseconds = INFINITE);

                bool Lock(unsigned int milliseconds = INFINITE);
                bool Unlock(void);
#else
                bool Lock(void);
                bool Unlock(void);
#endif

            private:
                Semaphore(const Semaphore &);
                const Semaphore &operator=(const Semaphore &);
            };

            class SYSTEM_API AutoResetEvent
            {
            private:
                struct Impl;
                Impl *impl_;

            public:
                AutoResetEvent(bool initialState = false);
                ~AutoResetEvent(void);

                bool Set(void);
                bool Reset(void);
                bool Pulse(void);

                bool Wait(unsigned int milliseconds = INFINITE);
#if defined(_WIN32) || defined(_WIN64)
                bool WaitEx(bool alertable, unsigned int milliseconds = INFINITE);
#endif

            private:
                AutoResetEvent(const AutoResetEvent &);
                const AutoResetEvent &operator=(const AutoResetEvent &);
            };

            class SYSTEM_API ManualResetEvent
            {
            private:
                struct Impl;
                Impl *impl_;

            public:
                ManualResetEvent(bool initialState = false);
                ~ManualResetEvent(void);

                bool Set(void);
                bool Reset(void);
                bool Pulse(void);

                bool Wait(unsigned int milliseconds = INFINITE);
#if defined(_WIN32) || defined(_WIN64)
                bool WaitEx(bool alertable, unsigned int milliseconds = INFINITE);
#endif

            private:
                ManualResetEvent(const ManualResetEvent &);
                const ManualResetEvent &operator=(const ManualResetEvent &);
            };

            // ScopedLock
            template<typename T>
            class ScopedLock : private TL::NonCopyable
            {
            private:
                T &mutex_;
                bool locked_;

            public:
                explicit ScopedLock(T *mutex, bool initiallyLocked = true) : mutex_(*mutex), locked_(false)
                {
                    if (initiallyLocked == true)
                    {
                        Lock();
                    }
                }
                explicit ScopedLock(T &mutex, bool initiallyLocked = true) : mutex_(mutex), locked_(false)
                {
                    if (initiallyLocked == true)
                    {
                        Lock();
                    }
                }
                ~ScopedLock(void)
                {
                    if (locked_ == true)
                    {
                        Unlock();
                    }
                }

                void Lock(void)
                {
                    if (locked_ == true)
                    {
                        assert(!"[ScopedLock::Lock()] locked_ must be false.");
                        throw "[ScopedLock::Lock()] locked_ must be false.";
                    }

                    mutex_.Lock();
                    locked_ = true;
                }
                void Unlock(void)
                {
                    if (locked_ == false)
                    {
                        assert(!"[ScopedLock::Unlock()] locked_ must be true.");
                        throw "[ScopedLock::Unlock()] locked_ must be true.";
                    }

                    locked_ = false;
                    mutex_.Unlock();
                }

                bool IsLocked(void) const
                {
                    return locked_;
                }
            };

            // ScopedTimedLock
            template<typename T>
            class ScopedTimedLock : private TL::NonCopyable
            {
            private:
                T &mutex_;
                bool locked_;

            public:
                ScopedTimedLock(T *mutex, unsigned int milliseconds) : mutex_(*mutex), locked_(false)
                {
                    TimedLock(milliseconds);
                }
                explicit ScopedTimedLock(T *mutex, bool initiallyLocked = true) : mutex_(*mutex), locked_(false)
                {
                    if (initiallyLocked == true)
                    {
                        Lock();
                    }
                }
                ScopedTimedLock(T &mutex, unsigned int milliseconds) : mutex_(mutex), locked_(false)
                {
                    TimedLock(milliseconds);
                }
                explicit ScopedTimedLock(T &mutex, bool initiallyLocked = true) : mutex_(mutex), locked_(false)
                {
                    if (initiallyLocked == true)
                    {
                        Lock();
                    }
                }
                ~ScopedTimedLock(void)
                {
                    if (locked_ == true)
                    {
                        Unlock();
                    }
                }

                void Lock(void)
                {
                    if (locked_ == true)
                    {
                        assert(!"[ScopedTimedLock::Lock()] locked_ must be false.");
                        throw "[ScopedTimedLock::Lock()] locked_ must be false.";
                    }

                    mutex_.Lock(INFINITE);
                    locked_ = true;
                }
                void TimedLock(unsigned int milliseconds)
                {
                    if (locked_ == true)
                    {
                        assert(!"[ScopedTimedLock::TimedLock()] locked_ must be false.");
                        throw "[ScopedTimedLock::TimedLock()] locked_ must be false.";
                    }

                    mutex_.Lock(milliseconds);
                    locked_ = true;
                }
                void Unlock(void)
                {
                    if (locked_ == false)
                    {
                        assert(!"[ScopedTimedLock::Unlock()] locked_ must be true.");
                        throw "[ScopedTimedLock::Unlock()] locked_ must be true.";
                    }

                    locked_ = false;
                    mutex_.Unlock();
                }

                bool IsLocked(void) const
                {
                    return locked_;
                }
            };

            // ScopedRWLock
            namespace ScopedRWLockState
            {
                enum TRWLockState
                {
                    Unlocked = 0,
                    ReadLocked = 1,
                    WriteLocked = 2
                };
            }

            template<typename T>
            class ScopedRWLock : private TL::NonCopyable
            {
            private:
                T &mutex_;
                ScopedRWLockState::TRWLockState state_;

            public:
                ScopedRWLock(T *mutex, ScopedRWLockState::TRWLockState initialState) : mutex_(*mutex), state_(ScopedRWLockState::Unlocked)
                {
                    if (initialState == ScopedRWLockState::ReadLocked)
                    {
                        ReadLock();
                    }
                    else if (initialState == ScopedRWLockState::WriteLocked)
                    {
                        WriteLock();
                    }
                }
                ScopedRWLock(T &mutex, ScopedRWLockState::TRWLockState initialState) : mutex_(mutex), state_(ScopedRWLockState::Unlocked)
                {
                    if (initialState == ScopedRWLockState::ReadLocked)
                    {
                        ReadLock();
                    }
                    else if (initialState == ScopedRWLockState::WriteLocked)
                    {
                        WriteLock();
                    }
                }
                ~ScopedRWLock(void)
                {
                    if (state_ != ScopedRWLockState::Unlocked)
                    {
                        Unlock();
                    }
                }

                void ReadLock(void)
                {
                    if (state_ != ScopedRWLockState::Unlocked)
                    {
                        assert(!"[ScopedRWLock::ReadLock()] state_ must be ScopedRWLockState::Unlocked.");
                        throw "[ScopedRWLock::ReadLock()] state_ must be ScopedRWLockState::Unlocked.";
                    }

                    mutex_.ReadLock();
                    state_ = ScopedRWLockState::ReadLocked;
                }
                void WriteLock(void)
                {
                    if (state_ != ScopedRWLockState::Unlocked)
                    {
                        assert(!"[ScopedRWLock::WriteLock()] state_ must be ScopedRWLockState::Unlocked.");
                        throw "[ScopedRWLock::WriteLock()] state_ must be ScopedRWLockState::Unlocked.";
                    }

                    mutex_.WriteLock();
                    state_ = ScopedRWLockState::WriteLocked;
                }
                void Unlock(void)
                {
                    if (state_ == ScopedRWLockState::Unlocked)
                    {
                        assert(!"[ScopedRWLock::Unlock()] state_ must not be ScopedRWLockState::Unlocked.");
                        throw "[ScopedRWLock::Unlock()] state_ must not be ScopedRWLockState::Unlocked.";
                    }

                    if (state_ == ScopedRWLockState::ReadLocked)
                    {
                        mutex_.ReadUnlock();
                    }
                    else if (state_ == ScopedRWLockState::WriteLocked)
                    {
                        mutex_.WriteUnlock();
                    }
                }

                bool IsLocked(void) const
                {
                    return state_ != ScopedRWLockState::Unlocked;
                }
                bool IsReadLocked(void) const
                {
                    return state_ == ScopedRWLockState::ReadLocked;
                }
                bool IsWriteLocked(void) const
                {
                    return state_ == ScopedRWLockState::WriteLocked;
                }

                ScopedRWLockState::TRWLockState GetState(void) const
                {
                    return state_;
                }
            };

            typedef ScopedLock<CriticalSection> CSLock;
            typedef ScopedRWLock<RWCriticalSection> RWCSLock;
        }
    }
}

#endif //__SyncObject__
