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

#ifndef __SpringCat_Common_Exceptions__
#define __SpringCat_Common_Exceptions__

#include <BaseCat/System.h>

namespace SpringCat
{
    namespace Common
    {
        //std::exception
        class Exception
        {
        private:
            BaseCat::System::TL::string message_;

        public:
            Exception(void)
            {}
            Exception(const BaseCat::System::TL::string &message)
                : message_(message)
            {}
            Exception(const Exception &rhs)
                : message_(rhs.message_)
            {}
            virtual ~Exception(void)
            {}

            Exception &operator=(const Exception &rhs)
            {
                Exception tmp(rhs);
                message_.swap(tmp.message_);

                return *this;
            }

            const BaseCat::System::TL::string &GetMessage(void) const
            {
                return message_;
            }
        };

        class InvalidArgumentException : public Exception
        {
        public:
            InvalidArgumentException(void)
                : Exception("InvalidArgumentException")
            {}
            InvalidArgumentException(const BaseCat::System::TL::string &message)
                : Exception(message)
            {}
            virtual ~InvalidArgumentException(void)
            {}
        };

        class NullPointerException : public Exception
        {
        public:
            NullPointerException(void)
                : Exception("NullPointerException")
            {}
            NullPointerException(const BaseCat::System::TL::string &message)
                : Exception(message)
            {}
            virtual ~NullPointerException(void)
            {}
        };

        class LogicException : public Exception
        {
        public:
            LogicException(void)
                : Exception("LogicException")
            {}
            LogicException(const BaseCat::System::TL::string &message)
                : Exception(message)
            {}
            virtual ~LogicException(void)
            {}
        };
    }
}

#endif
