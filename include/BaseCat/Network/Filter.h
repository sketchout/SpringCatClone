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

#ifndef __BaseCat_Network_Filter__
#define __BaseCat_Network_Filter__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <BaseCat/System.h>

namespace BaseCat
{
    namespace Network
    {
        namespace Filter
        {
            typedef void (__stdcall *Event)(Handle, Link::Handle, void *);
            typedef void (__stdcall *IOEvent)(Handle, Link::Handle, System::SmartHeap::Block, size_t, void *);

            NETWORK_API Handle __stdcall CreateHandle(void);
            NETWORK_API void __stdcall DestroyHandle(Handle filter);

            NETWORK_API Handle __stdcall GetPrev(Handle filter);
            NETWORK_API Handle __stdcall GetNext(Handle filter);

            NETWORK_API bool __stdcall SetOnLinkOpened(Handle filter, const Event &callback, void *context);
            NETWORK_API bool __stdcall SetOnLinkClosed(Handle filter, const Event &callback, void *context);

            NETWORK_API bool __stdcall SetOnSend(Handle filter, const IOEvent &callback, void *context);
            NETWORK_API bool __stdcall SetOnReceived(Handle filter, const IOEvent &callback, void *context);

            NETWORK_API bool __stdcall DoOnLinkOpened(Handle filter, Link::Handle link);
            NETWORK_API bool __stdcall DoOnLinkClosed(Handle filter, Link::Handle link);

            NETWORK_API bool __stdcall DoOnSend(Handle filter, Link::Handle link, System::SmartHeap::Block buffer, size_t size);
            NETWORK_API bool __stdcall DoOnReceived(Handle filter, Link::Handle link, System::SmartHeap::Block buffer, size_t size);
        }
    }
}

#endif
