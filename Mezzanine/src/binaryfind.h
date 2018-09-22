// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef binaryfind_h
#define binaryfind_h

#include "datatypes.h"

namespace Mezzanine
{
    /// @brief Search through an iterator range and return the desired iterator.
    /// @details Thanks to stack overflow and their CC-by-SA license which is written permission to
    /// use this.
    /// http://stackoverflow.com/questions/446296/where-can-i-get-a-useful-c-binary-search-algorithm
    /// @param begin The beginning of the iterator range.
    /// @param end One past the end of the actual iterators.
    /// @param val The value to find.
    /// @param Comparer An instance of comparison function to use while finding.
    /// @tparam Iter An iterator type.
    /// @tparam T The type of the value the iterator points to.
    /// @tparam Compare A functor that accepts two parameters of type to
    /// @return The end iterator if nothing was found or the iterator found by value.
    template<typename Iter, typename T, typename Compare >
    Iter binary_find(Iter begin, Iter end, const T& val, Compare Comparer)
    {
        // Finds the lower bound in at most log(last - first) + 1 comparisons
        Iter i = std::lower_bound(begin, end, val, Comparer);

        if (i != end && !Comparer(val,*i))
            return i; // found
        else
            return end; // not found
    }

    /// @brief Search through an iterator range and return the desired iterator.
    /// @details This uses the operator< to perform searches and matching for the contained item.
    /// @n
    /// Thanks to stack overflow and their CC-by-SA license which is written permission to
    /// use this.
    /// http://stackoverflow.com/questions/446296/where-can-i-get-a-useful-c-binary-search-algorithm
    /// @param begin The beginning of the iterator range.
    /// @param end One past the end of the actual iterators.
    /// @param val The value to find.
    /// @tparam Iter An iterator type.
    /// @tparam T The type of the value the iterator points to.
    /// @return The end iterator if nothing was found or the iterator found by value.
    template<class Iter, class T>
    Iter binary_find(Iter begin, Iter end, const T& val)
    {
        // Finds the lower bound in at most log(last - first) + 1 comparisons
        Iter i = std::lower_bound(begin, end, val);

        if (i != end && !(val < *i))
            return i; // found
        else
            return end; // not found
    }
}//Mezzanine

#endif
