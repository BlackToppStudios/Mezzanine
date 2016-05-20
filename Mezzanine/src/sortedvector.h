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
/// @brief This file includes the declaration and definition for the SortedVector class.

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
    /// @param Comparer An instance of comparison function to use while finding.
    /// @tparam Iter An iterator type.
    /// @tparam T The type of the value the iterator points to.
    /// @tparam Compare A functor that accepts two parameters of type to
    /// @return The end iterator if nothing was found or the iterator found by value.
    template<typename Iter, typename T, typename Compare >
    Iter binary_find(Iter begin, Iter end, T val, Compare Comparer)
    {
        // Finds the lower bound in at most log(last - first) + 1 comparisons
        Iter i = std::lower_bound(begin, end, val, Comparer);

        if (i != end && !Comparer(val,*i))
            return i; // found
        else
            return end; // not found
    }

    /// @brief Search throught an iterator range and return the desired iterator.
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
    Iter binary_find(Iter begin, Iter end, T val)
    {
        // Finds the lower bound in at most log(last - first) + 1 comparisons
        Iter i = std::lower_bound(begin, end, val);

        if (i != end && !(val < *i))
            return i; // found
        else
            return end; // not found
    }


    /// @brief This container uses an std::vector for storage, but sorts all
    /// @tparam T The this container will store, must implement operate < for sorting
    template<typename T, typename Sorter = std::less<T> >
    class SortedVector
    {
        public:
            /// @brief The type used for internal storage.
            typedef std::vector<T> StorageVector;
            /// @brief The type used when checking sizes and capacities of instances of this.
            typedef typename StorageVector::size_type size_type;
            /// @brief The type of items stored.
            typedef typename StorageVector::value_type value_type;
            /// @brief Type of mutable random access iterator. Invalidated on all insertions.
            typedef typename StorageVector::iterator iterator;
            /// @brief Type of const random access iterator. Invalidated on all insertions.
            typedef typename StorageVector::const_iterator const_iterator;
            /// @brief Type of mutable reverse iterator for random access. Invalidated on all
            /// insertions.
            typedef typename StorageVector::reverse_iterator reverse_iterator;
            /// @brief Type of const reverse iterator for random access. Invalidated on all
            /// insertions.
            typedef typename StorageVector::const_reverse_iterator const_reverse_iterator;

        private:
            /// @internal The actual vector that does most of the interesting work.
            StorageVector InternalStorage;

        public:

            /// @brief Get an iterator to the beginning of the container.
            /// @return A mutable iterator pointing to the first element.
            iterator begin()
                { return InternalStorage.begin(); }
            /// @brief Get a const iterator to the beginning of the container.
            /// @return A const iterator pointing to the first element.
            const_iterator begin() const
                { return InternalStorage.begin(); }

            /// @brief Get an iterator to one past the end of the container.
            /// @return A mutable iterator to one past the end of the container.
            iterator end()
                { return InternalStorage.end(); }
            /// @brief Get an iterator to one past the end of the container.
            /// @return A const iterator to one past the end of the container.
            const_iterator end() const
                { return InternalStorage.end(); }

            /// @brief Get an iterator to the last element of the container, going backwards.
            /// @return A mutable reverse iterator pointing to the first element at the end.
            reverse_iterator rbegin()
                { return InternalStorage.rbegin(); }
            /// @brief Get a const iterator to the last element of the container, going backwards.
            /// @return A const reverse iterator pointing to the first element at the end.
            const_reverse_iterator rbegin() const
                { return InternalStorage.rbegin(); }

            /// @brief Get an iterator to one before the first element of the container.
            /// @return A mutable reverse iterator pointing to the reverse end element at the start.
            reverse_iterator rend()
                { return InternalStorage.rend(); }
            /// @brief Get a const iterator to one before the first element of the container.
            /// @return A const reverse iterator pointing to the reverse end element at the start.
            const_reverse_iterator rend() const
                { return InternalStorage.rend(); }

            /// @brief Uses std::sort to sort this, might using something more special focus in the
            /// future.
            void sort()
                { std::sort(begin(),end(), Sorter()); }

            /// @brief How many items are stored in this?
            /// @return Some integer type, likely unsigned indicating how many items this stores.
            size_type size() const
                { return InternalStorage.size(); }

            /// @brief Since this container has no array-like concept this inserts the item were
            /// it needs to go.
            /// @details This has all the potentional allocation slow downs of push_back
            /// and costs of find the required place to insert. This sorts the whole container after
            /// adding items, so calling this after for a many additions is really slow, use
            /// @ref add_range to only sort once.
            /// @param value The value to put into the vector.
            void add(T value)
            {
                InternalStorage.push_back(value);
                sort();
            }

            /// @brief Add several items at once efficiently.
            /// @details Sorts only once so is much faster than add(),
            /// @tparam ForeignIterator The type of the the iterator of the other container, must
            /// be at least a forward iterator.
            /// @param OtherBegin An iterator to the start of the other range to be copied.
            /// @param OtherEnd An iterator to one past the end.
            template<class ForeignIterator>
            void add_range(ForeignIterator OtherBegin, ForeignIterator OtherEnd)
            {
                for(; OtherBegin!=OtherEnd; OtherBegin++)
                    { InternalStorage.push_back(*OtherBegin); }
                sort();
            }

            /// @brief Get an item in the vector, operates in fast constant time.
            /// @param Index A 0 based index, if too large this can throw std::out_of_range.
            /// @return A reference to the stored item in the container. If the value is changed
            /// then SortedVector::sort should be called.
            T& operator[] (size_t Index)
                { return InternalStorage[Index]; }

            /// @brief Get and interator to a specific item, operates in fast logarithmic time.
            /// @param value the item to get the location of.
            /// @return A mutable iterator to an item, can be adjusted by random access.
            iterator find(T value)
                { return binary_find(begin(),end(),value,Sorter()); }
            /// @brief Get and interator to a specific item, operates in fast logarithmic time.
            /// @param value the item to get the location of.
            /// @return A const iterator to an item, can be adjusted by random access.
            const_iterator find(T value) const
                { return binary_find(begin(),end(),value,Sorter()); }

            /// @brief Does the item exist in this vector?
            /// @param value The item in question.
            /// @return True if present false otherwise.
            Boole contains(T value) const
                { return std::binary_search(begin(),end(),value); }

            /// @brief Empty the Vector discarding all data.
            void clear()
                { InternalStorage.clear(); }
            /// @brief Allocate enough space for the specified quantity of items
            /// @param new_capacity The amount of items to be ready to store.
            void reserve( size_type new_capacity )
                { InternalStorage.reserve(new_capacity); }
            /// @brief How many items can this store before requiring an allocation.
            size_type capacity() const
                { return InternalStorage.capacity(); }

            /// @brief Remove an item indicated by the iterator.
            /// @param position An iterator pointing to the item to remove.
            /// @return An iterator to one after the erased item.
            iterator erase( iterator position )
                { return InternalStorage.erase(position); }
            /// @brief Remove items indicated by an iterator range.
            /// @param first An iterator pointing to the first item to be erased.
            /// @param last An iterator pointer to one past the last item to be erased.
            /// @return An iterator to one after the erased item.
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
