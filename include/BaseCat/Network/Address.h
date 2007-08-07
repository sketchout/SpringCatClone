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

#ifndef __BaseCat_Network_Address__
#define __BaseCat_Network_Address__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include "Consts.h"
#include <cstddef>

namespace BaseCat
{
    namespace Network
    {
        namespace Address
        {
            NETWORK_API Handle __stdcall CreateHandle(const char *ipAddress, unsigned short port, Family::Type addressFamily = Family::IPv4);
            NETWORK_API Handle __stdcall CreateHandle(unsigned long ipAddress, unsigned short port, Family::Type addressFamily = Family::IPv4);
            NETWORK_API void __stdcall DestroyHandle(Handle address);

            NETWORK_API bool __stdcall GetIPAddress(Handle address, char *ipAddress, size_t maxLen);
            NETWORK_API bool __stdcall GetPort(Handle address, unsigned short *port);
            NETWORK_API bool __stdcall GetFamily(Handle address, Family::Type *family);
        }
    }
}

#endif
