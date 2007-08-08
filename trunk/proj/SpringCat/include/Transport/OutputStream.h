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

#ifndef __SpringCat_Transport_OutputStream__
#define __SpringCat_Transport_OutputStream__

#include <SpringCat/Common/SmallObject.h>
#include <BaseCat/System.h>

namespace SpringCat
{
    namespace Transport
    {
        template<typename LinkT = Link>
        class OutputStream : public Common::SmallObject<OutputStream<LinkT> >
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
                if (GetLength() > 0)
                {
                    link_->Send(GetRawPtr(), GetLength());
                    Clear();
                }
            }

            size_t Write(const void *p, size_t size)
            {
                return BaseCat::System::Stream::Write(stream_, p, size);
            }
        };
    }
}

#endif
