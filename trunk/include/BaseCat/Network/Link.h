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

#ifndef __BaseCat_Network_Link__
#define __BaseCat_Network_Link__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <BaseCat/System.h>

namespace BaseCat
{
    namespace Network
    {
        namespace Link
        {
            typedef void (__stdcall *Event)(Handle, void *);

            NETWORK_API int __stdcall AddRef(Handle link);
            NETWORK_API int __stdcall Release(Handle link);

            NETWORK_API bool __stdcall Close(Handle link);

            NETWORK_API Address::Handle __stdcall GetLocalAddress(Handle link);
            NETWORK_API Address::Handle __stdcall GetRemoteAddress(Handle link);

            NETWORK_API bool __stdcall Send(Handle link, const void *data, size_t len);
            NETWORK_API bool __stdcall Send(Handle link, const System::SmartHeap::Block data);
            NETWORK_API bool __stdcall Send(Handle link, const System::SmartHeap::Block data, size_t len);
            NETWORK_API bool __stdcall Recv(Handle link, size_t len);

            NETWORK_API bool __stdcall SetData(Handle link, void *data);
            NETWORK_API bool __stdcall SetData(Handle link, void *data, const Event &onDestroy, void *context);
            NETWORK_API void * __stdcall GetData(Handle link);

            NETWORK_API bool __stdcall AddAttribute(Handle link, const char *name, void *data);
            NETWORK_API bool __stdcall RemoveAttribute(Handle link, const char *name);
            NETWORK_API void * __stdcall GetAttribute(Handle link, const char *name);

            NETWORK_API System::Stream::Handle __stdcall LockReadStream(Handle link);
            NETWORK_API bool __stdcall UnlockReadStream(Handle link);

            //NETWORK_API bool __stdcall SetSendBufferSize(Handle link, int value);
            //NETWORK_API int __stdcall GetSendBufferSize(Handle link);
            //NETWORK_API bool __stdcall SetRecvBufferSize(Handle link, int value);
            //NETWORK_API int __stdcall GetRecvBufferSize(Handle link);

            NETWORK_API bool __stdcall SetNoDelay(Handle link, bool value);
            NETWORK_API bool __stdcall GetNoDelay(Handle link, bool *value);
        }
    }
}

#endif
