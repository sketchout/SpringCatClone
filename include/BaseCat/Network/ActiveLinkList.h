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

#ifndef __BaseCat_Network_ActiveLinkList__
#define __BaseCat_Network_ActiveLinkList__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <cstddef>

namespace BaseCat
{
    namespace Network
    {
        namespace ActiveLinkList
        {
            NETWORK_API Link::Handle __stdcall GetLink(Handle activeLinkList, size_t index);
            NETWORK_API size_t __stdcall GetSize(Handle activeLinkList);
        }
    }
}

#endif
