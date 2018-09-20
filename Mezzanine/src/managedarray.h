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
#ifndef managedarray_h
#define managedarray_h

/// @file
/// @brief This file includes the declaration and definition for the SortedArray class.

#include "datatypes.h"
#include "binaryfind.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This container uses an std::array for storage, but manages the amount of used space.
    /// @tparam ElementType The type this container will store.
    /// @tparam NumElements The number of ElementType instances this array will be allocated to store.
    /// @details Since this container performs no allocations, it's iterators do not invalidate.
    /// However in some situations the contents of the values under the iterator can change, for
    /// example when swap is called.  The element is still the expected type and can be accessed,
    /// but the values may not be what you expect.
    ///////////////////////////////////////
    template<typename ElementType, size_t NumElements>
    class ManagedArray
    {
    public:
        /// @brief Assert for sanity checking array size.
        static_assert( NumElements > 0, "Array size must be greater than zero.");

        /// @brief Convenience type to refer to the type of this.
        typedef ManagedArray<ElementType,NumElements> SelfType;
        /// @brief The type of internal buffer where our elements will be stored.
        typedef typename std::aligned_storage<sizeof(ElementType),alignof(ElementType)>::type BufferElementType;
        /// @brief The type used when checking sizes and capacities of instances of this.
        typedef size_t size_type;
        /// @brief The type of items stored.
        typedef ElementType value_type;
        /// @brief Type of mutable random access iterator. Invalidated on all insertions.
        typedef value_type* iterator;
        /// @brief Type of const random access iterator. Invalidated on all insertions.
        typedef const value_type* const_iterator;
        /// @brief Type of mutable reverse iterator for random access. Invalidated on all insertions.
        typedef std::reverse_iterator<iterator> reverse_iterator;
        /// @brief Type of const reverse iterator for random access. Invalidated on all insertions.
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    private:
        /// @brief The internal buffer storing all of our objects.
        BufferElementType InternalStorage[NumElements];
        /// @brief The amount of initialized elements that exist in the buffer.
        size_t UsedSpace{0};

        ///////////////////////////////////////////////////////////////////////////////
        // Raw buffer/element management

        /// @brief Creates a new instance of an object in an already existing chunk of memory.
        /// @remarks This does not allocate.  Expects to work with already allocated array memory.
        /// @tparam ArgTypes A variadic template of types for the parameters of construction.
        /// @param ToCreate A pointer to the region of memory to create the object.
        /// @param Args A collection of arguments to pass into the constructor.
        template<class... ArgTypes>
        void Create(value_type* ToCreate, ArgTypes&&... Args)
            { ::new (static_cast<void*>(ToCreate)) ElementType(std::forward<ArgTypes>(Args)...); }
        /// @brief Gets a raw storage element and casts it to the appropriate type.
        /// @param Index The index of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        value_type& Retrieve(size_t Index)
            { return *( reinterpret_cast<ElementType*>( InternalStorage ) + Index ); }
        /// @brief Gets a raw storage element and casts it to the appropriate type.
        /// @param Index The index of the element to retrieve.
        /// @return Returns a const reference to the element at the specified index.
        const value_type& Retrieve(size_t Index) const
            { return *( reinterpret_cast<const ElementType*>( InternalStorage ) + Index ); }
        /// @brief Destroys an existing object by invoking it's destructor.
        /// @remarks This does not deallocate.  The memory is expected to be reused by this container.
        /// @param ToDestroy A pointer to the object that will be destroyed.
        void Destroy(value_type* ToDestroy)
            { ToDestroy->~value_type(); }
        /// @brief Destroys all elements in the container after the specified element.
        /// @param NewLast A pointer to the element that will become the new end iterator.
        void DestroyAtAndAfter(value_type* NewEnd)
        {
            value_type* CountdownToEnd = end();
            while( NewEnd != CountdownToEnd )
                { Destroy(--CountdownToEnd); }
            UsedSpace = std::distance(begin(),NewEnd);
        }
    public:
        /// @brief Class constructor.
        ManagedArray() = default;
        /// @brief Copy constructor.
        /// @param Other The other array to be copied.
        ManagedArray(const ManagedArray& Other) = default;
        /// @brief Move constructor.
        /// @remarks Since this is all stack allocated, this is pretty much a copy.
        /// @param Other The other array to be moved.
        ManagedArray(ManagedArray&& Other)
        {
            for( Whole CurrIdx = 0 ; CurrIdx < Other.size() ; ++CurrIdx )
                { push_back( std::move( Other[CurrIdx] ) ); }
            Other.clear();
        }
        /// @brief Initializer list constructor.
        /// @param InitList A listing of the values to initialize this array with.
        ManagedArray(std::initializer_list<ElementType> InitList)
            { append(InitList.begin(),InitList.end()); }
        /// @brief Class destructor.
        ~ManagedArray()
            { DestroyAtAndAfter(begin()); }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy Assignment Operator.
        /// @param Other The other array to be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move Assignment Operator.
        /// @param Other The other array to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other)
        {
            for( Whole CurrIdx = 0 ; CurrIdx < Other.size() ; ++CurrIdx )
                { push_back( std::move( Other[CurrIdx] ) ); }
            Other.clear();
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Iterators

        /// @brief Get an iterator to the beginning of the container.
        /// @return A mutable iterator pointing to the first element.
        iterator begin()
            { return reinterpret_cast<value_type*>( InternalStorage ); }
        /// @brief Get a const iterator to the beginning of the container.
        /// @return A const iterator pointing to the first element.
        const_iterator begin() const
            { return reinterpret_cast<const value_type*>( InternalStorage ); }
        /// @brief Get a const iterator to the beginning of the container.
        /// @return A const iterator pointing to the first element.
        const_iterator cbegin() const
            { return reinterpret_cast<const value_type*>( InternalStorage ); }

        /// @brief Get an iterator to one past the end of the container.
        /// @return A mutable iterator to one past the end of the container.
        iterator end()
            { return begin() + UsedSpace; }
        /// @brief Get a const iterator to one past the end of the container.
        /// @return A const iterator to one past the end of the container.
        const_iterator end() const
            { return begin() + UsedSpace; }
        /// @brief Get a const iterator to one past the end of the container.
        /// @return A const iterator to one past the end of the container.
        const_iterator cend() const
            { return begin() + UsedSpace; }

        /// @brief Get an iterator to the last element of the container, going backwards.
        /// @return A mutable reverse iterator pointing to the first element at the end.
        reverse_iterator rbegin()
            { return reverse_iterator( end() ); }
        /// @brief Get a const iterator to the last element of the container, going backwards.
        /// @return A const reverse iterator pointing to the first element at the end.
        const_reverse_iterator rbegin() const
            { return const_reverse_iterator( end() ); }
        /// @brief Get a const iterator to the last element of the container, going backwards.
        /// @return A const reverse iterator pointing to the first element at the end.
        const_reverse_iterator crbegin() const
            { return const_reverse_iterator( end() ); }

        /// @brief Get an iterator to one before the first element of the container.
        /// @return A mutable reverse iterator pointing to the reverse end element at the start.
        reverse_iterator rend()
            { return reverse_iterator( begin() ); }
        /// @brief Get a const iterator to one before the first element of the container.
        /// @return A const reverse iterator pointing to the reverse end element at the start.
        const_reverse_iterator rend() const
            { return const_reverse_iterator( begin() ); }
        /// @brief Get a const iterator to one before the first element of the container.
        /// @return A const reverse iterator pointing to the reverse end element at the start.
        const_reverse_iterator crend() const
            { return const_reverse_iterator( begin() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Query and Access

        /// @brief Gets he amount of valid elements currently stored in this array.
        /// @return Returns the amount of element storage currently consumed in this array.
        size_type size() const
            { return UsedSpace; }
        /// @brief Gets the maximum number of elements this array could store.
        /// @remarks This function does the same/is the same as max_size().  It exists for
        /// compatibility with container adapters.
        /// @return Returns the total number of elements this array has room for.
        size_type capacity() const
            { return NumElements; }
        /// @brief Gets the maximum number of elements this array can support.
        /// @remarks This function does the same/is the same as capacity().  It exists for
        /// compatibility with container adapters.
        /// @return Returns the total number of elements this array can store.
        size_type max_size() const
            { return NumElements; }
        /// @brief Gets whether or not this array is empty.
        /// @return Returns true if this array is not storing any elements, false otherwise.
        Boole empty() const
            { return UsedSpace == 0;  }

        /// @brief Gets the element at the specified index.
        /// @note Take care with the use of this function.  It performs no bounds checking.
        /// @param Index The offset of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        value_type& operator[] (size_t Index)
            { return Retrieve(Index); }
        /// @brief Gets the element at the specified index.
        /// @note Take care with the use of this function.  It performs no bounds checking.
        /// @param Index The offset of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        const value_type& operator[] (size_t Index) const
            { return Retrieve(Index); }
        /// @brief Gets the element at the specified index.
        /// @exception If the index specified is greater than the current number of stored elements,
        /// a std::out_of_range exception will be thrown.
        /// @param Index The offset of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        value_type& at(size_t Index)
        {
            if( Index >= UsedSpace ) {
                throw std::out_of_range("Specified array index is invalid.");
            }
            return Retrieve(Index);
        }
        /// @brief Gets the element at the specified index.
        /// @exception If the index specified is greater than the current number of stored elements,
        /// a std::out_of_range exception will be thrown.
        /// @param Index The offset of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        const value_type& at(size_t Index) const
        {
            if( Index >= UsedSpace ) {
                throw std::out_of_range("Specified array index is invalid.");
            }
            return Retrieve(Index);
        }

        /// @brief Gets whether or not an element exists in this array.
        /// @param value The element to check for.
        /// @return Returns true if an element of that value exists in this array, false otherwise.
        Boole contains(const value_type& value) const
            { return ( std::find(begin(),end(),value) != end() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Sequence Modifiers

        /// @brief Appends a new element to the array.
        /// @exception If the array is already full, attempting to push a new element will
        /// throw an std::out_of_range exception.
        /// @param ToPush The element that will be copied to the end of the array.
        void push_back(const value_type& ToPush)
        {
            if( UsedSpace >= NumElements ) {
                throw std::out_of_range("Attempting to add an element to a full array.");
            }
            Create( begin() + UsedSpace, ToPush );
            UsedSpace++;
        }
        /// @brief Appends a new element to the array.
        /// @exception If the array is already full, attempting to push a new element will
        /// throw an std::out_of_range exception.
        /// @param ToPush The element that will be moved to the end of the array.
        void push_back(value_type&& ToPush)
        {
            if( UsedSpace >= NumElements ) {
                throw std::out_of_range("Attempting to add an element to a full array.");
            }
            Create( begin() + UsedSpace, ToPush );
            UsedSpace++;
        }

        /// @brief Adds elements to the end of the array.
        /// @exception If the number of elements to be appended exceeds the capacity of the
        /// array, an std::out_of_range exception will be thrown.
        /// @param First An iterator to the first element of the range to append.
        /// @param Last An iterator to one passed the last element of the range to append.
        template<class InputIterator>
        void append(InputIterator First, InputIterator Last)
            { insert(end(),First,Last); }

        /// @brief Adds an element to the array by constructing in place.
        /// @param Pos An iterator to the position to construct the new element.
        /// @param Args Variadic template of construction parameters.
        /// @return Returns an iterator to the created element.
        template<class... ArgTypes>
        iterator emplace(const_iterator Pos, ArgTypes&&... Args)
        {
            if( UsedSpace >= NumElements ) {
                throw std::out_of_range("Attempting to add an element to a full array.");
            }

            iterator Ret = const_cast<iterator>(Pos);
            if( Pos == end() ) {
                Create(Ret,std::forward<ArgTypes>(Args)...);
            }else{
                Create( end(), std::move( Retrieve(UsedSpace - 1) ) );
                std::move_backward( Ret, end(), end() + 1 );
                Destroy(Ret); // Instead of Destroy/Create, we could create a temporary and move it
                Create(Ret,std::forward<ArgTypes>(Args)...);
            }
            ++UsedSpace;
            return Ret;
        }

        /// @brief Constructs a new element in place at the end of the array.
        /// @exception If the array is already full, attempting to push a new element will
        /// throw an std::out_of_range exception.
        /// @param Args Variadic template of construction parameters.
        /// @return Returns a reference to the created element.
        template<class... ArgTypes>
        value_type& emplace_back(ArgTypes&&... Args)
        {
            if( UsedSpace >= NumElements ) {
                throw std::out_of_range("Attempting to add an element to a full array.");
            }
            Create( begin() + UsedSpace, std::forward<ArgTypes>(Args)... );
            return Retrieve( UsedSpace++ );
        }

        /// @brief Inserts a new element by copy assignment.
        /// @exception If the array is already full, attempting to push a new element will
        /// throw an std::out_of_range exception.
        /// @param Pos A const iterator to the position the new element will be inserted.
        /// @return Returns an iterator to the inserted element.
        iterator insert(const_iterator Pos, const value_type& ToInsert)
        {
            if( UsedSpace >= NumElements ) {
                throw std::out_of_range("Attempting to add an element to a full array.");
            }

            iterator Ret = const_cast<iterator>(Pos);
            if( Ret == end() ) {
                Create(Ret,ToInsert);
            }else{
                Create( end(), std::move( Retrieve(UsedSpace - 1) ) );
                std::move_backward( Ret, end() - 1, end() );
                *Ret = ToInsert;
            }
            ++UsedSpace;
            return Ret;
        }

        /// @brief Inserts a new element by move assignment.
        /// @exception If the array is already full, attempting to push a new element will
        /// throw an std::out_of_range exception.
        /// @param Pos A const iterator to the position the new element will be inserted.
        /// @return Returns an iterator to the inserted element.
        iterator insert(const_iterator Pos, value_type&& ToInsert)
        {
            if( UsedSpace >= NumElements ) {
                throw std::out_of_range("Attempting to add an element to a full array.");
            }

            iterator Ret = const_cast<iterator>(Pos);
            if( Pos == end() ) {
                Create(Ret,ToInsert);
            }else{
                Create( end(), std::move( Retrieve(UsedSpace - 1) ) );
                std::move_backward( Ret, end() - 1, end() );
                *Ret = std::move(ToInsert);
            }
            ++UsedSpace;
            return Ret;
        }

        /// @brief Inserts a range of elements into the array (via copying).
        /// @param Pos A const iterator to the position the range will be inserted.
        /// @param First An iterator to the first element in the range to be inserted.
        /// @param Last An iterator to one passed the last element in the range to be inserted.
        /// @return Returns an iterator to the first element of the inserted range.
        template<class InputIterator>
        iterator insert(const_iterator Pos, InputIterator First, InputIterator Last)
        {
            const size_t ToInsert = std::distance(First,Last);
            if( UsedSpace + ToInsert > NumElements ) {
                throw std::out_of_range("Attempting to add a range of elements that would exceed array capacity.");
            }

            iterator Ret = const_cast<iterator>(Pos);
            if( ToInsert > 0 ) {
                size_t ToMoveConstruct = end() - Pos;
                size_t ToCopyConstruct = ( ToInsert > ToMoveConstruct ? ToInsert - ToMoveConstruct : 0 );
                //size_t ToCopy = ToInsert - ToCopyConstruct;

                iterator InitTarget = end() + ToInsert - 1;
                for(  ; ToMoveConstruct > 0 ; --ToMoveConstruct, --InitTarget )
                    { Create( InitTarget, std::move( *(InitTarget - ToInsert) ) ); }

                for(  ; ToCopyConstruct > 0 ; --ToCopyConstruct, --InitTarget )
                    { Create( InitTarget, *(--Last) ); }

                std::copy(First,Last,Ret);
                UsedSpace += ToInsert;
            }
            return Ret;
        }

        /// @brief Destroys the last element of the array.
        /// @remarks This performs no checks of its own and bad things happen if
        /// you call this on an empty array.
        void pop_back()
            { Destroy( begin() + --UsedSpace ); }

        /// @brief Removes a single element from the array.
        /// @exception If the provided iterator is outside this array, an std::out_of_range exception will be thrown.
        /// @param Pos A const iterator to the position of the element to be erased.
        /// @return Returns an iterator to the element after the one that was erased (and is now in it's spot).
        iterator erase(const_iterator Pos)
        {
            if( !(Pos >= begin() && Pos < end()) ) {
                throw std::out_of_range("Iterator provided is outside the valid container range.");
            }
            iterator Ret = const_cast<iterator>(Pos);
            DestroyAtAndAfter( std::move(Ret + 1,end(),Ret) );
            return Ret;
        }

        /// @brief Removes a range of elements from the array.
        /// @exception If the provided iterator is outside this array, an std::out_of_range exception will be thrown.
        /// @param First A const iterator to the first element to be removed.
        /// @param Last A const iterator to the one passed the last element to be removed.
        /// @return Returns an iterator to the first element after the removed range.
        iterator erase(const_iterator First, const_iterator Last)
        {
            if( !(First >= begin() && Last < end()) ) {
                throw std::out_of_range("Iterator range is outside the valid container range.");
            }
            iterator Ret = const_cast<iterator>(First);
            size_t ElementsToErase = std::distance(First,Last);
            if( ElementsToErase > 0 ) {
                DestroyAtAndAfter( std::move(Ret + ElementsToErase,end(),Ret) );
            }
            return Ret;
        }

        /// @brief Swaps the contents of two arrays.
        /// @remarks Since arrays are stack allocated, this method must swap each element stored.
        /// This can be slow for large arrays.
        void swap(SelfType& Other)
        {
            std::swap_ranges(begin(),end(),Other.begin());
            std::swap(UsedSpace,Other.UsedSpace);
        }

        /// @brief Empty the array, destructing all elements.
        void clear()
        {
            for( iterator CurrEle = begin() ; CurrEle != end() ; ++CurrEle )
                { Destroy(CurrEle); }
            UsedSpace = 0;
        }
    };//ManagedArray
} // Mezzanine

#endif // Include guard
