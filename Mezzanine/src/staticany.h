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
#ifndef _staticany_h
#define _staticany_h

/// @file
/// @brief This file includes the declaration and definition for the StaticAny class.

#include "datatypes.h"

namespace Mezzanine
{
    /// @brief A list of operations that can be done on the storage of a StaticAny.
    enum class StaticAnyOperations
    {
        QueryType,
        QuerySize,
        Copy,
        Move,
        Destroy
    };

    // Forward Declare
    template<size_t AnySize>
    class StaticAny;

    template<class ElementType, size_t AnySize>
    ElementType& StaticAnyCast(StaticAny<AnySize>& Any);

    template<class ElementType, size_t AnySize>
    const ElementType& StaticAnyCast(const StaticAny<AnySize>& Any);

    template<class ElementType, size_t AnySize>
    ElementType* StaticAnyCast(StaticAny<AnySize>* Any);

    template<class ElementType, size_t AnySize>
    const ElementType* StaticAnyCast(const StaticAny<AnySize>* Any);

    /// @brief A simple type trait to help identify static_any types.
    template<typename ElementType>
    struct is_static_any : public std::false_type
    {  };

    /// @brief A simple type trait to help identify static_any types.
    template<size_t CheckSize>
    struct is_static_any< StaticAny<CheckSize> > : public std::true_type
    {  };

    /// @brief Convenience type for getting the value of is_static_any.
    template<typename ElementType>
    inline constexpr Boole is_static_any_v = is_static_any<ElementType>::value;

    /// @brief A namespace for utilities that facilitate the operation of the StaticAny.
    namespace StaticAnyHelpers
    {
        /// @brief Umbrella function housing our type-specific operations.
        /// @details This function performs one of multiple operations, as determined by the first
        /// parameter.  The function template type is used for type storage while the StaticAny
        /// instance holds a type-erased pointer to the function.  Having multiple operations in
        /// one function helps reduce the need for multiple function pointers on the StaticAny.
        /// @tparam ElementType The type of element to be stored/managed by the StaticAny using
        /// this function.
        /// @param Op An enum value specifying the operation to be performed.
        /// @param AnyStorage A void pointer to the storage space of the StaticAny being operated
        /// on.  This parameter is is ignored by QueryType and QuerySize operations.
        /// @param Datum A pointer to the data to be used or populated.  This parameter is ignored by
        /// the Destroy operation.  With the Copy and Move operations this parameter should be the
        /// StaticAny being Copied/Moved from (the source of the Copy/Move).
        template<class ElementType>
        void Operation(const StaticAnyOperations Op, void* AnyStorage, void* Datum)
        {
            switch( Op )
            {
                case StaticAnyOperations::QueryType:
                {
                    *reinterpret_cast<const std::type_info**>(Datum) = &typeid(ElementType);
                    break;
                }
                case StaticAnyOperations::QuerySize:
                {
                    *reinterpret_cast<size_t*>(Datum) = sizeof(ElementType);
                    break;
                }
                case StaticAnyOperations::Copy:
                {
                    ElementType* CastedElement = reinterpret_cast<ElementType*>(AnyStorage);
                    ElementType* OtherElement = reinterpret_cast<ElementType*>(Datum);
                    assert(CastedElement);
                    assert(OtherElement);
                    ::new (CastedElement) ElementType(*OtherElement);
                    break;
                }
                case StaticAnyOperations::Move:
                {
                    ElementType* CastedElement = reinterpret_cast<ElementType*>(AnyStorage);
                    ElementType* OtherElement = reinterpret_cast<ElementType*>(Datum);
                    assert(CastedElement);
                    assert(OtherElement);
                    ::new (CastedElement) ElementType( std::move(*OtherElement) );
                    break;
                }
                case StaticAnyOperations::Destroy:
                {
                    ElementType* CastedElement = reinterpret_cast<ElementType*>(AnyStorage);
                    CastedElement->~ElementType();
                    break;
                }
            }
        }

        /// @brief Helper function for getting an alignment appropriate for a given size.
        /// @param Val The value to find the next highest power of 2 for.
        /// @return Returns the next highest power of 2 from the specified value.
        constexpr size_t GetBestAlign(const size_t Val)
        {
            size_t Ret = 1;
            while( Ret < Val )
                { Ret *= 2; }
            return Ret;
        }
    }//StaticAnyHelpers

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A container that uses type-erasure to store a single instance of a type with a specific size.
    ///////////////////////////////////////
    template<size_t AnySize>
    class StaticAny
    {
    public:
        /// @brief Convenience type for the type of "this".
        using SelfType = StaticAny<AnySize>;
        /// @brief Function pointer type containing our operations for the stored data.
        using OperationFunct = void(*)(const StaticAnyOperations Op, void* Any, void* Datum);
        /// @brief The type of internal buffer where our object will be stored.
        using BufferType = typename std::aligned_storage<AnySize,StaticAnyHelpers::GetBestAlign(AnySize)>::type;
    protected:
        // Friend declarations for the various implementations of StaticAnyCast.
        template<size_t>
        friend class StaticAny;

        template<class ElementType, size_t AnyCastSize>
        friend ElementType& StaticAnyCast(StaticAny<AnyCastSize>& Any);

        template<class ElementType, size_t AnyCastSize>
        friend const ElementType& StaticAnyCast(const StaticAny<AnyCastSize>& Any);

        template<class ElementType, size_t AnyCastSize>
        friend ElementType* StaticAnyCast(StaticAny<AnyCastSize>* Any);

        template<class ElementType, size_t AnyCastSize>
        friend const ElementType* StaticAnyCast(const StaticAny<AnyCastSize>* Any);

        /// @brief Pointer to the operation function for performing common operations on our Element.
        OperationFunct ElementOp = nullptr;
        /// @brief Internal buffer storing our type-erased Element.
        BufferType InternalStorage;

        /// @brief Copies an Element into this StaticAny.
        /// @tparam ElementType The type that will be stored.
        /// @param Val The value to be copied into the internal storage.
        template<class ElementType>
        void CopyValue(const ElementType& Val)
        {
            using DecayedElementType = std::decay_t<ElementType>;
            static_assert(AnySize >= sizeof(DecayedElementType),"Element must be able to fit in the allocated space.");
            assert(ElementOp == nullptr);

            ::new( static_cast<void*>( &InternalStorage ) ) DecayedElementType(Val);
            ElementOp = &StaticAnyHelpers::Operation<DecayedElementType>;
        }
        /// @brief Moves an Element into this StaticAny.
        /// @tparam ElementType The type that will be stored.
        /// @param Val The value to be moved into the internal storage.
        template<class ElementType>
        void MoveValue(ElementType&& Val)
        {
            using DecayedElementType = std::decay_t<ElementType>;
            static_assert(AnySize >= sizeof(DecayedElementType),"Element must be able to fit in the allocated space.");
            assert(ElementOp == nullptr);

            ::new( static_cast<void*>( &InternalStorage ) ) DecayedElementType(Val);
            ElementOp = &StaticAnyHelpers::Operation<DecayedElementType>;
        }
        /// @brief A method that attempts to deduce whether the element needs to be copied or moved.
        /// @tparam ElementType The type that will be stored.
        /// @param Val The value to be placed into the internal storage.
        template<class ElementType>
        void CopyOrMoveValue(ElementType&& Val)
        {
            if( std::is_rvalue_reference<ElementType>::value ) {
                MoveValue( std::forward<ElementType>(Val) );
            }else{
                CopyValue( std::forward<ElementType>(Val) );
            }
        }
        /// @brief Copies a StaticAny the same size or smaller into this StaticAny.
        /// @tparam OtherSize The size of the other StaticAny to be copied into this.
        /// @param Other The other StaticAny to be copied.
        template<size_t OtherSize>
        void CopyAny(const StaticAny<OtherSize>& Other)
        {
            static_assert(AnySize >= OtherSize,"Element must be able to fit in the allocated space.");
            assert(ElementOp == nullptr);

            void* ThisData = reinterpret_cast<void*>(&InternalStorage);
            void* OtherData = reinterpret_cast<void*>(const_cast<typename StaticAny<OtherSize>::BufferType*>(&Other.InternalStorage));
            Other.ElementOp(StaticAnyOperations::Copy,ThisData,OtherData);
            ElementOp = Other.ElementOp;
        }
        /// @brief Moves a StaticAny the same size or smaller into this StaticAny.
        /// @tparam OtherSize The size of the other StaticAny to be moved into this.
        /// @param Other The other StaticAny to be moved.
        template<size_t OtherSize>
        void MoveAny(StaticAny<OtherSize>&& Other)
        {
            static_assert(AnySize >= OtherSize,"Element must be able to fit in the allocated space.");
            assert(ElementOp == nullptr);

            void* ThisData = reinterpret_cast<void*>(&InternalStorage);
            void* OtherData = reinterpret_cast<void*>(const_cast<typename StaticAny<OtherSize>::BufferType*>(&Other.InternalStorage));
            Other.ElementOp(StaticAnyOperations::Move,ThisData,OtherData);
            ElementOp = Other.ElementOp;
            Other.ElementOp = nullptr;
        }

        /// @brief Destructs the element being stored and prepares the StaticAny for the storing another element.
        void Destroy()
        {
            if( ElementOp != nullptr ) {
                ElementOp(StaticAnyOperations::Destroy,&InternalStorage,nullptr);
                ElementOp = nullptr;
            }
        }
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        /// @brief No init/empty constructor.
        StaticAny() = default;
        /// @brief Copy constructor.
        /// @param Other The other StaticAny (of the same type) to copy.
        explicit StaticAny(const StaticAny& Other)
            { CopyAny(Other); }
        /// @brief Move constructor.
        /// @param Other The other StaticAny (of the same type) to move.
        explicit StaticAny(StaticAny&& Other)
            { MoveAny( std::move(Other) ); }
        /// @brief Copy from different size StaticAny constructor.
        /// @tparam Size The size of the internal buffer in the StaticAny to copy.
        /// @param Other The other StaticAny to copy.
        template<size_t CheckSize, typename = std::enable_if_t< (CheckSize < AnySize) > >
        StaticAny(const StaticAny<CheckSize>& Other)
            { CopyAny(Other); }
        /// @brief Move from different size StaticAny constructor.
        /// @tparam Size The size of the internal buffer in the StaticAny to move.
        /// @param Other The other StaticAny to move.
        template<size_t CheckSize, typename = std::enable_if_t< (CheckSize < AnySize) > >
        StaticAny(StaticAny<CheckSize>&& Other)
            { MoveAny( std::move(Other) ); }
        /// @brief Move value constructor.
        /// @tparam ElementType The type of element that will be stored inside the StaticAny.
        /// @param Val The value to be moved.
        template<typename ElementType, typename = std::enable_if_t< !is_static_any_v< std::remove_cv_t< std::remove_reference_t<ElementType> > > > >
        StaticAny(ElementType&& Val)
            { CopyOrMoveValue( std::forward<ElementType>( Val ) ); }
        /// @brief Class destructor.
        ~StaticAny()
            { Destroy(); }

        /// @brief Copy assignment operator.
        /// @param Other The other StaticAny of the same type to copy.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other)
        {
            Destroy();
            CopyAny(Other);
            return *this;
        }
        /// @brief Move assignment operator.
        /// @param Other The other StaticAny of the same type to move.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other)
        {
            Destroy();
            MoveAny( std::move(Other) );
            return *this;
        }
        /// @brief Differently sized StaticAny copy assignment operator.
        /// @tparam Size The size of the other StaticAny to be copied.
        /// @param Other The other StaticAny of the same size or smaller to be copied.
        /// @return Returns a reference to this.
        template<size_t Size, class = std::enable_if_t< (Size < AnySize) > >
        SelfType& operator=(const StaticAny<Size>& Other)
        {
            Destroy();
            CopyAny(Other);
            return *this;
        }
        /// @brief Differently sized StaticAny move assignment operator.
        /// @tparam Size The size of the other StaticAny to be moved.
        /// @param Other The other StaticAny of the same size or smaller to be moved.
        /// @return Returns a reference to this.
        template<size_t Size, class = std::enable_if_t< (Size < AnySize) > >
        SelfType& operator=(StaticAny<Size>&& Other)
        {
            Destroy();
            MoveAny( std::move(Other) );
            return *this;
        }
        /// @brief Move Element assignment operator.
        /// @tparam ElementType The type of element to be moved.
        /// @param Val The element to be moved.
        /// @return Returns a reference to this.
        template<typename ElementType, class = std::enable_if_t< !is_static_any_v< std::remove_cv_t< std::remove_reference_t<ElementType> > > > >
        SelfType& operator=(ElementType&& Val)
        {
            Destroy();
            CopyOrMoveValue( std::forward<ElementType>(Val) );
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Constructs an element inside the StaticAny storage.
        /// @tparam ElementType The type of element to be constructed.
        /// @tparam ArgTypes A variadic template of the arguments to the constructor that will be called.
        /// @param Args The values of the variadic template being passed into the element constructor.
        template<class ElementType, class... ArgTypes>
        void emplace(ArgTypes&&... Args)
        {
            using DecayedElementType = std::decay_t<ElementType>;
            Destroy();
            ::new (&InternalStorage) DecayedElementType(std::forward<ArgTypes>(Args)...);
            ElementOp = &StaticAnyHelpers::Operation<DecayedElementType>;
        }

        /// @brief Gets the type of element being stored in this StaticAny.
        /// @return Returns a const reference to an std::type_info of the object being stored.
        const std::type_info& get_type() const
        {
            if( empty() ) {
                return typeid(void);
            }else{
                const std::type_info* Info = nullptr;
                ElementOp(StaticAnyOperations::QueryType,nullptr,reinterpret_cast<void*>(&Info));
                return (*Info);
            }
        }

        /// @brief Gets the size of the stored element.
        /// @return Returns the size of the element stored in this StaticAny in bytes, or 0 if no element is stored.
        size_t size() const
        {
            size_t Ret = 0;
            if( ElementOp != nullptr ) {
                ElementOp(StaticAnyOperations::QuerySize,nullptr,reinterpret_cast<void*>(&Ret));
            }
            return Ret;
        }
        /// @brief Gets the maximum size this StaticAny can store.
        /// @return Returns the "AnySize" template parameter of this StaticAny.
        static constexpr size_t capacity()
            { return AnySize; }
        /// @brief Checks to see if the StaticAny is currently empty.
        /// @return Returns true if the StaticAny isn't storing an element, false otherwise.
        bool empty() const
            { return ElementOp == nullptr; }

        /// @brief Destroys the current object being stored by the StaticAny.
        void clear()
            { Destroy(); }
    };//StaticAny

    /// @brief Casts a StaticAny into its appropriate type.
    /// @warning You can only cast a StaticAny into the exact type that that was stored in it.  You cannot cast
    /// a StaticAny into a covariant or any other type.  Attempting to do so will cause a std::bad_cast exception
    /// to be thrown.
    /// @exception If the type being stored by the StaticAny doesn't match the type being casted to a
    /// std::bad_cast exception will be thrown.
    /// @tparam ElementType The type to cast the type-erased element in the StaticAny to.
    /// @tparam AnyCastSize The (deduced) size of the internal storage of the StaticAny.
    /// @param Any The StaticAny to cast.
    /// @return Returns a reference to the internal buffer of the type specified by ElementType.
    template<class ElementType, size_t AnyCastSize>
    inline ElementType& StaticAnyCast(StaticAny<AnyCastSize>& Any)
    {
        //if( typeid(ElementType) != Any.get_type() ) { // Using std::type_index makes the comparisons safer in some linking contexts.
        if( std::type_index( typeid(ElementType) ) != std::type_index( Any.get_type() ) ) {
            throw std::bad_cast();
        }
        return reinterpret_cast<ElementType&>( Any.InternalStorage );
    }
    /// @brief Casts a StaticAny into its appropriate type.
    /// @warning You can only cast a StaticAny into the exact type that that was stored in it.  You cannot cast
    /// a StaticAny into a covariant or any other type.  Attempting to do so will cause a std::bad_cast exception
    /// to be thrown.
    /// @exception If the type being stored by the StaticAny doesn't match the type being casted to a
    /// std::bad_cast exception will be thrown.
    /// @tparam ElementType The type to cast the type-erased element in the StaticAny to.
    /// @tparam AnyCastSize The (deduced) size of the internal storage of the StaticAny.
    /// @param Any The StaticAny to cast.
    /// @return Returns a const reference to the internal buffer of the type specified by ElementType.
    template<class ElementType, size_t AnyCastSize>
    inline const ElementType& StaticAnyCast(const StaticAny<AnyCastSize>& Any)
    {
        //if( typeid(ElementType) != Any.get_type() ) { // Using std::type_index makes the comparisons safer in some linking contexts.
        if( std::type_index( typeid(ElementType) ) != std::type_index( Any.get_type() ) ) {
            throw std::bad_cast();
        }
        return reinterpret_cast<const ElementType&>( Any.InternalStorage );
    }
    /// @brief Casts a StaticAny into its appropriate type.
    /// @warning You can only cast a StaticAny into the exact type that that was stored in it.  You cannot cast
    /// a StaticAny into a covariant or any other type.  Attempting to do so will cause a std::bad_cast exception
    /// to be thrown.
    /// @exception If the type being stored by the StaticAny doesn't match the type being casted to a
    /// std::bad_cast exception will be thrown.
    /// @tparam ElementType The type to cast the type-erased element in the StaticAny to.
    /// @tparam AnyCastSize The (deduced) size of the internal storage of the StaticAny.
    /// @param Any The StaticAny to cast.
    /// @return Returns a pointer to the internal buffer of the type specified by ElementType.
    template<class ElementType, size_t AnyCastSize>
    inline ElementType* StaticAnyCast(StaticAny<AnyCastSize>* Any)
    {
        //if( typeid(ElementType) != Any.get_type() ) { // Using std::type_index makes the comparisons safer in some linking contexts.
        if( std::type_index( typeid(ElementType) ) != std::type_index( Any.get_type() ) ) {
            throw std::bad_cast();
        }
        return reinterpret_cast<ElementType*>( Any.InternalStorage );
    }
    /// @brief Casts a StaticAny into its appropriate type.
    /// @warning You can only cast a StaticAny into the exact type that that was stored in it.  You cannot cast
    /// a StaticAny into a covariant or any other type.  Attempting to do so will cause a std::bad_cast exception
    /// to be thrown.
    /// @exception If the type being stored by the StaticAny doesn't match the type being casted to a
    /// std::bad_cast exception will be thrown.
    /// @tparam ElementType The type to cast the type-erased element in the StaticAny to.
    /// @tparam AnyCastSize The (deduced) size of the internal storage of the StaticAny.
    /// @param Any The StaticAny to cast.
    /// @return Returns a const pointer to the internal buffer of the type specified by ElementType.
    template<class ElementType, size_t AnyCastSize>
    inline const ElementType* StaticAnyCast(const StaticAny<AnyCastSize>* Any)
    {
        //if( typeid(ElementType) != Any.get_type() ) { // Using std::type_index makes the comparisons safer in some linking contexts.
        if( std::type_index( typeid(ElementType) ) != std::type_index( Any.get_type() ) ) {
            throw std::bad_cast();
        }
        return reinterpret_cast<const ElementType*>( Any.InternalStorage );
    }

    template<size_t AnySize>
    class TrivialStaticAny
    {
    protected:
    public:
        TrivialStaticAny();
        ~TrivialStaticAny();
    };//TrivialStaticAny
}//Mezzanine

#endif // _staticany_h
