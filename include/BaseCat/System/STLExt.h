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

#ifndef __BaseCat_System_TL_STLExt__
#define __BaseCat_System_TL_STLExt__

#include "Allocator.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <queue>
#include <set>

namespace BaseCat
{
    namespace System
    {
        namespace TL
        {
            typedef std::basic_string<char, std::char_traits<char>, Allocator<char> > string;
            typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t> > wstring;

            template<typename T, typename AllocatorT = Allocator<T> >
            class vector : public std::vector<T, AllocatorT>
            {
            public:
                typedef std::vector<T, AllocatorT> BaseType;

            public:
                vector(void)
                    : BaseType()
                {}
                explicit vector(const AllocatorT &allocator)
                    : BaseType(allocator)
                {}
                explicit vector(size_type count)
                    : BaseType(count)
                {}
                vector(size_type count, const T &value)
                    : BaseType(count, value)
                {}
                vector(size_type count, const T &value, const AllocatorT &allocator)
                    : BaseType(count, value, allocator)
                {}
                vector(const vector &rhs)
                    : BaseType(rhs)
                {}
                template<typename IteratorT>
                vector(IteratorT first, IteratorT last)
                    : BaseType(first, last)
                {}
                template<typename IteratorT>
                vector(IteratorT first, IteratorT last, const AllocatorT &allocator)
                    : BaseType(first, last, allocator)
                {}
            };

            template<typename T, typename AllocatorT = Allocator<T> >
            class list : public std::list<T, AllocatorT>
            {
            public:
                typedef std::list<T, AllocatorT> BaseType;

            public:
                list(void)
                    : BaseType()
                {}
                explicit list(const AllocatorT &allocator)
                    : BaseType(allocator)
                {}
                explicit list(size_type count)
                    : BaseType(count)
                {}
                list(size_type count, const T &value)
                    : BaseType(count, value)
                {}
                list(size_type count, const T &value, const AllocatorT &allocator)
                    : BaseType(count, value, allocator)
                {}
                list(const vector &rhs)
                    : BaseType(rhs)
                {}
                template<typename IteratorT>
                list(IteratorT first, IteratorT last)
                    : BaseType(first, last)
                {}
                template<typename IteratorT>
                list(IteratorT first, IteratorT last, const AllocatorT &allocator)
                    : BaseType(first, last, allocator)
                {}
            };

            template<typename KeyT, typename ValueT,
                typename LessT = std::less<KeyT>,
                typename AllocatorT = Allocator<std::pair<KeyT, ValueT> >
            >
            class map : public std::map<KeyT, ValueT, LessT, AllocatorT>
            {
            public:
                typedef std::map<KeyT, ValueT, LessT, AllocatorT> BaseType;

            public:
                map(void)
                    : BaseType()
                {}
                explicit map(const key_compare &pred)
                    : BaseType(pred)
                {}
                map(const key_compare &pred, const AllocatorT &allocator)
                    : BaseType(pred, allocator)
                {}
                template<typename IteratorT>
                map(IteratorT first, IteratorT last)
                    : BaseType(first, last)
                {}
                template<typename IteratorT>
                map(IteratorT first, IteratorT last, const key_compare &pred)
                    : BaseType(first, last, pred)
                {}
                template<typename IteratorT>
                map(IteratorT first, IteratorT last,
                    const key_compare &pred, const AllocatorT &allocator)
                    : BaseType(first, last, pred, allocator)
                {}
            };

            template<typename KeyT, typename ValueT,
                typename LessT = std::less<KeyT>,
                typename AllocatorT = Allocator<std::pair<KeyT, ValueT> >
            >
            class multimap : public std::multimap<KeyT, ValueT, LessT, AllocatorT>
            {
            public:
                typedef std::multimap<KeyT, ValueT, LessT, AllocatorT> BaseType;

            public:
                multimap(void)
                    : BaseType()
                {}
                explicit multimap(const key_compare &pred)
                    : BaseType(pred)
                {}
                multimap(const key_compare &pred, const AllocatorT &allocator)
                    : BaseType(pred, allocator)
                {}
                template<typename IteratorT>
                multimap(IteratorT first, IteratorT last)
                    : BaseType(first, last)
                {}
                template<typename IteratorT>
                multimap(IteratorT first, IteratorT last, const key_compare &pred)
                    : BaseType(first, last, pred)
                {}
                template<typename IteratorT>
                multimap(IteratorT first, IteratorT last,
                    const key_compare &pred, const AllocatorT &allocator)
                    : BaseType(first, last, pred, allocator)
                {}
            };

            template<typename T, typename AllocatorT = Allocator<T> >
            class deque : public std::deque<T, AllocatorT>
            {
            public:
                typedef std::deque<T, AllocatorT> BaseType;

            public:
                deque(void)
                    : BaseType()
                {}
                explicit deque(const Allocator<T> &allocator)
                    : BaseType(allocator)
                {}

                explicit deque(size_type count)
                    : BaseType(count)
                {}
                deque(size_type count, const T &value)
                    : BaseType(count, value)
                {}
                deque(size_type count, const T &value, const AllocatorT &allocator)
                    : BaseType(count, value, allocator)
                {}
                deque(const deque &rhs)
                    : BaseType(rhs)
                {}
                template<typename IteratorT>
                deque(IteratorT first, IteratorT last)
                    : BaseType(first, last)
                {}
                template<typename IteratorT>
                deque(IteratorT first, IteratorT last, const AllocatorT &allocator)
                    : BaseType(first, last, allocator)
                {}
            };

            template<typename T, typename ContainerT = deque<T> >
            class queue : public std::queue<T, ContainerT>
            {
            public:
                typedef std::queue<T, ContainerT> BaseType;

            public:
                queue(void)
                    : BaseType()
                {}
                explicit queue(const ContainerT &container)
                    : BaseType(container)
                {}
            };

            template<typename KeyT,
                typename LessT = std::less<KeyT>,
                typename AllocatorT = Allocator<KeyT>
            >
            class set : public std::set<KeyT, LessT, AllocatorT>
            {
            public:
                typedef std::set<KeyT, LessT, AllocatorT> BaseType;

            public:
                set(void)
                    : BaseType()
                {}
                explicit set(const key_compare &pred)
                    : BaseType(pred)
                {}
                set(const key_compare &pred, const AllocatorT &allocator)
                    : BaseType(pred, allocator)
                {}
                template<typename IteratorT>
                set(IteratorT first, IteratorT last)
                    : BaseType(first, last)
                {}
                template<typename IteratorT>
                set(IteratorT first, IteratorT last, const key_compare &pred)
                    : BaseType(first, last, pred)
                {}
                template<typename IteratorT>
                set(IteratorT first, IteratorT last,
                    const key_compare &pred, const AllocatorT &allocator)
                    : BaseType(first, last, pred, allocator)
                {}
            };

            template<typename KeyT,
                typename LessT = std::less<KeyT>,
                typename AllocatorT = Allocator<KeyT>
            >
            class multiset : public std::multiset<KeyT, LessT, AllocatorT>
            {
            public:
                typedef std::multiset<KeyT, LessT, AllocatorT> BaseType;

            public:
                multiset(void)
                    : BaseType()
                {}
                explicit multiset(const key_compare &pred)
                    : BaseType(pred)
                {}
                multiset(const key_compare &pred, const AllocatorT &allocator)
                    : BaseType(pred, allocator)
                {}
                template<typename IteratorT>
                multiset(IteratorT first, IteratorT last)
                    : BaseType(first, last)
                {}
                template<typename IteratorT>
                multiset(IteratorT first, IteratorT last, const key_compare &pred)
                    : BaseType(first, last, pred)
                {}
                template<typename IteratorT>
                multiset(IteratorT first, IteratorT last,
                    const key_compare &pred, const AllocatorT &allocator)
                    : BaseType(first, last, pred, allocator)
                {}
            };
        }
    }
}

#endif
