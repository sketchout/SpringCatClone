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

#ifndef __BaseCat_Network_Connector__
#define __BaseCat_Network_Connector__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <BaseCat/System.h>

namespace BaseCat
{
    namespace Network
    {
        namespace Connector
        {
            typedef void (__stdcall *Event)(Handle, Link::Handle, void *);
            typedef void (__stdcall *EventEx)(Handle, Link::Handle, void *, void *);

            NETWORK_API Handle __stdcall CreateHandle(System::ThreadPool::Handle threadPool);
            NETWORK_API Handle __stdcall CreateHandle(System::ThreadPool::Handle threadPool, FilterChain::Handle filterChain);
            NETWORK_API void __stdcall DestroyHandle(Handle connector);

            NETWORK_API bool __stdcall Connect(Handle connector, const char *ipAddress, unsigned short port);
            NETWORK_API bool __stdcall Connect(Handle connector, const char *ipAddress, unsigned short port, int timeout);
            NETWORK_API bool __stdcall Connect(Handle connector, const char *ipAddress, unsigned short port, int timeout, Interrupter::Handle interrupter);

            NETWORK_API bool __stdcall ConnectEx(Handle connector, const char *ipAddress, unsigned short port, void *contextEx);
            NETWORK_API bool __stdcall ConnectEx(Handle connector, const char *ipAddress, unsigned short port, int timeout, void *contextEx);
            NETWORK_API bool __stdcall ConnectEx(Handle connector, const char *ipAddress, unsigned short port, int timeout, Interrupter::Handle interrupter, void *contextEx);

            NETWORK_API ActiveLinkList::Handle __stdcall LockActiveLinks(Handle connector);
            NETWORK_API bool __stdcall UnlockActiveLinks(Handle connector);

            NETWORK_API bool __stdcall SetOnConnected(Handle connector, const Event &callback, void *context);
            NETWORK_API bool __stdcall SetOnConnectedEx(Handle connector, const EventEx &callback, void *context);
            NETWORK_API bool __stdcall SetOnReceived(Handle connector, const Event &callback, void *context);
            NETWORK_API bool __stdcall SetOnDisconnected(Handle connector, const Event &callback, void *context);
        }
    }
}

#endif
