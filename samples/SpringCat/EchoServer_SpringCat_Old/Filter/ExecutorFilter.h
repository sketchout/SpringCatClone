#ifndef __SpringCat_Filter_ExecutorFilter__
#define __SpringCat_Filter_ExecutorFilter__

#include "Common/Filter.h"

namespace SpringCat
{
    namespace Common
    {
        class FilterChain;
        class ThreadPool;
    }

    namespace Filter
    {
        class ExecutorFilter : public Common::IFilter
        {
        private:
            struct Context;
            struct Impl;
            std::auto_ptr<Impl> impl_;

        public:
            ExecutorFilter(Common::FilterChain *chain, Common::ThreadPool *threadPool);
            ~ExecutorFilter(void);

        protected:
            virtual void OnOpened(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link);
            virtual void OnClosed(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link);
            virtual void OnSend(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size);
            virtual void OnReceived(BaseCat::Network::Filter::Handle next,
                BaseCat::Network::Link::Handle link,
                BaseCat::System::SmartHeap::Block buffer, size_t size);
        };
    }
}

#endif
