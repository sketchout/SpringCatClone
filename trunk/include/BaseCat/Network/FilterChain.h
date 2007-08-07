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

#ifndef __BaseCat_Network_FilterChain__
#define __BaseCat_Network_FilterChain__

#include "NetworkBase.h"
#include "NetworkHandles.h"
#include <BaseCat/System.h>

namespace BaseCat
{
    namespace Network
    {
        namespace FilterChain
        {
            NETWORK_API Handle __stdcall CreateHandle(void);
            NETWORK_API void __stdcall DestroyHandle(Handle filterChain);

            NETWORK_API bool __stdcall Add(Handle filterChain, const char *name, Filter::Handle filter);
            NETWORK_API bool __stdcall Remove(Handle filterChain, const char *name);
            NETWORK_API Filter::Handle __stdcall Find(Handle filterChain, const char *name);
        }
    }
}

#endif
