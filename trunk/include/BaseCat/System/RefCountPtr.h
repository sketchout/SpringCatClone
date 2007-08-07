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

#ifndef __BaseCat_RefCountPtr__
#define __BaseCat_RefCountPtr__

#include <utility>

namespace BaseCat
{
    namespace System
    {
        namespace TL
        {
            template<typename T>
            class RefCountPtr
            {
            private:
                typedef T* StoredType;
                typedef T* PointerType;
                typedef T const* ConstPointerType;
                typedef T& ReferenceType;
                typedef T const& ConstReferenceType;

            private:
                StoredType pointee_;

            public:
                RefCountPtr(void) : pointee_(NULL)
                {
                }
                RefCountPtr(const StoredType &p) : pointee_(p)
                {
                    if (pointee_ != NULL)
                    {
                        pointee_->AddRef();
                    }
                }
#if (_MSC_VER >= 1301) || defined(_UNIX_)
                RefCountPtr(const RefCountPtr &rhs) : pointee_(rhs.pointee_)
                {
                    if (pointee_ != NULL)
                    {
                        pointee_->AddRef();
                    }
                }
                template<typename U>
                RefCountPtr(RefCountPtr<U> &rhs) : pointee_(rhs.GetRawPtr())
                {
                    if (pointee_ != NULL)
                    {
                        pointee_->AddRef();
                    }
                }
#endif
                template<typename U>
                RefCountPtr(const RefCountPtr<U> &rhs) : pointee_(const_cast<T *>(rhs.GetRawPtr()))
                {
                    if (pointee_ != NULL)
                    {
                        pointee_->AddRef();
                    }
                }
                ~RefCountPtr(void)
                {
                    if (pointee_ != NULL)
                    {
                        pointee_->Release();
                    }
                }

                void Swap(RefCountPtr &rhs)
                {
                    std::swap(pointee_, rhs.pointee_);
                }

                PointerType GetRawPtr(void)
                {
                    return pointee_;
                }
                ConstPointerType GetRawPtr(void) const
                {
                    return pointee_;
                }

#if (_MSC_VER >= 1301) || defined(_UNIX_)
                RefCountPtr &operator=(const RefCountPtr &rhs)
                {
                    RefCountPtr temp(rhs);
                    temp.Swap(*this);

                    return *this;
                }
                template<typename U>
                RefCountPtr &operator=(RefCountPtr<U> &rhs)
                {
                    RefCountPtr temp(rhs);
                    temp.Swap(*this);

                    return *this;
                }
#endif
                template<typename U>
                RefCountPtr &operator=(const RefCountPtr<U> &rhs)
                {
                    RefCountPtr temp(rhs);
                    temp.Swap(*this);

                    return *this;
                }

                PointerType operator->()
                {
                    return GetRawPtr();
                }
                ConstPointerType operator->() const
                {
                    return GetRawPtr();
                }

                ReferenceType operator*()
                {
                    return *GetRawPtr();
                }
                ConstReferenceType operator*() const
                {
                    return *GetRawPtr();
                }
            };
        }
    }
}

#endif
