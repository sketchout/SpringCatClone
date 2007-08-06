#ifndef __SpringCat_Filter_ExecutorFilter__
#define __SpringCat_Filter_ExecutorFilter__

#include "Common/Filter.h"

namespace SpringCat
{
    namespace Filter
    {
        class ExecutorFilter : public Common::IFilter
        {
        private:
            class IEventRunnable
            {
            public:
                virtual ~IEventRunnable(void) {}

                virtual void Process(void) = 0;

            public:
                // static
                static void *operator new(size_t size)
                {
                    if (size == 0)
                    {
                        size = 1;
                    }

                    void *result = NULL;

#pragma warning(disable:4127)
                    while (true)
#pragma warning(default:4127)
                    {
                        result = BaseCat::System::MPHeap::Alloc(NULL, size);
                        if (result != NULL)
                        {
                            break;
                        }

                        std::new_handler newHandler = std::set_new_handler(0);
                        std::set_new_handler(newHandler);

                        if (newHandler != NULL)
                        {
                            (*newHandler)();
                        }
                        else
                        {
                            throw std::bad_alloc();
                        }
                    }

                    return result;
                }

                // static
                static void operator delete(void *p) throw()
                {
                    if (p == NULL)
                    {
                        return;
                    }

                    BaseCat::System::MPHeap::Free(NULL, p);
                    p = NULL;
                }
            };

            struct Context
            {
            public:
                static const char * const filterName;
                static const char * const contextName;

            private:
                typedef BaseCat::System::TL::queue<IEventRunnable *> Queue;

            private:
                Queue queue_;
                BaseCat::System::Threading::CriticalSection lock_;

            public:
                Context(void)
                {}
                ~Context(void)
                {}

                void Lock(void)
                {
                    lock_.Lock();
                }
                void Unlock(void)
                {
                    lock_.Unlock();
                }

                void PushEvent(IEventRunnable *event)
                {
                    BaseCat::System::Threading::ScopedLock<Context> lock(this);
                    queue_.push(event);
                }
                IEventRunnable *GetEvent(void)
                {
                    IEventRunnable *result = NULL;

                    {
                        BaseCat::System::Threading::ScopedLock<Context> lock(this);
                        if (queue_.empty() == false)
                        {
                            result = queue_.front();
                            queue_.pop();
                        }
                    }

                    return result;
                }

            public:
                // static
                static void *operator new(size_t size)
                {
                    if (size == 0)
                    {
                        size = 1;
                    }

                    void *result = NULL;

#pragma warning(disable:4127)
                    while (true)
#pragma warning(default:4127)
                    {
                        result = BaseCat::System::MPHeap::Alloc(NULL, size);
                        if (result != NULL)
                        {
                            break;
                        }

                        std::new_handler newHandler = std::set_new_handler(0);
                        std::set_new_handler(newHandler);

                        if (newHandler != NULL)
                        {
                            (*newHandler)();
                        }
                        else
                        {
                            throw std::bad_alloc();
                        }
                    }

                    return result;
                }

                // static
                static void operator delete(void *p) throw()
                {
                    if (p == NULL)
                    {
                        return;
                    }

                    BaseCat::System::MPHeap::Free(NULL, p);
                    p = NULL;
                }
            };

        private:
            Common::ThreadPool *threadPool_;

        public:
            ExecutorFilter(Common::ThreadPool *threadPool)
                : Common::IFilter(Context::filterName), threadPool_(threadPool)
            {}
            ~ExecutorFilter(void)
            {}

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                class DoOnOpened : public IEventRunnable
                {
                private:
                    BaseCat::Network::Filter::Handle next_;
                    BaseCat::Network::Link::Handle link_;

                public:
                    DoOnOpened(BaseCat::Network::Filter::Handle next, BaseCat::Network::Link::Handle link)
                        : next_(next), link_(link)
                    {
                        BaseCat::Network::Link::AddRef(link_);
                    }
                    virtual ~DoOnOpened(void)
                    {
                        BaseCat::Network::Link::Release(link_);
                    }

                    virtual void Process(void)
                    {
                        BaseCat::Network::Filter::DoOnLinkOpened(next_, link_);
                    }
                };

                Context *context = new Context;
                BaseCat::Network::Link::AddAttribute(link, Context::contextName, context);
                QueueFilterEvent(context, new DoOnOpened(next, link));
            }
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link)
            {
                class DoOnClosed : public IEventRunnable
                {
                private:
                    BaseCat::Network::Filter::Handle next_;
                    BaseCat::Network::Link::Handle link_;

                public:
                    DoOnClosed(BaseCat::Network::Filter::Handle next, BaseCat::Network::Link::Handle link)
                        : next_(next), link_(link)
                    {
                        BaseCat::Network::Link::AddRef(link_);
                    }
                    virtual ~DoOnClosed(void)
                    {
                        Context *context = static_cast<Context *>(BaseCat::Network::Link::GetAttribute(link_, Context::contextName));
                        BaseCat::Network::Link::RemoveAttribute(link_, Context::contextName);
                        delete context;

                        BaseCat::Network::Link::Release(link_);
                    }

                    virtual void Process(void)
                    {
                        BaseCat::Network::Filter::DoOnLinkClosed(next_, link_);
                    }
                };

                Context *context = static_cast<Context *>(BaseCat::Network::Link::GetAttribute(link, Context::contextName));
                QueueFilterEvent(context, new DoOnClosed(next, link));
            }
            virtual void OnSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size)
            {
                class DoOnSend : public IEventRunnable
                {
                private:
                    BaseCat::Network::Filter::Handle next_;
                    BaseCat::Network::Link::Handle link_;
                    BaseCat::System::SmartHeap::Block buffer_;
                    size_t size_;

                public:
                    DoOnSend(BaseCat::Network::Filter::Handle next,
                        BaseCat::Network::Link::Handle link,
                        BaseCat::System::SmartHeap::Block buffer, size_t size)
                        : next_(next), link_(link), buffer_(buffer), size_(size)
                    {
                        BaseCat::Network::Link::AddRef(link_);
                    }
                    virtual ~DoOnSend(void)
                    {
                        BaseCat::Network::Link::Release(link_);
                    }

                    virtual void Process(void)
                    {
                        BaseCat::Network::Filter::DoOnSend(next_, link_, buffer_, size_);
                    }
                };

                Context *context = static_cast<Context *>(BaseCat::Network::Link::GetAttribute(link, Context::contextName));
                QueueFilterEvent(context, new DoOnSend(next, link, buffer, size));
            }
            virtual void OnReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size)
            {
                class DoOnReceived : public IEventRunnable
                {
                private:
                    BaseCat::Network::Filter::Handle next_;
                    BaseCat::Network::Link::Handle link_;
                    BaseCat::System::SmartHeap::Block buffer_;
                    size_t size_;

                public:
                    DoOnReceived(BaseCat::Network::Filter::Handle next,
                        BaseCat::Network::Link::Handle link,
                        BaseCat::System::SmartHeap::Block buffer, size_t size)
                        : next_(next), link_(link), buffer_(buffer), size_(size)
                    {
                        BaseCat::Network::Link::AddRef(link_);
                    }
                    virtual ~DoOnReceived(void)
                    {
                        BaseCat::Network::Link::Release(link_);
                    }

                    virtual void Process(void)
                    {
                        BaseCat::Network::Filter::DoOnReceived(next_, link_, buffer_, size_);
                    }
                };

                Context *context = static_cast<Context *>(BaseCat::Network::Link::GetAttribute(link, Context::contextName));
                QueueFilterEvent(context, new DoOnReceived(next, link, buffer, size));
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

                IEventRunnable *item = context->GetEvent();
                item->Process();
                delete item;

                return 0;
            }
        };

        const char * const ExecutorFilter::Context::filterName = "ExecutorFilter";
        const char * const ExecutorFilter::Context::contextName = "ExecutorFilterContext";
    }
}

#endif
