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

#ifndef __BaseCat_System_TL_Allocator__
#define __BaseCat_System_TL_Allocator__

#include <BaseCat/System.h>

namespace BaseCat
{
    namespace System
    {
        namespace TL
        {
            template<typename T>
            class Allocator
            {   // generic Allocator for objects of class T
            public:
                typedef typename T value_type;
                typedef value_type* pointer;
                typedef value_type& reference;
                typedef const value_type* const_pointer;
                typedef const value_type& const_reference;

                typedef size_t size_type;
                typedef ptrdiff_t difference_type;

                template<class _Other>
                struct rebind
                {   // convert an Allocator<T> to an Allocator <_Other>
                    typedef Allocator<_Other> other;
                };

                pointer address(reference _Val) const
                {   // return address of mutable _Val
                    return (&_Val);
                }

                const_pointer address(const_reference _Val) const
                {   // return address of nonmutable _Val
                    return (&_Val);
                }

                Allocator()
                {   // construct default Allocator (do nothing)
                }

                Allocator(const Allocator<T>&)
                {   // construct by copying (do nothing)
                }

                template<class _Other>
                Allocator(const Allocator<_Other>&)
                {   // construct from a related Allocator (do nothing)
                }

                template<class _Other>
                Allocator<T>& operator=(const Allocator<_Other>&)
                {   // assign from a related Allocator (do nothing)
                    return (*this);
                }

                void deallocate(pointer _Ptr, size_type)
                {   // deallocate object at _Ptr, ignore size
                    //::operator delete(_Ptr);
                    BaseCat::System::MPHeap::Free(NULL, _Ptr);
                }

                pointer allocate(size_type _Count)
                {   // allocate array of _Count elements
                    return static_cast<pointer>(BaseCat::System::MPHeap::Alloc(NULL, sizeof(value_type) * _Count));
                }

                pointer allocate(size_type _Count, const void *)
                {   // allocate array of _Count elements, ignore hint
                    return (allocate(_Count));
                }

                void construct(pointer _Ptr, const T& _Val)
                {   // construct object at _Ptr with value _Val
                    //new(_Ptr) value_type(_Val);
                    std::_Construct(_Ptr, _Val);
                }

                void destroy(pointer _Ptr)
                {   // destroy object at _Ptr
                    std::_Destroy(_Ptr);
                }

                _SIZT max_size() const
                {   // estimate maximum array size
                    _SIZT _Count = (_SIZT)(-1) / sizeof (T);
                    return (0 < _Count ? _Count : 1);
                }
            };

            // Allocator TEMPLATE OPERATORS
            template<typename T, typename _Other>
            inline bool operator==(const Allocator<T>&, const Allocator<_Other>&)
            {   // test for Allocator equality (always true)
                return true;
            }

            template<typename T, typename _Other>
            inline bool operator!=(const Allocator<T>&, const Allocator<_Other>&)
            {   // test for Allocator inequality (always false)
                return false;
            }
        }
    }
}

#endif
