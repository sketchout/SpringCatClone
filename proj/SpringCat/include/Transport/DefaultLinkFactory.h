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

#ifndef __SpringCat_Transport_DefaultLinkFactory__
#define __SpringCat_Transport_DefaultLinkFactory__

#include <BaseCat/System.h>
#include <BaseCat/Network.h>

namespace SpringCat
{
    namespace Transport
    {
        template<typename LinkT>
        struct DefaultLinkFactory : private BaseCat::System::TL::NonCopyable
        {
            static LinkT *Alloc(BaseCat::Network::Link::Handle link)
            {
                return new LinkT(link);
            }
            static void Free(LinkT *linkPtr)
            {
                delete linkPtr;
                linkPtr = NULL;
            }
        };
    }
}

#endif
