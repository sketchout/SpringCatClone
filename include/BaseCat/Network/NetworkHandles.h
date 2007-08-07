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

#ifndef __BaseCat_Network_NetworkHandles__
#define __BaseCat_Network_NetworkHandles__

namespace BaseCat
{
    namespace Network
    {
        namespace Interrupter
        {
            typedef struct _Handle
            {
            } *Handle;
        }
        namespace Filter
        {
            typedef struct _Handle
            {
            } *Handle;
        }
        namespace FilterChain
        {
            typedef struct _Handle
            {
            } *Handle;
        }
        namespace Address
        {
            typedef struct _Handle
            {
            } *Handle;
        }
        namespace Link
        {
            typedef struct _Handle
            {
            } *Handle;
        }
        namespace ActiveLinkList
        {
            typedef struct _Handle
            {
            } *Handle;
        }
        namespace Acceptor
        {
            typedef struct _Handle
            {
            } *Handle;
        }
        namespace Connector
        {
            typedef struct _Handle
            {
            } *Handle;
        }
        namespace UDPListener
        {
            typedef struct _Handle
            {
            } *Handle;
        }
    }
}

#endif
