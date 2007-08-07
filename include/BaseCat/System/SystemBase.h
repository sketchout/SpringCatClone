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

#ifndef __BaseCat_SystemBase__
#define __BaseCat_SystemBase__

#if defined(_WIN32) || defined(_WIN64)
    #ifdef SYSTEM_EXPORTS
        #define SYSTEM_API __declspec(dllexport)
    #else
        #define SYSTEM_API __declspec(dllimport)

        #ifndef _DEBUG
            #pragma comment(lib, "BaseCat.System.lib")
        #else
            #pragma comment(lib, "BaseCat.SystemD.lib")
        #endif
    #endif
#else
    #define SYSTEM_API
#endif

#endif
