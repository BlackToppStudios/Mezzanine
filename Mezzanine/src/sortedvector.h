// © Copyright 2010 - 2014 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef sortedvector_h
#define sortedvector_h

/// @file
/// @brief Describe file here

#include "datatypes.h"

namespace Mezzanine
{
    /// @brief Search throught an iterator range and return the desired iterator.
    /// @details Thanks to stack overflow and their CC-by-SA license which is written permission to
    /// use this.
    /// http://stackoverflow.com/questions/446296/where-can-i-get-a-useful-c-binary-search-algorithm
    /// @param begin The beginning of the iterator range.
    /// @param end One past the end of the actual iterators.
    /// @param val The value to find.
    /// @tparam Iter An iterator type.
    /// @tparam T The type of the value the iterator points to.
    /// @return The end iterator if nothing was found or the iterator found by value.
    template<class Iter, class T>
    Iter binary_find(Iter begin, Iter end, T val)
    {
        // Finds the lower bound in at most log(last - first) + 1 comparisons
        Iter i = std::lower_bound(begin, end, val);

        if (i != end && !(val < *i))
            return i; // found
        else
            return end; // not found
    }

    template<typename T>
    class SortedVector
    {
        public:
            typedef std::vector<T> StorageVector;
            typedef typename StorageVector::size_type size_type;
            typedef typename StorageVector::value_type value_type;
            typedef typename StorageVector::iterator iterator;
            typedef typename StorageVector::const_iterator const_iterator;

            typedef typename StorageVector::reverse_iterator reverse_iterator;
            typedef typename StorageVector::const_reverse_iterator const_reverse_iterator;

        private:
            /// @internal The actual vector that does most of the interesting work.
            StorageVector InternalStorage;

        public:

            iterator begin()
                { return InternalStorage.begin(); }
            const_iterator begin() const
                { return InternalStorage.begin(); }

            iterator end()
                { return InternalStorage.end(); }
            const_iterator end() const
                { return InternalStorage.end(); }

            reverse_iterator rbegin()
                { return InternalStorage.rbegin(); }
            const_reverse_iterator rbegin() const
                { return InternalStorage.rbegin(); }

            reverse_iterator rend()
                { return InternalStorage.rend(); }
            const_reverse_iterator rend() const
                { return InternalStorage.rend(); }

            void sort()
                { std::sort(begin(),end()); }

            size_type size() const
                { return InternalStorage.size(); }

            void add(T value)
            {
                InternalStorage.push_back(value);
                sort();
            }

            T& operator[] (size_t Index)
                { return InternalStorage[Index]; }

            iterator find(T value)
                { return binary_find(begin(),end(),value); }
            const_iterator find(T value) const
                { return binary_find(begin(),end(),value); }

            Boole contains(T value) const
                { return std::binary_search(begin(),end(),value); }


            void clear()
                { InternalStorage.clear(); }
            void reserve( size_type new_capacity )
                { InternalStorage.reserve(new_capacity); }
            size_type capacity() const
                { return InternalStorage.capacity(); }


            iterator erase( iterator position )
                { return InternalStorage.erase(position); }
            iterator erase( iterator first, iterator last )
                { return InternalStorage.erase(first, last); }

//            void resize( size_type count )
//                { InternalStorage.reserve(count); }

            // proper C++11 way
            //void resize( size_type count )
            //    { InternalStorage.reserve(count); }
            //void resize( size_type count, const value_type& value )
            //    { InternalStorage.reserve(count,value); }
    };
} // /namespace Mezzanine

#endif // Include guard
