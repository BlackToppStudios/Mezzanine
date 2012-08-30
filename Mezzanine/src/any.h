//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _any_h
#define _any_h

#include "datatypes.h"
#include "exception.h"

#ifndef SWIG
    #include <typeinfo>
#endif

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class AnyPlaceHolder
    /// @headerfile any.h
    /// @brief This is an abstract interface for a Holder to be used with the Any class.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AnyPlaceHolder
    {
        public:
            /// @brief Class destructor.
            virtual ~AnyPlaceHolder() {};
            /// @brief Gets the type-info of the stored data.
            /// @return Returns a type_info struct detailing the type of data stored.
            virtual const std::type_info& GetType() const = 0;
            /// @brief Creates a duplicate of this AnyPlaceHolder.
            /// @return Returns a pointer to the created duplicate.
            virtual AnyPlaceHolder* Clone() const = 0;
            /// @brief Writes this AnyPlaceHolder's data to an output stream.
            /// @param Out The stream to be written to.
			virtual void WriteToStream(std::ostream& Out) = 0;
    };//AnyPlaceHolder

    ///////////////////////////////////////////////////////////////////////////////
    /// @class AnyHolder
    /// @headerfile any.h
    /// @brief This is a holder for the actual value stored by an Any.
    /// @details
    ///////////////////////////////////////
    template <typename ValueType>
    class MEZZ_LIB AnyHolder : public AnyPlaceHolder
    {
        public:
            /// @brief The data being stored.
            ValueType Held;

            /// @brief Class Constructor.
            /// @param Value The value to set as.
            AnyHolder(const ValueType& Value)
              : Held(Value) {}
            /// @brief Class Destructor.
            virtual ~AnyHolder() {}

            /// @copydoc AnyPlaceHolder::GetType()
            virtual const std::type_info& GetType() const
                { return typeid(ValueType); }
            /// @copydoc AnyPlaceHolder::Clone()
            virtual AnyPlaceHolder* Clone() const
                { return new AnyPlaceHolder(Held); }
            /// @copydoc AnyPlaceHolder::WriteToStream(std::ostream&)
			virtual void WriteToStream(std::ostream& Out)
                { Out << Held; }
    };//AnyHolder

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Any
    /// @headerfile any.h
    /// @brief This is a simple class used for type-safe casting of any type of data.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB Any
    {
        protected:
            AnyPlaceHolder* Data;

            template <typename ValueType>
            friend ValueType* Any_Cast(Any*);
        public:
            /// @brief Class Constructor.
            Any() : Data(0) {}
            /// @brief Copy Constructor.
            /// @param Other The other Any to construct from.
            Any(const Any& Other)
              : Data(Other.Data ? Other.Data->Clone() : 0) {}
            /// @brief Template copy Constructor.
            /// @param Value The data to construct this Any with.
            template<typename ValueType>
            explicit Any(const ValueType& Value)
              : Data(new AnyHolder<ValueType>(Value)) {}
            /// @brief Class Destructor.
            ~Any() { Destroy(); }

            /// @brief Swaps the contents of this Any with another.
            /// @param Other The other Any to swap with.
            /// @return Returns a reference to this Any.
            Any& Swap(Any& Other)
            {
                std::swap(Data, Other.Data);
                return *this;
            }
            /// @brief Templated Assignment operator.
            /// @param Other The Value to copy into this Any.
            /// @return Returns a reference to this Any.
            template<typename ValueType>
            Any& operator=(const ValueType& Other)
            {
                Any(Other).Swap(*this);
                return *this;
            }
            /// @brief Assignment operator.
            /// @param Other The Any to copy into this Any.
            /// @return Returns a reference to this Any.
            Any& operator=(const Any& Other)
            {
                Any(Other).Swap(*this);
                return *this;
            }

            /// @brief Checks if this Any is storing data.
            /// @return Returns true if this Any is storing valid data, false otherwise.
            bool IsEmpty() const
            {
                return !Data;
            }
            /// @brief Gets the type-info of the stored data.
            /// @return Returns a type_info struct detailing the type of data stored.
            const std::type_info& GetType() const
            {
                return Data ? Data->GetType() : typeid(void);
            }
            /// @brief Destroy's the data contained in this Any and makes it empty.
            void Destroy()
            {
                delete Data;
                Data = NULL;
            }
            /// @brief Streaming operator.
            /// @param Out The output stream to stream to.
            /// @param Value The Any to be streamed.
            /// @return Returns a reference to the streamed stream.
            inline friend std::ostream& operator <<
                ( std::ostream& Out, const Any& Value )
            {
                if(Value.Data)
                    Value.Data->WriteToStream(Out);
                return Out;
            }
            /// @brief Parens operator.
            /// @return Returns the value being stored.
            template<typename ValueType>
            ValueType operator()() const
            {
                /// @todo These exceptions should get logged, but the Entresol class is too large for it to be worthwhile to include it here.
                if(!Data)
                {
                    throw Exception("Bad cast from uninitialized Any.  In Any::operator()");
                }
                else if(GetType() == typeid(ValueType))
                {
                    return static_cast< AnyHolder<ValueType>* >(Data)->Held;
                }
                else
                {
                    StringStream exceptionstream;
                    exceptionstream << "Bad cast from type '" << GetType().name() << "' to '" << typeid(ValueType).name() << "'.  In Any::operator()";
                    throw Exception(exceptionstream.str());
                }
            }
    };//Any

    /// @brief Cast from an Any to a value.
    /// @param Operand A pointer to the Any to cast from.
    /// @return Returns the casted value.
    template<typename ValueType>
    ValueType* MEZZ_LIB Any_Cast(Any* Operand)
    {
        return Operand && Operand->GetType() == typeid(ValueType)
                    ? &static_cast< AnyHolder<ValueType> *>(Operand->Data)->Held
                    : 0;
    }
    /// @brief Cast from an Any to a value.
    /// @param Operand A pointer to the Any to cast from.
    /// @return Returns the casted value.
    template<typename ValueType>
    const ValueType* MEZZ_LIB Any_Cast(const Any* Operand)
    {
        return Any_Cast<ValueType>(const_cast<Any*>(Operand));
    }
    /// @brief Cast from an Any to a value.
    /// @param Operand The Any to cast from.
    /// @return Returns the casted value.
    template<typename ValueType>
    ValueType MEZZ_LIB Any_Cast(const Any& Operand)
    {
        const ValueType* Result = Any_Cast<ValueType>(&Operand);
        if(!Result)
		{
			StringStream exceptionstream;
			exceptionstream << "Bad cast from type '" << Operand.GetType().name() << "' to '" << typeid(ValueType).name() << "'.  In Mezzanine::Any_Cast";
			throw Exception(exceptionstream.str());
		}
        return *Result;
    }
}//Mezzanine

#endif
