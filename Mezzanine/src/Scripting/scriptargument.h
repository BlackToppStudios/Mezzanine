//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _scriptargument_h
#define _scriptargument_h

/// @file
/// @brief This file has the interfaces for Script Arguments and the associated dependency chain

#include "datatypes.h"
#include "smartptr.h"

namespace Mezzanine
{
    namespace Scripting
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief The interface for a script argument
        /// @details All the members that all script for all languages must implement.
        /// @n @n
        /// These are created to provide data to scripts. This is intended to primarily
        /// transport primitive types into and out of scripts without needing to care
        /// about any underlying types. This limits what operations can be done with
        /// this, because it is intended to provide an abstraction that can be used
        /// to implement specific algorithms for languages that require them.
        /// @todo Define operators for the iScriptArgument at least for assignment
        ///////////////////////////////////////
        class MEZZ_LIB iScriptArgument
        {
            public:
                /// @brief Overidable Deconstructor
                virtual ~iScriptArgument()
                    {}

                /// @brief Get the Argument as a String
                /// @return The argument value lexographically converted as a @ref String
                virtual String GetString() = 0;

                /// @brief Get the Argument as a Whole
                /// @return The argument value lexographically converted as a @ref Whole
                virtual Whole GetWhole() = 0;

                /// @brief Get the Argument as a Integer
                /// @return The argument value lexographically converted as an @ref Integer
                virtual Integer GetInteger() = 0;

                /// @brief Get the Argument as a Real
                /// @return The argument value lexographically converted as an @ref Real
                virtual Real GetReal() = 0;

                /// @brief Get the Argument as a Bool
                /// @return The argument value lexographically converted as an @ref Bool
                virtual Bool GetBool() = 0;

        }; // iScriptArgument

        /// @brief A generic implementation of a ScriptArgument that is suitable for primitive types in most situations
        template <class T>
        class MEZZ_LIB ScriptArgumentSpecific : public iScriptArgument
        {
            private:
                /// @brief The actual data.
                /// @warning To prevent Slicing don't store the parent class in a container.
                /// This practically guarantees that derived class will have different sizes
                /// and overflow container boundaries.
                T Datum;

            public:
                /// @brief To make working with this easier.
                typedef T Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentSpecific(T InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentSpecific()
                    {}

                /// @brief Get the Argument as a String, slow default implementation.
                /// @return The argument value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Argument as a Whole, slow default implementation.
                /// @return The argument value lexographically converted as a @ref Whole
                virtual Whole GetWhole()
                    { return ToWhole(Datum); }

                /// @brief Get the Argument as a Integer, slow default implementation.
                /// @return The argument value lexographically converted as an @ref Integer
                virtual Integer GetInteger()
                    { return ToInteger(Datum); }

                /// @brief Get the Argument as a Real, slow default implementation.
                /// @return The argument value lexographically converted as an @ref Real
                virtual Real GetReal()
                    { return ToReal(Datum); }

                /// @brief Get the Argument as a Bool
                /// @return The argument value lexographically converted as an @ref Bool
                virtual Bool GetBool()
                    { return ToBool(Datum); }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(T NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operation can be performed on.
                virtual T GetValue()
                    { return Datum; }

        }; //ScriptArgumentSpecific

        /// @brief A Integer implementation of a ScriptArgument that is suitable for primitive types in most situations
        template <>
        class MEZZ_LIB ScriptArgumentSpecific<Integer> : public iScriptArgument
        {
            private:
                /// @brief The Integer actual data.
                Integer Datum;

            public:
                /// @brief To make working with this easier.
                typedef Integer Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Integer value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentSpecific(Integer InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentSpecific()
                    {}

                /// @brief Get the Integer as a String.
                /// @return The Integer value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Integer as a Whole.
                /// @return The Integer value lexographically converted as a @ref Whole
                virtual Whole GetWhole()
                    { return Whole(Datum); }

                /// @brief Get the Integer as a Integer.
                /// @return The Integer value lexographically converted as an @ref Integer
                virtual Integer GetInteger()
                    { return Datum; }

                /// @brief Get the Integer as a Real.
                /// @return The Integer value lexographically converted as an @ref Real
                virtual Real GetReal()
                    { return Real(Datum); }

                /// @brief Get the Integer as a Bool
                /// @return The argument value lexographically converted as an @ref Bool
                virtual Bool GetBool()
                    { return Bool(Datum); }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Integer NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Integer GetValue()
                    { return Datum; }
        }; //ScriptArgumentSpecific<Integer>

        /// @brief A Whole number implementation of a ScriptArgument that is suitable for primitive types in most situations
        template <>
        class MEZZ_LIB ScriptArgumentSpecific<Whole> : public iScriptArgument
        {
            private:
                /// @brief The Whole actual data.
                Whole Datum;

            public:
                /// @brief To make working with this easier.
                typedef Whole Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Whole value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentSpecific(Whole InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentSpecific()
                    {}

                /// @brief Get the Whole as a String
                /// @return The Whole value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Whole number.
                /// @return The Whole value lexographically converted as a @ref Whole
                virtual Whole GetWhole()
                    { return Datum; }

                /// @brief Get the Whole as a Integer.
                /// @return The Whole value lexographically converted as an @ref Integer
                virtual Integer GetInteger()
                    { return Integer(Datum); }

                /// @brief Get the Whole as a Real.
                /// @return The Whole value lexographically converted as an @ref Real
                virtual Real GetReal()
                    { return Real(Datum); }

                /// @brief Get the Whole Number as a Bool
                /// @return The argument value lexographically converted as an @ref Bool
                virtual Bool GetBool()
                    { return Bool(Datum); }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Whole NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operation can be performed on.
                virtual Whole GetValue()
                    { return Datum; }

        }; //ScriptArgumentSpecific<Whole>

        /// @brief A String implementation of a ScriptArgument that is suitable for primitive types in most situations
        template <>
        class MEZZ_LIB ScriptArgumentSpecific<String> : public iScriptArgument
        {
            private:
                /// @brief The String data.
                String Datum;

            public:
                /// @brief To make working with this easier.
                typedef String Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument String value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentSpecific(String InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentSpecific()
                    {}

                /// @brief Get the String
                /// @return The String
                virtual String GetString() const
                    { return Datum; }

                /// @brief Get the String as a Whole.
                /// @return The String value lexographically converted as a @ref Whole
                virtual Whole GetWhole()
                    { return ConvertTo<Whole>(Datum); }

                /// @brief Get the String as a Integer.
                /// @return The String value lexographically converted as an @ref Integer
                virtual Integer GetInteger()
                    { return ConvertTo<Integer>(Datum); }

                /// @brief Get the String as a Real.
                /// @return The String value lexographically converted as an @ref Real
                virtual Real GetReal()
                    { return ConvertTo<Real>(Datum); }

                /// @brief Get the String as a Bool
                /// @return The argument value lexographically converted as an @ref Bool
                virtual Bool GetBool()
                    { return ToBool(Datum); }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(String NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operation can be performed on.
                virtual String GetValue()
                    { return Datum; }

        }; //ScriptArgumentSpecific<String>

        /// @brief A Real number implementation of a ScriptArgument that is suitable for primitive types in most situations
        template <>
        class MEZZ_LIB ScriptArgumentSpecific<Real> : public iScriptArgument
        {
            private:
                /// @brief The Real actual data.
                Real Datum;

            public:
                /// @brief To make working with this easier.
                typedef Real Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Integer value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentSpecific(Real InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentSpecific()
                    {}

                /// @brief Get the Real as a String.
                /// @return The Real value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Real as a Whole.
                /// @return The Real value lexographically converted as a @ref Whole
                virtual Whole GetWhole()
                    { return Whole(Datum); }

                /// @brief Get the Real as a Integer.
                /// @return The Real value lexographically converted as an @ref Integer
                virtual Integer GetInteger()
                    { return Integer(Datum); }

                /// @brief Get the Real.
                /// @return The Real.
                virtual Real GetReal()
                    { return Datum; }

                /// @brief Get the Real as a Bool
                /// @return The Real value lexographically converted as an @ref Bool
                virtual Bool GetBool()
                    { return Bool(Datum); }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Real NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Real GetValue()
                    { return Datum; }
        }; //ScriptArgumentSpecific<Real>


        /// @brief A Bool implementation of a ScriptArgument that is suitable for primitive types in most situations
        template <>
        class MEZZ_LIB ScriptArgumentSpecific<Bool> : public iScriptArgument
        {
            private:
                /// @brief The Bool actual data.
                Bool Datum;

            public:
                /// @brief To make working with this easier.
                typedef Bool Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Integer value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentSpecific(Bool InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentSpecific()
                    {}

                /// @brief Get the Bool as a String.
                /// @return The Bool value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Bool as a Whole.
                /// @return The Bool value lexographically converted as a @ref Whole
                virtual Whole GetWhole()
                    { return Whole(Datum); }

                /// @brief Get the Bool as a Integer.
                /// @return The Bool value lexographically converted as an @ref Integer
                virtual Integer GetInteger()
                    { return Integer(Datum); }

                /// @brief Get the Bool as a Integer.
                /// @return The Bool value lexographically converted as an @ref Integer
                virtual Real GetReal()
                    { return Real(Datum); }

                /// @brief Get the Bool.
                /// @return The Bool.
                virtual Bool GetBool()
                    { return Datum; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Bool NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Bool GetValue()
                    { return Datum; }
        }; //ScriptArgumentSpecific<Real>

    }//Scripting
}//Mezzanine



#endif // \_scriptargument_h
