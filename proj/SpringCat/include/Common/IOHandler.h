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

#ifndef __SpringCat_Common_IOHandler__
#define __SpringCat_Common_IOHandler__

#include <SpringCat/Common/SmallObject.h>

namespace SpringCat
{
    namespace Common
    {
        template<typename LinkT = Transport::Link>
        class IIOHandler : public SmallObject<IIOHandler<LinkT> >
        {
        public:
            virtual ~IIOHandler(void) {}

        public:
            virtual void OnOpened(LinkT *link) = 0;
            virtual void OnOpened(LinkT *link, void *context) = 0;
            virtual void OnClosed(LinkT *link) = 0;
            virtual void OnReceived(LinkT *link) = 0;
        };
    }
}

#endif
