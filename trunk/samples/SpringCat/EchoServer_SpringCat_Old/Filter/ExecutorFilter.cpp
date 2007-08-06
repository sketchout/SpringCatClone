#include "ExecutorFilter.h"
#include "Common/ThreadPool.h"
#include "Transport/Link.h"

namespace SpringCat
{
    namespace Filter
    {
        class IFilterEventRunnable
        {
        public:
            virtual ~IFilterEventRunnable(void) {}

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

        struct ExecutorFilter::Context
        {
        public:
            static const char * const filterName;
            static const char * const contextName;

        private:
            typedef BaseCat::System::TL::queue<IFilterEventRunnable *> Queue;

        private:
            Queue queue_;
            BaseCat::System::Threading::CriticalSection lock_;

        public:
            Context(void)
            {}
            ~Context(void)
            {}

            void PushEvent(IFilterEventRunnable *event)
            {
                BaseCat::System::Threading::ScopedLock<BaseCat::System::Threading::CriticalSection> loc(lock_);
                queue_.push(event);
            }
            IFilterEventRunnable *GetEvent(void)
            {
                IFilterEventRunnable *result = NULL;

                {
                    BaseCat::System::Threading::ScopedLock<BaseCat::System::Threading::CriticalSection> loc(lock_);
                    if (queue_.empty() == false)
                    {
                        result = queue_.front();
                        queue_.pop();
                    }
                }

                return result;
            }
        };

        const char * const ExecutorFilter::Context::filterName = "ExecutorFilter";
        const char * const ExecutorFilter::Context::contextName = "ExecutorFilterContext";

        struct ExecutorFilter::Impl
        {
        private:
            Common::ThreadPool *threadPool_;

        public:
            Impl(Common::ThreadPool *threadPool)
                : threadPool_(threadPool)
            {
                if (threadPool_ == NULL)
                {
                    throw std::exception();
                }
            }

            void QueueFilterEvent(Context *context)
            {
                threadPool_->QueueWorkItem(&DoFilterEvent, context);
            }

        private:
            static unsigned int __stdcall DoFilterEvent(void *data)
            {
                if (data == NULL)
                {
                    return 0;
                }

                Context *context = static_cast<Context *>(data);

                IFilterEventRunnable *item = context->GetEvent();
                item->Process();
                delete item;

                return 0;
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

        ExecutorFilter::ExecutorFilter(Common::FilterChain *chain, Common::ThreadPool *threadPool)
            : Common::IFilter(chain, Context::filterName), impl_(new Impl(threadPool))
        {}

        ExecutorFilter::~ExecutorFilter(void)
        {}

        void ExecutorFilter::OnOpened(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link)
        {
            class DoOnOpened : public IFilterEventRunnable
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

            context->PushEvent(new DoOnOpened(next, link));
            impl_->QueueFilterEvent(context);
        }

        void ExecutorFilter::OnClosed(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link)
        {
            class DoOnClosed : public IFilterEventRunnable
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
                    BaseCat::Network::Link::Release(link_);
                }

                virtual void Process(void)
                {
                    BaseCat::Network::Filter::DoOnLinkClosed(next_, link_);

                    Context *context = static_cast<Context *>(BaseCat::Network::Link::GetAttribute(link_, Context::contextName));
                    BaseCat::Network::Link::RemoveAttribute(link_, Context::contextName);
                    delete context;
                }
            };

            Context *context = static_cast<Context *>(BaseCat::Network::Link::GetAttribute(link, Context::contextName));

            context->PushEvent(new DoOnClosed(next, link));
            impl_->QueueFilterEvent(context);
        }

        void ExecutorFilter::OnSend(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link,
            BaseCat::System::SmartHeap::Block buffer, size_t size)
        {
            class DoOnSend : public IFilterEventRunnable
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

            context->PushEvent(new DoOnSend(next, link, buffer, size));
            impl_->QueueFilterEvent(context);
        }

        void ExecutorFilter::OnReceived(BaseCat::Network::Filter::Handle next,
            BaseCat::Network::Link::Handle link,
            BaseCat::System::SmartHeap::Block buffer, size_t size)
        {
            class DoOnReceived : public IFilterEventRunnable
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

            context->PushEvent(new DoOnReceived(next, link, buffer, size));
            impl_->QueueFilterEvent(context);
        }
    }
}


//namespace SpringCat
//{
//    namespace Filter
//    {
//        struct ExecutorFilter::Context
//        {
//            static const char * const filterName;
//            static const char * const contextName;
//        };
//
//        const char * const ExecutorFilter::Context::filterName = "ExecutorFilter";
//        const char * const ExecutorFilter::Context::contextName = "ExecutorFilterContext";
//
//        class IFilterEventRunnable
//        {
//        public:
//            virtual ~IFilterEventRunnable(void) {}
//
//            virtual void Process(void) = 0;
//
//        public:
//            // static
//            static void *operator new(size_t size)
//            {
//                if (size == 0)
//                {
//                    size = 1;
//                }
//
//                void *result = NULL;
//
//#pragma warning(disable:4127)
//                while (true)
//#pragma warning(default:4127)
//                {
//                    result = BaseCat::System::MPHeap::Alloc(NULL, size);
//                    if (result != NULL)
//                    {
//                        break;
//                    }
//
//                    std::new_handler newHandler = std::set_new_handler(0);
//                    std::set_new_handler(newHandler);
//
//                    if (newHandler != NULL)
//                    {
//                        (*newHandler)();
//                    }
//                    else
//                    {
//                        throw std::bad_alloc();
//                    }
//                }
//
//                return result;
//            }
//
//            // static
//            static void operator delete(void *p) throw()
//            {
//                if (p == NULL)
//                {
//                    return;
//                }
//
//                BaseCat::System::MPHeap::Free(NULL, p);
//                p = NULL;
//            }
//        };
//
//        struct ExecutorFilter::Impl
//        {
//        private:
//            Common::ThreadPool *threadPool_;
//
//        public:
//            Impl(Common::ThreadPool *threadPool)
//                : threadPool_(threadPool)
//            {
//                if (threadPool_ == NULL)
//                {
//                    throw std::exception();
//                }
//            }
//
//            void QueueFilterEvent(IFilterEventRunnable *item)
//            {
//                threadPool_->QueueWorkItem(&DoFilterEvent, item);
//            }
//
//        private:
//            static unsigned int __stdcall DoFilterEvent(void *data)
//            {
//                if (data == NULL)
//                {
//                    return 0;
//                }
//
//                IFilterEventRunnable *item = static_cast<IFilterEventRunnable *>(data);
//                item->Process();
//                delete item;
//
//                return 0;
//            }
//
//        public:
//            // static
//            static void *operator new(size_t size)
//            {
//                if (size == 0)
//                {
//                    size = 1;
//                }
//
//                void *result = NULL;
//
//#pragma warning(disable:4127)
//                while (true)
//#pragma warning(default:4127)
//                {
//                    result = BaseCat::System::MPHeap::Alloc(NULL, size);
//                    if (result != NULL)
//                    {
//                        break;
//                    }
//
//                    std::new_handler newHandler = std::set_new_handler(0);
//                    std::set_new_handler(newHandler);
//
//                    if (newHandler != NULL)
//                    {
//                        (*newHandler)();
//                    }
//                    else
//                    {
//                        throw std::bad_alloc();
//                    }
//                }
//
//                return result;
//            }
//
//            // static
//            static void operator delete(void *p) throw()
//            {
//                if (p == NULL)
//                {
//                    return;
//                }
//
//                BaseCat::System::MPHeap::Free(NULL, p);
//                p = NULL;
//            }
//        };
//
//        ExecutorFilter::ExecutorFilter(Common::FilterChain *chain, Common::ThreadPool *threadPool)
//            : Common::IFilter(chain, Context::filterName), impl_(new Impl(threadPool))
//        {}
//
//        ExecutorFilter::~ExecutorFilter(void)
//        {}
//
//        void ExecutorFilter::OnOpened(BaseCat::Network::Filter::Handle next,
//            BaseCat::Network::Link::Handle link)
//        {
//            class DoOnOpened : public IFilterEventRunnable
//            {
//            private:
//                BaseCat::Network::Filter::Handle next_;
//                Transport::LinkPtr link_;
//
//            public:
//                DoOnOpened(BaseCat::Network::Filter::Handle next, const Transport::LinkPtr &link)
//                    : next_(next), link_(link)
//                {}
//                virtual ~DoOnOpened(void)
//                {}
//
//                virtual void Process(void)
//                {
//                    BaseCat::Network::Filter::DoOnLinkOpened(next_, link_->GetHandle());
//                }
//            };
//
//            impl_->QueueFilterEvent(new DoOnOpened(next, new Transport::Link(link)));
//        }
//
//        void ExecutorFilter::OnClosed(BaseCat::Network::Filter::Handle next,
//            BaseCat::Network::Link::Handle link)
//        {
//            class DoOnClosed : public IFilterEventRunnable
//            {
//            private:
//                BaseCat::Network::Filter::Handle next_;
//                Transport::LinkPtr link_;
//
//            public:
//                DoOnClosed(BaseCat::Network::Filter::Handle next, const Transport::LinkPtr &link)
//                    : next_(next), link_(link)
//                {}
//                virtual ~DoOnClosed(void)
//                {}
//
//                virtual void Process(void)
//                {
//                    BaseCat::Network::Filter::DoOnLinkClosed(next_, link_->GetHandle());
//                }
//            };
//
//            impl_->QueueFilterEvent(new DoOnClosed(next, new Transport::Link(link)));
//        }
//
//        void ExecutorFilter::OnSend(BaseCat::Network::Filter::Handle next,
//            BaseCat::Network::Link::Handle link,
//            BaseCat::System::SmartHeap::Block buffer, size_t size)
//        {
//            class DoOnSend : public IFilterEventRunnable
//            {
//            private:
//                BaseCat::Network::Filter::Handle next_;
//                Transport::LinkPtr link_;
//                BaseCat::System::SmartHeap::Block buffer_;
//                size_t size_;
//
//            public:
//                DoOnSend(BaseCat::Network::Filter::Handle next,
//                    const Transport::LinkPtr &link,
//                    BaseCat::System::SmartHeap::Block buffer, size_t size)
//                    : next_(next), link_(link), buffer_(buffer), size_(size)
//                {}
//                virtual ~DoOnSend(void)
//                {}
//
//                virtual void Process(void)
//                {
//                    BaseCat::Network::Filter::DoOnSend(next_, link_->GetHandle(), buffer_, size_);
//                }
//            };
//
//            impl_->QueueFilterEvent(new DoOnSend(next, new Transport::Link(link), buffer, size));
//        }
//
//        void ExecutorFilter::OnReceived(BaseCat::Network::Filter::Handle next,
//            BaseCat::Network::Link::Handle link,
//            BaseCat::System::SmartHeap::Block buffer, size_t size)
//        {
//            class DoOnReceived : public IFilterEventRunnable
//            {
//            private:
//                BaseCat::Network::Filter::Handle next_;
//                Transport::LinkPtr link_;
//                BaseCat::System::SmartHeap::Block buffer_;
//                size_t size_;
//
//            public:
//                DoOnReceived(BaseCat::Network::Filter::Handle next,
//                    const Transport::LinkPtr &link,
//                    BaseCat::System::SmartHeap::Block buffer, size_t size)
//                    : next_(next), link_(link), buffer_(buffer), size_(size)
//                {}
//                virtual ~DoOnReceived(void)
//                {}
//
//                virtual void Process(void)
//                {
//                    BaseCat::Network::Filter::DoOnReceived(next_, link_->GetHandle(), buffer_, size_);
//                }
//            };
//
//            impl_->QueueFilterEvent(new DoOnReceived(next, new Transport::Link(link), buffer, size));
//        }
//    }
//}
