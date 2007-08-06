//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Transport_InputStream__
#define __SpringCat_Transport_InputStream__

#include <SpringCat/Common/SmallObject.h>
#include <BaseCat/System.h>

namespace SpringCat
{
    namespace Transport
    {
        template<typename LinkT = Link>
        class InputStream : public Common::SmallObject<InputStream<LinkT> >
        {
        private:
            LinkT *link_;
            BaseCat::System::Stream::Handle stream_;

        public:
            InputStream(LinkT *link)
                : link_(link), stream_(link->LockReadStream())
            {}
            ~InputStream(void)
            {
                link_->UnlockReadStream();
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

            size_t Read(void *p, size_t size)
            {
                return BaseCat::System::Stream::Read(stream_, p, size);
            }

            size_t Peek(void *p, size_t size)
            {
                return BaseCat::System::Stream::Peek(stream_, p, size);
            }

            size_t RemoveLeft(size_t size)
            {
                return BaseCat::System::Stream::RemoveLeft(stream_, size);
            }

            size_t RemoveRight(size_t size)
            {
                return BaseCat::System::Stream::RemoveRight(stream_, size);
            }
        };
    }
}

#endif
