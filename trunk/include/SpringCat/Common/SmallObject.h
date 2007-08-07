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

#ifndef __SpringCat_Common_SmallObject__
#define __SpringCat_Common_SmallObject__

#include <BaseCat/System.h>

namespace SpringCat
{
    namespace Common
    {
        template<typename T>
        class SmallObject : private BaseCat::System::TL::NonCopyable
        {
        public:
            static void *operator new(size_t size)
            {
                if (size == 0)
                {
                    size = 1;
                }

                void *result = NULL;

#pragma warning(disable:4127)
                while (true)
#pragma warning(default:4127)
                {
                    result = BaseCat::System::MPHeap::Alloc(NULL, size);
                    if (result != NULL)
                    {
                        break;
                    }

                    std::new_handler newHandler = std::set_new_handler(0);
                    std::set_new_handler(newHandler);

                    if (newHandler != NULL)
                    {
                        (*newHandler)();
                    }
                    else
                    {
                        throw std::bad_alloc();
                    }
                }

                return result;
            }
            static void operator delete(void *p) throw()
            {
                if (p == NULL)
                {
                    return;
                }

                BaseCat::System::MPHeap::Free(NULL, p);
                p = NULL;
            }
        };
    }
}

#endif
