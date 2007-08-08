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

#ifndef __SpringCat_Filter_ExecutorFilter__
#define __SpringCat_Filter_ExecutorFilter__

#include <SpringCat/Common/SmallObject.h>
#include <SpringCat/Common/Filter.h>

namespace SpringCat
{
    namespace Filter
    {
        class ExecutorFilter : public Common::IFilter
        {
        private:
            static const char * const FILTER_NAME;

        private:
            class IEventRunnable : public Common::SmallObject<IEventRunnable>
            {
            private:
                __uint64 seq_;

            public:
                IEventRunnable(__uint64 seq)
                    : seq_(seq)
                {}
                virtual ~IEventRunnable(void) {}

                __uint64 GetSeq(void) const
                {
                    return seq_;
                }

                virtual bool Process(void) = 0;
            };

            typedef BaseCat::System::TL::deque<IEventRunnable *> EventQueue;

            struct Context : public Common::SmallObject<Context>
            {
            public:
                static const char * const CONTEXT_NAME;

            private:
                __uint64 currentSeq_;
                __uint64 processedSeq_;
                bool locked_;
                EventQueue queue_;
                mutable BaseCat::System::Threading::CriticalSection lock_;

            public:
                Context(void)
                    : currentSeq_(0), processedSeq_(0), locked_(false)
                {}
                ~Context(void)
                {}

                bool TryLock(void)
                {
                    BaseCat::System::Threading::ScopedLock<
                        BaseCat::System::Threading::CriticalSection> lock(lock_);

                    if (locked_ == true)
                    {
                        return false;
                    }

                    locked_ = true;

                    return true;
                }
                void Unlock(void)
                {
                    BaseCat::System::Threading::ScopedLock<
                        BaseCat::System::Threading::CriticalSection> lock(lock_);

                    locked_ = false;
                }

                __uint64 GetSeq(void)
                {
                    BaseCat::System::Threading::ScopedLock<
                        BaseCat::System::Threading::CriticalSection> lock(lock_);

                    return currentSeq_++;
                }

                bool PushEvent(IEventRunnable *event)
                {
                    BaseCat::System::Threading::ScopedLock<
                        BaseCat::System::Threading::CriticalSection> lock(lock_);

                    queue_.push_back(event);

                    if (locked_ == true)
                    {
                        return false;
                    }

                    return true;
                }
                bool GetEvents(EventQueue &result)
                {
                    BaseCat::System::Threading::ScopedLock<
                        BaseCat::System::Threading::CriticalSection> lock(lock_);

                    if (locked_ == false)
                    {
                        return false;
                    }

#pragma warning(disable:4127)
                    while (true)
#pragma warning(default:4127)
                    {
                        if (queue_.empty() == false)
                        {
                            IEventRunnable *item = queue_.front();
                            if (item->GetSeq() == processedSeq_)
                            {
                                ++processedSeq_;
                                result.push_back(item);
                                queue_.pop_front();
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (result.empty() == true)
                    {
                        locked_ = false;
                    }

                    return (result.empty() == false);
                }
            };

        private:
            Common::ThreadPool *threadPool_;

        public:
            ExecutorFilter(Common::ThreadPool *threadPool)
                : Common::IFilter(FILTER_NAME), threadPool_(threadPool)
            {}
            ~ExecutorFilter(void)
            {}

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                using namespace BaseCat;

                class DoOnOpened : public IEventRunnable
                {
                private:
                    Network::Filter::Handle next_;
                    Network::Link::Handle link_;

                public:
                    DoOnOpened(__uint64 seq,
                        Network::Filter::Handle next, Network::Link::Handle link)
                        : IEventRunnable(seq), next_(next), link_(link)
                    {
                        Network::Link::AddRef(link_);
                    }
                    virtual ~DoOnOpened(void)
                    {
                        Network::Link::Release(link_);
                    }

                    virtual bool Process(void)
                    {
                        Network::Filter::DoOnLinkOpened(next_, link_);

                        return true;
                    }
                };

                Context *context = new Context;
                Network::Link::AddAttribute(link, Context::CONTEXT_NAME, context);
                QueueFilterEvent(context, new DoOnOpened(context->GetSeq(), next, link));
            }
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                using namespace BaseCat;

                class DoOnClosed : public IEventRunnable
                {
                private:
                    Network::Filter::Handle next_;
                    Network::Link::Handle link_;

                public:
                    DoOnClosed(__uint64 seq,
                        Network::Filter::Handle next, Network::Link::Handle link)
                        : IEventRunnable(seq), next_(next), link_(link)
                    {
                        Network::Link::AddRef(link_);
                    }
                    virtual ~DoOnClosed(void)
                    {
                        Context *context = static_cast<Context *>(
                            Network::Link::GetAttribute(link_, Context::CONTEXT_NAME));
                        Network::Link::RemoveAttribute(link_, Context::CONTEXT_NAME);
                        delete context;

                        Network::Link::Release(link_);
                    }

                    virtual bool Process(void)
                    {
                        Network::Filter::DoOnLinkClosed(next_, link_);

                        return false;
                    }
                };

                Context *context = static_cast<Context *>(
                    Network::Link::GetAttribute(link, Context::CONTEXT_NAME));
                QueueFilterEvent(context, new DoOnClosed(context->GetSeq(), next, link));
            }
            virtual void OnSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size)
            {
                using namespace BaseCat;

                class DoOnSend : public IEventRunnable
                {
                private:
                    Network::Filter::Handle next_;
                    Network::Link::Handle link_;
                    System::SmartHeap::Block buffer_;
                    size_t size_;

                public:
                    DoOnSend(__uint64 seq,
                        Network::Filter::Handle next, Network::Link::Handle link,
                        System::SmartHeap::Block buffer, size_t size)
                        : IEventRunnable(seq), next_(next),
                        link_(link), buffer_(buffer), size_(size)
                    {
                        Network::Link::AddRef(link_);
                    }
                    virtual ~DoOnSend(void)
                    {
                        Network::Link::Release(link_);
                    }

                    virtual bool Process(void)
                    {
                        Network::Filter::DoOnSend(next_, link_, buffer_, size_);

                        return true;
                    }
                };

                Context *context = static_cast<Context *>(
                    Network::Link::GetAttribute(link, Context::CONTEXT_NAME));
                QueueFilterEvent(context, new DoOnSend(context->GetSeq(),
                                                    next, link, buffer, size));
            }
            virtual void OnReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size)
            {
                using namespace BaseCat;

                class DoOnReceived : public IEventRunnable
                {
                private:
                    Network::Filter::Handle next_;
                    Network::Link::Handle link_;
                    System::SmartHeap::Block buffer_;
                    size_t size_;

                public:
                    DoOnReceived(__uint64 seq,
                        Network::Filter::Handle next, Network::Link::Handle link,
                        System::SmartHeap::Block buffer, size_t size)
                        : IEventRunnable(seq), next_(next),
                        link_(link), buffer_(buffer), size_(size)
                    {
                        Network::Link::AddRef(link_);
                    }
                    virtual ~DoOnReceived(void)
                    {
                        Network::Link::Release(link_);
                    }

                    virtual bool Process(void)
                    {
                        Network::Filter::DoOnReceived(next_, link_, buffer_, size_);

                        return true;
                    }
                };

                Context *context = static_cast<Context *>(
                    Network::Link::GetAttribute(link, Context::CONTEXT_NAME));
                QueueFilterEvent(context, new DoOnReceived(context->GetSeq(),
                                                    next, link, buffer, size));
            }

        private:
            void QueueFilterEvent(Context *context, IEventRunnable *item)
            {
                context->PushEvent(item);
                threadPool_->QueueWorkItem(&DoFilterEvent, context);
            }
            static unsigned int __stdcall DoFilterEvent(void *data)
            {
                if (data == NULL)
                {
                    return 0;
                }

                Context *context = static_cast<Context *>(data);
                if (context->TryLock() == false)
                {
                    return 0;
                }

                bool exit = false;
                while (exit == false)
                {
                    EventQueue items;
                    if (context->GetEvents(items) == false)
                    {
                        break;
                    }

                    for (EventQueue::iterator it = items.begin();
                        it != items.end(); ++it)
                    {
                        IEventRunnable *item = *it;
                        if (item->Process() == false)
                        {
                            context->Unlock();
                            exit = true;
                        }
                        delete item;

                        if (exit == true)
                        {
                            break;
                        }
                    }
                    items.clear();
                }

                return 0;
            }
        };

        const char * const ExecutorFilter::FILTER_NAME = "SpringCat::Filter::ExecutorFilter";
        const char * const ExecutorFilter::Context::CONTEXT_NAME = "SpringCat::Filter::ExecutorFilter::Context";
    }
}

#endif
