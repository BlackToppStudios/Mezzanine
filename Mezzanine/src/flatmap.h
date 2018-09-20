// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef flatmap_h
#define flatmap_h

/// @file
/// @brief This file includes the declaration and definition for the FlatMap class.

#include "datatypes.h"
#include "binaryfind.h"

namespace Mezzanine
{
    /// @brief A simple type trait that'll reduce any number of types to void.
    /// @note This can be replaced with std::void_t in C++17.
    template<typename...>
    struct Voider
        { using type = void; };
    /// @brief A simple type trait value that gets the "type" member of Voider.
    /// @note This can be replaced with std::void_t in C++17.
    template<typename... Types>
    using Voider_t = typename Voider<Types...>::type;

    /// @brief Type trait for detecting of a comparator is transparent.
    /// @tparam Comp The comparator functor to check.
    /// @tparam KeyType The key being used.
    /// @details This is the dummy trait for failure cases.
    template<typename Comp, typename KeyType, typename = Voider_t<>>
    struct Is_Transparent : std::false_type
        {  };
    /// @brief Type trait specialization for detecting of a comparator is transparent.
    /// @tparam Comp The comparator functor to check.
    /// @tparam KeyType The key being used.
    template<typename Comp, typename KeyType>
    struct Is_Transparent<Comp,KeyType,Voider_t<typename Comp::is_transparent>> : std::true_type
        {  };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A simple comparison functor that stores (and uses) another functor.
    /// @details This is used by the FlatMap as a indirect so we can use stl algorithms in the map
    /// implementation, but also so they don't use the raw std::pair that the iterators will all
    /// be pointing to.  Instead this functor will make them use exclusively the keys for
    /// comparison while also preserving the flexibility of allowing any key compare you want or
    /// need in the implementation.
    ///////////////////////////////////////
    template<typename StoredType, typename CompareType = std::less<typename StoredType::first_type>>
    class FlatPairCompare
    {
    public:
        /// @brief Convenience type to refer to the type of this.
        using SelfType = FlatPairCompare<StoredType,CompareType>;
        /// @brief Convenience type for the key of the pair.
        using KeyType = typename StoredType::first_type;
        /// @brief Convenience type for the value of the pair.
        using ElementType = typename StoredType::second_type;
    protected:
        /// @brief The functor that will compare the keys.
        CompareType CompFunct;
    public:
        /// @brief Default constructor (removed).
        FlatPairCompare() = delete;
        /// @brief Copy constructor.
        /// @param Other The other pair comparison object to be copied.
        FlatPairCompare(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other pair comparison object to be moved.
        FlatPairCompare(SelfType&& Other) = default;
        /// @brief Compare functor constructor.
        /// @param Funct The function object to initialize this comparison object with.
        FlatPairCompare(CompareType Funct) :
            CompFunct(Funct)
            {  }

        /// @brief Copy-assignment operator.
        /// @param Other The other pair comparison object to be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move-assignment operator.
        /// @param Other The other pair comparison object to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        /// @brief Gets the key comparison object.
        /// @return Returns a reference to the internal key comparison object being used by this.
        CompareType& GetKeyComp()
            { return CompFunct; }
        /// @brief Gets the key comparison object.
        /// @return Returns a const reference to the internal key comparison object being used by this.
        const CompareType& GetKeyComp() const
            { return CompFunct; }
        /// @brief Gets a copy of the key comparison object.
        /// @return Returns a newly constructed copy of the key comparison object being used by this.
        CompareType GetKeyCompCopy() const
            { return CompFunct; }

        /// @brief Compares two stored type values to see which one should be sorted before the other.
        /// @param Left The first stored type to be compared.
        /// @param Right The second stored type to be compared.
        /// @return Returns true if the Left pair should be sorted before the Right pair, false otherwise.
        Boole operator()(const StoredType& Left, const StoredType& Right) const
            { return CompFunct(Left.first,Right.first); }
        /// @brief Compares a key on a stored type to a standalone key to see which one should be sorted before the other.
        /// @param Key The key to compare to the stored pair.
        /// @param Pair The pair whose key will be used for the comparison.
        /// @return Returns true if the Left key should be sorted before the Right key, false otherwise.
        Boole operator()(const KeyType& Key, const StoredType& Pair) const
            { return CompFunct(Key,Pair.first); }
        /// @brief Compares a key on a stored type to a standalone key to see which one should be sorted before the other.
        /// @param Pair The pair whose key will be used for the comparison.
        /// @param Key The key to compare to the stored pair.
        /// @return Returns true if the Left key should be sorted before the Right key, false otherwise.
        Boole operator()(const StoredType& Pair, const KeyType& Key) const
            { return CompFunct(Pair.first,Key); }

        /// @brief Compares a key on a stored type to a standalone key to see which one should be sorted before the other.
        /// @tparam alt_key The type for the key to compare.  This must be a comparison supported by CompareType.
        /// @param Key The key to compare to the stored pair.
        /// @param Pair The pair whose key will be used for the comparison.
        /// @return Returns true if the Left key should be sorted before the Right key, false otherwise.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<CompareType,alt_key>::value>>
        Boole operator()(const alt_key& Key, const StoredType& Pair)
            { return CompFunct(Key,Pair.first); }
        /// @brief Compares a key on a stored type to a standalone key to see which one should be sorted before the other.
        /// @tparam alt_key The type for the key to compare.  This must be a comparison supported by CompareType.
        /// @param Pair The pair whose key will be used for the comparison.
        /// @param Key The key to compare to the stored pair.
        /// @return Returns true if the Left key should be sorted before the Right key, false otherwise.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<CompareType,alt_key>::value>>
        Boole operator()(const StoredType& Pair, const alt_key& Key)
            { return CompFunct(Pair.first,Key); }
    };//FlatPairCompare

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This container sorts associative pairs in contiguous memory for fast associative lookups.
    /// @tparam KeyType The type of key that will be used for element lookups.
    /// @tparam ElementType The type of element this container will store.
    /// @tparam Compare The comparator functor to use for sorting keys (and thus their associated elements).
    /// @tparam Alloc Allocator type for elements in the map.
    /// @remarks This container tries to adhere to the api provided by the C++14 std::map as much as it
    /// can, with the addition of the "contains()" method that was included in C++20.  C++17 methods were
    /// consciously omitted due to the additional complexity it would introduce.
    ///////////////////////////////////////
    template< typename KeyType,
              typename ElementType,
              typename Compare = std::less<KeyType>,
              typename Alloc = std::allocator< std::pair<const KeyType,ElementType> > >
    class FlatMap
    {
    public:
        /// @brief Convenience type to refer to the type of this.
        using SelfType = FlatMap<KeyType,ElementType,Compare,Alloc>;
        /// @brief The type used to sort the associative pair.
        using key_type = KeyType;
        /// @brief The type of value actually being stored in the associative pair.
        using mapped_type = ElementType;
        /// @brief The type of the key/value associative pair used by this adapter.
        using value_type = std::pair<const key_type,mapped_type>;
        /// @brief The type of the key/value associative pair stored by the underlying container.
        using stored_type = std::pair<key_type,mapped_type>;
        /// @brief The type of underlying container used to store all of the mapped keys and values.
        using container_type = std::vector<stored_type>;
        /// @brief The type of allocator being used when an allocation be required.
        using allocator_type = Alloc;
        /// @brief Iterator type pointing to the associative pair.
        using iterator = typename container_type::iterator;
        /// @brief Const Iterator type pointing to the associative pair.
        using const_iterator = typename container_type::const_iterator;
        /// @brief Iterator type that advances in reverse order.
        using reverse_iterator = typename container_type::reverse_iterator;
        /// @brief Const Iterator type that advances in reverse order.
        using const_reverse_iterator = typename container_type::const_reverse_iterator;
        /// @brief A convenience type for a pair of iterators from this map.
        using iterator_pair = typename std::pair<iterator,iterator>;
        /// @brief A convenience type for a pair of const iterators from this map.
        using const_iterator_pair = typename std::pair<const_iterator,const_iterator>;
        /// @brief Type suitable to describe the positional difference between two iterators.
        using difference_type = typename container_type::difference_type;
        /// @brief Type suitable to describe the size of the container.  Cannot be negative.
        using size_type = typename container_type::size_type;
        /// @brief Comparison object type for sorting keys.
        using key_compare = Compare;
        /// @brief Comparison object type for sorting key/value pair objects.
        using value_compare = FlatPairCompare<stored_type,key_compare>;
    protected:
        /// @brief The internal container storing all of our pairs.
        container_type InternalStorage;
        /// @brief The comparison object to be used with stl algorithms.
        value_compare CompareObj{ key_compare() };
    public:
        /// @brief Default constructor.
        FlatMap() = default;
        /// @brief Copy constructor.
        /// @param Other The other FlatMap to be copied.
        FlatMap(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other FlatMap to be moved.
        FlatMap(SelfType&& Other) = default;
        /// @brief Range constructor.
        /// @param Begin A const iterator to the beginning of the range to insert.
        /// @param End A const iterator to the end of the range to insert.
        template<typename InputIterator>
        FlatMap(InputIterator Begin, InputIterator End)
            { insert(Begin,End); }
        /// @brief Initializer list constructor.
        /// @param List An initializer list of elements to insert.
        FlatMap(std::initializer_list<value_type> List)
            { insert(List.begin(),List.end()); }
        /// @brief Class destructor.
        ~FlatMap() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy-assignment operator.
        /// @param Other The other FlatMap to be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move-assignment operator.
        /// @param Other The other FlatMap to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Allocator

        /// @brief Gets the allocator object being used by this container.
        /// @return Returns the allocator being used by this container.
        allocator_type get_allocator() const
            { return InternalStorage.get_allocator(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Iterators

        /// @brief Get an iterator to the beginning of the container.
        /// @return A mutable iterator pointing to the first pair.
        iterator begin()
            { return InternalStorage.begin(); }
        /// @brief Get a const iterator to the beginning of the container.
        /// @return A const iterator pointing to the first pair.
        const_iterator begin() const
            { return InternalStorage.begin(); }
        /// @brief Get a const iterator to the beginning of the container.
        /// @return A const iterator pointing to the first pair.
        const_iterator cbegin() const
            { return InternalStorage.cbegin(); }

        /// @brief Get an iterator to one past the end of the container.
        /// @return A mutable iterator to one past the end of the container.
        iterator end()
            { return InternalStorage.end(); }
        /// @brief Get a const iterator to one past the end of the container.
        /// @return A const iterator to one past the end of the container.
        const_iterator end() const
            { return InternalStorage.end(); }
        /// @brief Get a const iterator to one past the end of the container.
        /// @return A const iterator to one past the end of the container.
        const_iterator cend() const
            { return InternalStorage.cend(); }

        /// @brief Get an iterator to the last pair of the container, going backwards.
        /// @return A mutable reverse iterator pointing to the first pair at the end.
        reverse_iterator rbegin()
            { return InternalStorage.rbegin(); }
        /// @brief Get a const iterator to the last pair of the container, going backwards.
        /// @return A const reverse iterator pointing to the first pair at the end.
        const_reverse_iterator rbegin() const
            { return InternalStorage.rbegin(); }
        /// @brief Get a const iterator to the last pair of the container, going backwards.
        /// @return A const reverse iterator pointing to the first pair at the end.
        const_reverse_iterator crbegin() const
            { return InternalStorage.crbegin(); }

        /// @brief Get an iterator to one before the first pair of the container.
        /// @return A mutable reverse iterator pointing to the reverse end pair at the start.
        reverse_iterator rend()
            { return InternalStorage.rend(); }
        /// @brief Get a const iterator to one before the first pair of the container.
        /// @return A const reverse iterator pointing to the reverse end pair at the start.
        const_reverse_iterator rend() const
            { return InternalStorage.rend(); }
        /// @brief Get a const iterator to one before the first pair of the container.
        /// @return A const reverse iterator pointing to the reverse end pair at the start.
        const_reverse_iterator crend() const
            { return InternalStorage.crend(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Capacity

        /// @brief Gets he amount of pairs currently stored in this map.
        /// @return Returns the amount of pair storage currently consumed in this map.
        size_type size() const
            { return InternalStorage.size(); }
        /// @brief Gets the maximum number of pairs this map can support.
        /// @remarks This function does the same/is the same as capacity().  It exists for
        /// compatibility with container adapters.
        /// @return Returns the total number of pairs this map can store.
        size_type max_size() const
            { return InternalStorage.max_size(); }
        /// @brief Gets whether or not this map is empty.
        /// @return Returns true if this map is not storing any pairs, false otherwise.
        Boole empty() const
            { return InternalStorage.empty();  }

        ///////////////////////////////////////////////////////////////////////////////
        // Element Access

        /// @brief Gets the element associated with the specified key.
        /// @warning Unlike normal maps, adding or removing elements can invalidate iterators and
        /// references if the underlying container does a reallocation.  Avoid adding or removing
        /// elements while holding on to references retrieved via this method.
        /// @note If a pair with the specified key does not already exist, a new
        /// element with that key will be created, inserted, and returned.
        /// @param Key The key mapped to the stored data that will be retrieved.
        /// @return Returns a reference to the mapped value associated with the specified key.
        mapped_type& operator[] (const key_type& Key)
        {
            iterator InsertIt = lower_bound(Key);
            if( InsertIt == end() ) { // Doesn't exist, everything is lower so insert at end
                InternalStorage.push_back( std::make_pair(Key,mapped_type()) );
                return InternalStorage.back().second;
            }
            if( !CompareObj.GetKeyComp()(Key,(*InsertIt).first) ) { // Exact match
                return (*InsertIt).second;
            }
            // Doesn't exist, insert somewhere in the sequence
            InsertIt = emplace_hint( std::next(InsertIt), Key, mapped_type() );
            return (*InsertIt).second;
        }
        /// @brief Gets the element associated with the specified key being moved.
        /// @warning Unlike normal maps, adding or removing elements can invalidate iterators and
        /// references if the underlying container does a reallocation.  Avoid adding or removing
        /// elements while holding on to references retrieved via this method.
        /// @note If a pair with the specified key does not already exist, a new
        /// element with that key will be created, inserted, and returned.
        /// @param Key The key mapped to the stored data that will be retrieved.  Will be used for
        /// move construction if the pair does not already exist.
        /// @return Returns a reference to the mapped value associated with the specified key.
        mapped_type& operator[] (key_type&& Key)
        {
            iterator InsertIt = lower_bound(Key);
            if( InsertIt == end() ) { // Doesn't exist, everything is lower so insert at end
                InternalStorage.push_back( std::make_pair(Key,mapped_type()) );
                return InternalStorage.back().second;
            }
            if( !CompareObj.GetKeyComp()(Key,(*InsertIt).first) ) { // Exact match
                return (*InsertIt).second;
            }
            // Doesn't exist, insert somewhere in the sequence
            InsertIt = emplace_hint( std::next(InsertIt), std::move(Key), mapped_type() );
            return (*InsertIt).second;
        }
        /// @brief Gets the element associated with the specified key.
        /// @warning Unlike normal maps, adding or removing elements can invalidate iterators and
        /// references if the underlying container does a reallocation.  Avoid adding or removing
        /// elements while holding on to references retrieved via this method.
        /// @exception If no values are mapped to the key specified, a std::out_of_range
        /// exception will be thrown.
        /// @param Key The key mapped to the stored data that will be retrieved.
        /// @return Returns a reference to the mapped value associated with the specified key.
        mapped_type& at(const key_type& Key)
        {
            iterator FoundIt = find(Key);
            if( FoundIt == end() ) {
                throw std::out_of_range("Specified key was not found in FlatMap.");
            }
            return (*FoundIt).second;
        }
        /// @brief Gets the element associated with the specified key.
        /// @warning Unlike normal maps, adding or removing elements can invalidate iterators and
        /// references if the underlying container does a reallocation.  Avoid adding or removing
        /// elements while holding on to references retrieved via this method.
        /// @exception If no values are mapped to the key specified, a std::out_of_range
        /// exception will be thrown.
        /// @param Key The key mapped to the stored data that will be retrieved.
        /// @return Returns a const reference to the mapped value associated with the specified key.
        const mapped_type& at(const key_type& Key) const
        {
            const_iterator FoundIt = find(Key);
            if( FoundIt == end() ) {
                throw std::out_of_range("Specified key was not found in FlatMap.");
            }
            return (*FoundIt).second;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Lookup

        /// @brief Gets the number of stored pairs with keys matching a specified value.
        /// @remarks This method exists primarily for api compatibility with multi_maps.  This method will always return 1 or 0.
        /// @param Key The key to check for.
        /// @return Returns the number of stored pairs that match the specified key.
        size_type count(const key_type& Key) const
            { return ( find(Key) != end() ? 1 : 0 ); }
        /// @brief Gets the number of stored pairs with keys matching a specified value.
        /// @tparam alt_key An type not matching key_type but is still equivalent comparable.
        /// @param Key The key to check for.
        /// @return Returns the number of stored pairs that match the specified key.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        size_type count(const alt_key& Key) const
            { return ( find(Key) != end() ? 1 : 0 ); }

        /// @brief Gets an iterator to the stored pair with a specific key.
        /// @param Key The key to search for.
        /// @return Returns an iterator to to the pair matching the specified key, or end() if no pair was found.
        iterator find(const key_type& Key)
            { return binary_find(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the stored pair with a specific key.
        /// @param Key The key to search for.
        /// @return Returns an const iterator to to the pair matching the specified key, or end() if no pair was found.
        const_iterator find(const key_type& Key) const
            { return binary_find(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the stored pair with a specific key.
        /// @tparam alt_key An type not matching key_type but is still equivalent comparable.
        /// @param Key The key to search for.
        /// @return Returns an iterator to to the pair matching the specified key, or end() if no pair was found.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        iterator find(const alt_key& Key)
            { return binary_find(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the stored pair with a specific key.
        /// @tparam alt_key An type not matching key_type but is still equivalent comparable.
        /// @param Key The key to search for.
        /// @return Returns an const iterator to to the pair matching the specified key, or end() if no pair was found.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        const_iterator find(const alt_key& Key) const
            { return binary_find(begin(),end(),Key,CompareObj); }

        /// @brief Gets whether or not this container has a specific key stored.
        /// @param Key The key to search for.
        /// @return Returns true if the key was found inside this container, false otherwise.
        Boole contains(const key_type& Key) const
            { return ( find(Key) != end() ); }
        /// @brief Gets whether or not this container has a specific key stored.
        /// @tparam alt_key An type not matching key_type but is still equivalent comparable.
        /// @param Key The key to search for.
        /// @return Returns true if the key was found inside this container, false otherwise.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        Boole contains(const alt_key& Key) const
            { return ( find(Key) != end() ); }

        /// @brief Gets an iterator pair representing the range of elements equal to the key.
        /// @remarks This method exists primarily for api compatibility with multi_maps.  This
        /// method will only ever return a range of one or two end iterators.
        /// @param Key The key to search for.
        /// @return Returns a pair of iterators containing the range of elements equal to the
        /// specified key.
        iterator_pair equal_range(const key_type& Key)
            { return std::equal_range(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator pair representing the range of elements equal to the key.
        /// @remarks This method exists primarily for api compatibility with multi_maps.  This
        /// method will only ever return a range of one or two end iterators.
        /// @param Key The key to search for.
        /// @return Returns a pair of const iterators containing the range of elements equal
        /// to the specified key.
        const_iterator_pair equal_range(const key_type& Key) const
            { return std::equal_range(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator pair representing the range of elements equal to the key.
        /// @remarks This method exists primarily for api compatibility with multi_maps.  This
        /// method will only ever return a range of one or two end iterators.
        /// @param Key The key to search for.
        /// @return Returns a pair of iterators containing the range of elements equal to the
        /// specified key.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        iterator_pair equal_range(const alt_key& Key)
            { return std::equal_range(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator pair representing the range of elements equal to the key.
        /// @remarks This method exists primarily for api compatibility with multi_maps.  This
        /// method will only ever return a range of one or two end iterators.
        /// @param Key The key to search for.
        /// @return Returns a pair of const iterators containing the range of elements equal
        /// to the specified key.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        const_iterator_pair equal_range(const alt_key& Key) const
            { return std::equal_range(begin(),end(),Key,CompareObj); }

        /// @brief Gets an iterator to the first element that doesn't compare less than the key.
        /// @param Key The key to search for.
        /// @return Returns an iterator to first element that is not less than the key specified,
        /// or the end iterator if no such match could be found.
        iterator lower_bound(const key_type& Key)
            { return std::lower_bound(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the first element that doesn't compare less than the key.
        /// @param Key The key to search for.
        /// @return Returns a const iterator to first element that is not less than the key
        /// specified, or the end iterator if no such match could be found.
        const_iterator lower_bound(const key_type& Key) const
            { return std::lower_bound(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the first element that doesn't compare less than the key.
        /// @param Key The key to search for.
        /// @return Returns an iterator to first element that is not less than the key specified,
        /// or the end iterator if no such match could be found.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        iterator lower_bound(const alt_key& Key)
            { return std::lower_bound(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the first element that doesn't compare less than the key.
        /// @param Key The key to search for.
        /// @return Returns a const iterator to first element that is not less than the key
        /// specified, or the end iterator if no such match could be found.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        const_iterator lower_bound(const alt_key& Key) const
            { return std::lower_bound(begin(),end(),Key,CompareObj); }

        /// @brief Gets an iterator to the first element that compares greater than the key.
        /// @param Key The key to search for.
        /// @return Returns an iterator to the first element greater than the key specified,
        /// or the end iterator if no such match could be made.
        iterator upper_bound(const key_type& Key)
            { return std::upper_bound(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the first element that compares greater than the key.
        /// @param Key The key to search for.
        /// @return Returns a const iterator to the first element greater than the key specified,
        /// or the end iterator if no such match could be made.
        const_iterator upper_bound(const key_type& Key) const
            { return std::upper_bound(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the first element that compares greater than the key.
        /// @param Key The key to search for.
        /// @return Returns an iterator to the first element greater than the key specified,
        /// or the end iterator if no such match could be made.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        iterator upper_bound(const alt_key& Key)
            { return std::upper_bound(begin(),end(),Key,CompareObj); }
        /// @brief Gets an iterator to the first element that compares greater than the key.
        /// @param Key The key to search for.
        /// @return Returns a const iterator to the first element greater than the key specified,
        /// or the end iterator if no such match could be made.
        template<class alt_key, typename = std::enable_if_t<Is_Transparent<Compare,alt_key>::value>>
        const_iterator upper_bound(const alt_key& Key) const
            { return std::upper_bound(begin(),end(),Key,CompareObj); }

        ///////////////////////////////////////////////////////////////////////////////
        // Sequence Modifiers

        /// @brief Inserts a new Key/Value pair if no such key already exists.
        /// @param Val The Key/Value pair to be inserted.
        /// @return If the insert was successful a pair containing an iterator to the newly
        /// inserted value_type and true will be returned.  If the insert was unsuccessful
        /// then a pair containing an iterator to the value_type that blocked the insert
        /// and false will be returned.
        std::pair<iterator,Boole> insert(const value_type& Val)
        {
            iterator FoundIt = lower_bound(Val.first);
            if( FoundIt != end() && (*FoundIt).first == Val.first ) {
                return std::make_pair(FoundIt,false);
            }else{
                iterator InsertIt = InternalStorage.insert(FoundIt,Val);
                return std::make_pair(InsertIt,true);
            }
        }
        /// @brief Inserts a new Key/Value pair if no such key already exists.
        /// @param Val The Key/Value pair to be inserted.
        /// @return If the insert was successful a pair containing an iterator to the newly
        /// inserted value_type and true will be returned.  If the insert was unsuccessful
        /// then a pair containing an iterator to the value_type that blocked the insert
        /// and false will be returned.
        std::pair<iterator,Boole> insert(value_type&& Val)
        {
            iterator FoundIt = lower_bound(Val.first);
            if( FoundIt != end() && (*FoundIt).first == Val.first ) {
                return std::make_pair(FoundIt,false);
            }else{
                iterator InsertIt = InternalStorage.insert( FoundIt, std::move(Val) );
                return std::make_pair(InsertIt,true);
            }
        }
        /// @brief Inserts a new Key/Value pair if no such key already exists with a hit of where to insert.
        /// @param Hint An iterator to the element that will follow the newly inserted element.
        /// @param Val The Key/Value pair to be inserted.
        /// @return If the insert was successful an iterator to the newly inserted value_type
        /// will be returned.  If the insert was unsuccessful then an iterator to the value_type
        /// that blocked the insert will be returned.
        iterator insert(const_iterator Hint, const value_type& Val)
        {
            if( empty() ) {
                InternalStorage.push_back(Val);
                return InternalStorage.begin();
            }
            if( Hint == begin() && CompareObj(Val,*Hint) ) {
                return InternalStorage.insert(begin(),Val);
            }
            const_iterator PrevIt = std::prev(Hint);
            if( Hint == end() && CompareObj(*PrevIt,Val) ) {
                return InternalStorage.insert(end(),Val);
            }
            if( CompareObj(*PrevIt,Val) && CompareObj(Val,*Hint) ) {
                return InternalStorage.insert(Hint,Val);
            }
            // All sane shortcuts have failed, so ignore hint
            std::pair<iterator,Boole> Ret = insert(Val);
            return Ret.first;
        }
        /// @brief Inserts a new Key/Value pair if no such key already exists with a hit of where to insert.
        /// @param Hint An iterator to the element that will follow the newly inserted element.
        /// @param Val The Key/Value pair to be inserted.
        /// @return If the insert was successful an iterator to the newly inserted value_type
        /// will be returned.  If the insert was unsuccessful then an iterator to the value_type
        /// that blocked the insert will be returned.
        iterator insert(const_iterator Hint, value_type&& Val)
        {
            if( empty() ) {
                InternalStorage.push_back( std::move(Val) );
                return InternalStorage.begin();
            }
            if( Hint == begin() && CompareObj(Val,*Hint) ) {
                return InternalStorage.insert( begin(), std::move(Val) );
            }
            const_iterator PrevIt = std::prev(Hint);
            if( Hint == end() && CompareObj(*PrevIt,Val) ) {
                return InternalStorage.insert( end(), std::move(Val) );
            }
            if( CompareObj(*PrevIt,Val) && CompareObj(Val,*Hint) ) {
                return InternalStorage.insert( Hint, std::move(Val) );
            }
            // All sane shortcuts have failed, so ignore hint
            std::pair<iterator,Boole> Ret = insert( std::move(Val) );
            return Ret.first;
        }
        /// @brief Inserts a range of elements into the container.
        /// @param First An iterator to the start of the range to be inserted.
        /// @param Last An iterator to one-passed-the-final element to be inserted.
        template<class ItType>
        void insert(ItType First, ItType Last)
        {
            while( First != Last )
            {
                insert( *First );
                ++First;
            }
        }
        /// @brief Inserts a range of elements into the container.
        /// @param List An initializer list storing all the elements to be inserted.
        void insert(std::initializer_list<value_type> List)
        {
            insert(List.begin(),List.end());
        }

        /// @brief Inserts just like the "insert" method, but with in-place construction for the element.
        /// @tparam ArgTypes A collection of deduced types to use for the construction of stored elements.
        /// @details Due to the memory layout of this container and the bundle of parameters from the
        /// variadic template, an ideal implementation of this method isn't possible.  A construct and
        /// a move is the simplest implementation possible to maintain api compatibility.
        /// @param Args Variadic template of construction parameters.
        /// @return If the insert was successful a pair containing an iterator to the newly
        /// inserted value_type and true will be returned.  If the insert was unsuccessful
        /// then a pair containing an iterator to the value_type that blocked the insert
        /// and false will be returned.
        template<class... ArgTypes>
        std::pair<iterator,Boole> emplace(ArgTypes&&... Args)
        {
            value_type ToMove(std::forward<ArgTypes>(Args)...);
            return this->insert( std::move(ToMove) );
        }

        /// @brief Inserts just like the "insert" method, but with in-place construction for the element and uses a hint.
        /// @tparam ArgTypes A collection of deduced types to use for the construction of stored elements.
        /// @details Due to the memory layout of this container and the bundle of parameters from the
        /// variadic template, an ideal implementation of this method isn't possible.  A construct and
        /// a move is the simplest implementation possible to maintain api compatibility.
        /// @param Hint An iterator to the position just after a suggested position in the container to insert.
        /// @param Args Variadic template of construction parameters.
        /// @return If the insert was successful an iterator to the newly inserted value_type
        /// will be returned.  If the insert was unsuccessful then an iterator to the value_type
        /// that blocked the insert will be returned.
        template<class... ArgTypes>
        iterator emplace_hint(const_iterator Hint, ArgTypes&&... Args)
        {
            value_type ToMove(std::forward<ArgTypes>(Args)...);
            return this->insert( Hint, std::move(ToMove) );
        }

        /// @brief Swaps the contents of this container with another of the same type.
        /// @param Other The other FlatMap to swap with.
        void swap(SelfType& Other)
        {
            InternalStorage.swap(Other.InternalStorage);
            std::swap(CompareObj,Other.CompareObj);
        }

        /// @brief Removes a Key/Value pair in this container.
        /// @param Pos An iterator to the element that will be removed.
        /// @return Returns an iterator pointing to the element that follows the removed element.
        iterator erase(iterator Pos)
        {
            return InternalStorage.erase(Pos);
        }
        /// @brief Removes a Key/Value pair in this container.
        /// @param Pos An iterator to the element that will be removed.
        /// @return Returns an iterator pointing to the element that follows the removed element.
        iterator erase(const_iterator Pos)
        {
            return InternalStorage.erase(Pos);
        }
        /// @brief Removes a range of Key/Value pairs from this container.
        /// @param First An iterator to the beginning of the range to remove.
        /// @param Last An iterator to one-passed-the-last element in the range to remove.
        /// @return Returns an iterator pointing to the element that follows the last removed element.
        iterator erase(const_iterator First, const_iterator Last)
        {
            return InternalStorage.erase(First,Last);
        }
        /// @brief Removes a Key/Value pair in this container based on a key.
        /// @param Key The key of the element to be removed.
        /// @return Returns the number of elements with the specified key that were removed.
        size_type erase(const key_type& Key)
        {
            iterator FoundIt = find(Key);
            if( FoundIt != end() ) {
                erase(FoundIt);
                return 1;
            }
            return 0;
        }

        /// @brief Removes all elements from the container.
        void clear()
            { InternalStorage.clear(); }
    };//FlatMap
}//Mezzanine

#endif
