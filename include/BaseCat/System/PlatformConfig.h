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

#ifndef __BaseCat_System_PlatformConfig__
#define __BaseCat_System_PlatformConfig__

#if defined(_WIN32) || defined(_WIN64)
    typedef unsigned __int64 __uint64;
#else
    #include <stdint.h>

    #define _UNIX_

    #define __stdcall
    #define INFINITE        0

    typedef int64_t __int64;
    typedef uint64_t __uint64;

    template<typename T>
    inline T __max(T a, T b)
    {
        return (((a) > (b)) ? (a) : (b));
    }

    template<typename T>
    inline T __min(T a, T b)
    {
        return (((a) < (b)) ? (a) : (b));
    }
#endif

#endif
