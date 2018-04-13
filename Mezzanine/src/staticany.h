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

    /// @brief A simple type trait to help identify static_any types.
    template<typename ElementType>
    struct is_static_any : public std::false_type
    {  };

    /// @brief A simple type trait to help identify static_any types.
    template<size_t AnySize>
    struct is_static_any<StaticAny<AnySize>> : public std::true_type
    {  };

    /// @brief Convenience type for getting the value of is_static_any.
    template<typename ElementType>
    inline constexpr Boole is_static_any_t = is_static_any<ElementType>::value;

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
        using OperationFunct = void(*)(const StaticAnyOperations Op, void* Any, void* Datum)
        /// @brief The type of internal buffer where our object will be stored.
        using BufferType = typename std::aligned_storage<AnySize,AnySize>::type;
    protected:
        /// @brief Pointer to the operation function for performing common operations on our Element.
        OperationFunct ElementOp = nullptr;
        /// @brief Internal buffer storing our type-erased Element.
        BufferType InternalStorage;

        /// @brief Umbrella function housing our type-specific operations.
        /// @details This function performs one of multiple operations, as determined by the first
        /// parameter.  The function template type is used for type storage while the StaticAny
        /// instance holds a type-erased pointer to the function.  Having multiple operations in
        /// one function helps reduce the need for multiple function pointers on the StaticAny.
        /// @param Op An enum value specifying the operation to be performed.
        /// @param Any A void pointer to the StaticAny instance to be operated on if applicable.  This
        /// parameter is ignored by QueryType and QuerySize operations.
        /// @param Datum A pointer to the data to be used or populated.  This parameter is ignored by
        /// the Destroy operation.  With the Copy and Move operations this parameter should be the
        /// StaticAny being Copied/Moved from (the source of the Copy/Move).
        template<class StaticAnyType>
        static void Operation(const StaticAnyOperations Op, void* Any, void* Datum)
        {
            switch( Op )
            {
                case StaticAnyOperations::QueryType:
                {
                    *reinterpret_cast<const std::type_info**>(Datum) = &typeid(StaticAnyType);
                    break;
                }
                case StaticAnyOperations::QuerySize:
                {
                    *reinterpret_cast<size_t*>(Datum) = sizeof(StaticAnyType);
                    break;
                }
                case StaticAnyOperations::Copy:
                {
                    StaticAnyType* CastedAny = reinterpret_cast<StaticAnyType*>(Any);
                    StaticAnyType* OtherAny = reinterpret_cast<StaticAnyType*>(Datum);
                    assert(CastedAny);
                    assert(OtherAny);
                    ::new (CastedAny) StaticAnyType(*OtherAny);
                    break;
                }
                case StaticAnyOperations::Move:
                {
                    StaticAnyType* CastedAny = reinterpret_cast<StaticAnyType*>(Any);
                    StaticAnyType* OtherAny = reinterpret_cast<StaticAnyType*>(Datum);
                    assert(CastedAny);
                    assert(OtherAny);
                    ::new (CastedAny) StaticAnyType( std::move(*OtherAny) );
                    break;
                }
                case StaticAnyOperations::Destroy:
                {
                    StaticAnyType* CastedAny = reinterpret_cast<StaticAnyType*>(Any);
                    CastedAny->~StaticAnyType();
                    break;
                }
            }
        }

        template<class ElementType>
        void CopyOrMove(ElementType&& Val)
        {
            static_assert(AnySize >= sizeof(ElementType));
            assert(ElementOp == nullptr);

            using NonConstElementType = std::remove_cv_t< std::remove_reference_t<ElementType> >;
            NonConstElementType* NonConstParam = const_cast<NonConstElementType*>(&Val);
        }

        template<class ElementType>
        void AssignFromAny(ElementType&& Val)
        {

        }

        void Destroy()
        {
            if( ElementOp != nullptr ) {
                ElementOp(StaticAnyOperations::Destroy,&InternalStorage[0],nullptr);
                ElementOp = nullptr;
            }
        }

        /*
        template<typename ElementType, class... ArgTypes>
        void Create(ArgTypes&&... Args)
        {
            ::new (static_cast<void*>(&InternalStorage[0])) ElementType(std::forward<ArgTypes>(Args)...);
            ElementInfo = typeid(ElementType);
        }

        template<typename ElementType>
        ElementType* Retrieve()
        {
            return ( reinterpret_cast<ElementType*>( &InternalStorage[0] ) );
        }

        template<typename ElementType>
        Boole VerifyElementType()
        {
            return ( typeid(ElementType) == ElementInfo );
        }

        template<typename ElementType>
        void Destroy()
        {
            Retrieve()->~ElementType();
            ElementInfo = nullptr;
        }//*/
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        StaticAny() = default;

        StaticAny(const SelfType& Other)
        {

        }

        template<size_t Size, class = std::enable_if_t<Size <= AnySize> >
        StaticAny(const StaticAny<Size>& Other)
        {

        }

        template<size_t Size, class = std::enable_if_t<Size <= AnySize> >
        StaticAny(StaticAny<Size>&& Other)
        {

        }

        template<typename ElementType, class = std::enable_if_t< !is_static_any_t< std::decay_t<ElementType> > > >
        StaticAny(ElementType&& Val)
        {

        }

        /// @brief Class destructor.
        ~StaticAny()
            { Destroy(); }

        SelfType& operator=(const SelfType& Other)
        {

        }

        template<size_t Size, class = std::enable_if_t<Size <= AnySize> >
        SelfType& operator=(const StaticAny<Size>& Other)
        {

        }

        template<size_t Size, class = std::enable_if_t<Size <= AnySize> >
        SelfType& operator=(StaticAny<Size>&& Other)
        {

        }

        template<typename ElementType, class = std::enable_if_t< !is_static_any_t< std::decay_t<ElementType> > > >
        SelfType& operator=(ElementType&& Val)
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Destroys the current object being stored by the StaticAny.
        void Reset()
            { Destroy(); }
    };//StaticAny

    template<size_t AnySize>
    TrivialStaticAny
    {
    protected:
    public:
        TrivialStaticAny();
        ~TrivialStaticAny();
    };//TrivialStaticAny
}//Mezzanine

#endif // _staticany_h
