//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Transport_OutputStream__
#define __SpringCat_Transport_OutputStream__

#include <BaseCat/System.h>

namespace SpringCat
{
    namespace Transport
    {
        template<typename LinkT = Link>
        class OutputStream : private BaseCat::System::TL::NonCopyable
        {
        private:
            LinkT *link_;
            BaseCat::System::Stream::Handle stream_;

        public:
            OutputStream(LinkT *link)
                : link_(link), stream_(BaseCat::System::Stream::CreateHandle())
            {}
            ~OutputStream(void)
            {
                Flush();

                BaseCat::System::Stream::DestroyHandle(stream_);
                stream_ = NULL;
            }

            void Attach(BaseCat::System::SmartHeap::Block p)
            {
                BaseCat::System::Stream::Attach(stream_, p);
            }

            BaseCat::System::SmartHeap::Block Detach(void)
            {
                return BaseCat::System::Stream::Detach(stream_);
            }

            BaseCat::System::SmartHeap::Block GetRawPtr(void)
            {
                return BaseCat::System::Stream::GetRawPtr(stream_);
            }

            size_t GetLength(void) const
            {
                return BaseCat::System::Stream::GetLength(stream_);
            }

            void Clear(void)
            {
                BaseCat::System::Stream::Clear(stream_);
            }

            void Flush(void)
            {
                link_->Send(GetRawPtr(), GetLength());
                Clear();
            }

            size_t Write(const void *p, size_t size)
            {
                return BaseCat::System::Stream::Write(stream_, p, size);
            }
        };
    }
}

#endif
