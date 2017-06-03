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
#ifndef _scriptargument_h
#define _scriptargument_h

/// @file
/// @brief This file has the interfaces for Script Arguments and the associated dependency chain

#include "datatypes.h"
#include "countedptr.h"

namespace Mezzanine
{
    namespace Scripting
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief The interface for a script argument
        /// @details These are all the members that all script for all languages must
        /// implement. This mandatory functionlity insures that once portion of C++ code
        /// has access to a script argument it can always access the data contained.
        /// @n @n
        /// These are created to provide data to scripts. This is intended to primarily
        /// to act as an abstraction layer between game data and data inside scripts,
        /// but currently just transports primitives into and out of scripts without
        /// needing to care about any underlying types. This limits what operations
        /// can be done with, because it is intended to provide an abstraction that
        /// can be used to implement specific algorithms for languages that require
        /// them.
        /// @n @n
        /// For example Lua requires calling different functions for passing Strings
        /// Numbers, Nils and other types, while Ruby does not require this. These
        /// classes will always present Data to C++ class the same way, but will take
        /// whatever steps required for the scripting language and optionally the
        /// types they represent.
        ///////////////////////////////////////
        class MEZZ_LIB iScriptArgument : public IntrusiveRefCount
        {
            public:
                /// @brief Overidable Deconstructor
                virtual ~iScriptArgument()
                    {}

                /// @brief Get the Argument as a String
                /// @return The argument value lexographically converted as a @ref String
                virtual String GetString() const = 0;

                /// @brief Get the Argument as a Whole
                /// @return The argument value lexographically converted as a @ref Whole
                virtual Whole GetWhole() const = 0;

                /// @brief Get the Argument as a Integer
                /// @return The argument value lexographically converted as an @ref Integer
                virtual Integer GetInteger() const = 0;

                /// @brief Get the Argument as a Real
                /// @return The argument value lexographically converted as an @ref Real
                virtual Real GetReal() const = 0;

                /// @brief Get the Argument as a Boole
                /// @return The argument value lexographically converted as an @ref Boole
                virtual Boole GetBoole() const = 0;

                /// @brief Is this value representing a Null/Nil value.
                /// @return True if ths argument is null, false if it is anything other Type. This may of may not for language specific semantics.
                virtual Boole IsNull() const = 0;

                /// @brief Get data about the underlying specific to the scripting system using the argument.
                /// @return An Integer containing scripting langauge specific data.
                virtual Integer GetTypeData() const = 0;

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual iScriptArgument* GetMostDerived()
                    { return this; }
        }; // iScriptArgument

        /// @brief A listing of the Base types an iScriptArgument could represent.
        enum ScriptArgument
        {
            GenericUnknown      = 0,    ///< This is not readily available when instantiating ScriptArgumentGeneric<T> where T is unknown
            GenericInteger      = 1,    ///< Returned from GetTypeData() const when a ScriptArgumentGeneric is specialized as an Integer
            GenericWhole        = 2,    ///< Returned from GetTypeData() const when a ScriptArgumentGeneric is specialized as an Whole
            GenericString       = 3,    ///< Returned from GetTypeData() const when a ScriptArgumentGeneric is specialized as an String
            GenericReal         = 4,    ///< Returned from GetTypeData() const when a ScriptArgumentGeneric is specialized as an Real
            GenericBool         = 5,    ///< Returned from GetTypeData() const when a ScriptArgumentGeneric is specialized as an Bool
            GenericNull         = 6,    ///< Returned from GetTypeData() const when a ScriptArgumentGeneric is intended as NullPtr, SQL Null, Lua Nil, Ruby nil, etc...
            GenericMax          = 6     ///< This is always equal to the maximum value in this enumeration so that other constant values can be based on this. For example Lua uses this +1 for its NilValue
        };

        /// @brief A generic implementation of a ScriptArgument that is suitable for primitive types in most situations.
        /// @details Scripting languages the do not benefit from having different implementations per type should
        /// implement their argument subclass derived from this. This will attempt to provide basic type conversion
        /// for Datum to C++ types lexigraphically. Any that is innapropriate or suboptimal should be overidden
        template <class T>
        class MEZZ_LIB ScriptArgumentGeneric : public iScriptArgument
        {
            protected:
                /// @brief The actual data.
                /// @warning To prevent slicing don't store the parent class in a container.
                /// Dpractically guarantees that derived class will have different sizes
                /// and overflow container boundaries. If you must store shared pointers in
                /// constainers
                T Datum;

            public:
                /// @brief To make working with this easier.
                typedef T Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentGeneric(T InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get the Argument as a String, slow default implementation.
                /// @return The argument value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Argument as a Whole, slow default implementation.
                /// @return The argument value lexographically converted as a @ref Whole
                virtual Whole GetWhole() const
                    { return ToWhole(Datum); }

                /// @brief Get the Argument as a Integer, slow default implementation.
                /// @return The argument value lexographically converted as an @ref Integer
                virtual Integer GetInteger() const
                    { return ToInteger(Datum); }

                /// @brief Get the Argument as a Real, slow default implementation.
                /// @return The argument value lexographically converted as an @ref Real
                virtual Real GetReal() const
                    { return ToReal(Datum); }

                /// @brief Get the Argument as a Boole
                /// @return The argument value lexographically converted as an @ref Boole
                virtual Boole GetBoole() const
                    { return ToBool(Datum); }

                /// @brief Is this value representing a Null/Nil value.
                /// @return This will return true if the string representation of this is empty and the Integer representation equals 0, this could and should be overridden on derived scripting arguments
                virtual Boole IsNull() const
                    { return GetString()=="" && GetInteger()==0; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(T NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operation can be performed on.
                virtual T GetValue() const
                    { return Datum; }

                /// @brief Get data about this having an unknown type
                /// @return This will return an Integer containing GenericUnknown. If needed proper specialization of this can be made for any class, see the ScriptArgumentGeneric<Mezzanine::Scripting::Lua::Lua51Nil>
                virtual Integer GetTypeData() const
                    { return GenericUnknown; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<T>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentGeneric<T>

        /// @brief A Integer implementation of a ScriptArgument that is suitable for primitive types in most situations
        /// @details Scripting Languages that require algorithms specific to Integers should add required functionality
        /// to a type derived from this.
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<Integer> : public iScriptArgument
        {
            protected:
                /// @brief The Integer actual data.
                Integer Datum;

            public:
                /// @brief To make working with this easier.
                typedef Integer Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Integer value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentGeneric(Integer InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get the Integer as a String.
                /// @return The Integer value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Integer as a Whole.
                /// @return The Integer value lexographically converted as a @ref Whole
                virtual Whole GetWhole() const
                    { return Whole(Datum); }

                /// @brief Get the Integer as a Integer.
                /// @return The Integer value lexographically converted as an @ref Integer
                virtual Integer GetInteger() const
                    { return Datum; }

                /// @brief Get the Integer as a Real.
                /// @return The Integer value lexographically converted as an @ref Real
                virtual Real GetReal() const
                    { return Real(Datum); }

                /// @brief Get the Integer as a Boole
                /// @return The argument value lexographically converted as an @ref Boole
                virtual Boole GetBoole() const
                    { return Boole(Datum); }

                /// @brief Is this value representing a Null/Nil value.
                /// @return In most languages 0 is a valid value for Integers, so any value in this is assumed to not be NULL. This should be overridden if different behavior is required.
                virtual Boole IsNull() const
                    { return false; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Integer NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Integer GetValue() const
                    { return Datum; }

                /// @brief Get data about this being and Integer
                /// @return This will return an Integer containing GenericInteger.
                virtual Integer GetTypeData() const
                    { return GenericInteger; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<Integer>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentGeneric<Integer>

        /// @brief A Whole number implementation of a ScriptArgument that is suitable for primitive types in most situations
        /// @details Scripting Languages that require algorithms specific to Whole number should add required functionality
        /// to a type derived from this.
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<Whole> : public iScriptArgument
        {
            protected:
                /// @brief The Whole actual data.
                Whole Datum;

            public:
                /// @brief To make working with this easier.
                typedef Whole Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Whole value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentGeneric(Whole InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get the Whole as a String
                /// @return The Whole value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Whole number.
                /// @return The Whole value lexographically converted as a @ref Whole
                virtual Whole GetWhole() const
                    { return Datum; }

                /// @brief Get the Whole as a Integer.
                /// @return The Whole value lexographically converted as an @ref Integer
                virtual Integer GetInteger() const
                    { return Integer(Datum); }

                /// @brief Get the Whole as a Real.
                /// @return The Whole value lexographically converted as an @ref Real
                virtual Real GetReal() const
                    { return Real(Datum); }

                /// @brief Get the Whole Number as a Boole
                /// @return The argument value lexographically converted as an @ref Boole
                virtual Boole GetBoole() const
                    { return Boole(Datum); }

                /// @brief Is this value representing a Null/Nil value.
                /// @return In most languages 0 is a valid value for Whole Numbers, so any value in this is assumedto not be NULL. This should be overridden if different behavior is required.
                virtual Boole IsNull() const
                    { return false; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Whole NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operation can be performed on.
                virtual Whole GetValue() const
                    { return Datum; }

                /// @brief Get data about this being a Whol
                /// @return This will return an Integer containing GenericWhole.
                virtual Integer GetTypeData() const
                    { return GenericWhole; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<Whole>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentGeneric<Whole>

        /// @brief A String implementation of a ScriptArgument that is suitable for primitive types in most situations
        /// @details Scripting Languages that require algorithms specific to strings should add required functionality
        /// to a type derived from this.
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<String> : public iScriptArgument
        {
            protected:
                /// @brief The String data.
                String Datum;

            public:
                /// @brief To make working with this easier.
                typedef String Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument String value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentGeneric(String InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get the String
                /// @return The String
                virtual String GetString() const
                    { return Datum; }

                /// @brief Get the String as a Whole.
                /// @return The String value lexographically converted as a @ref Whole
                virtual Whole GetWhole() const
                    { return ConvertTo<Whole>(Datum); }

                /// @brief Get the String as a Integer.
                /// @return The String value lexographically converted as an @ref Integer
                virtual Integer GetInteger() const
                    { return ConvertTo<Integer>(Datum); }

                /// @brief Get the String as a Real.
                /// @return The String value lexographically converted as an @ref Real
                virtual Real GetReal() const
                    { return ConvertTo<Real>(Datum); }

                /// @brief Get the String as a Boole
                /// @return The argument value lexographically converted as an @ref Boole
                virtual Boole GetBoole() const
                    { return ToBool(Datum); }

                /// @brief Is this value representing a Null/Nil value.
                /// @return In most languages "" is a valid value for Strings, so any value in this is assumed to not be NULL. This should be overridden if different behavior is required.
                virtual Boole IsNull() const
                    { return false; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(String NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operation can be performed on.
                virtual String GetValue() const
                    { return Datum; }

                /// @brief Get data about this being a String
                /// @return This will return an Integer containing GenericString.
                virtual Integer GetTypeData() const
                    { return GenericString; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<String>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentGeneric<String>

        /// @brief A Real number implementation of a ScriptArgument that is suitable for primitive types in most situations
        /// @details Scripting Languages that require algorithms specific to Real Numbers should add required functionality
        /// to a type derived from this.
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<Real> : public iScriptArgument
        {
            protected:
                /// @brief The Real actual data.
                Real Datum;

            public:
                /// @brief To make working with this easier.
                typedef Real Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Integer value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentGeneric(Real InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get the Real as a String.
                /// @return The Real value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Real as a Whole.
                /// @return The Real value lexographically converted as a @ref Whole
                virtual Whole GetWhole() const
                    { return Whole(Datum); }

                /// @brief Get the Real as a Integer.
                /// @return The Real value lexographically converted as an @ref Integer
                virtual Integer GetInteger() const
                    { return Integer(Datum); }

                /// @brief Get the Real.
                /// @return The Real.
                virtual Real GetReal() const
                    { return Datum; }

                /// @brief Get the Real as a Boole
                /// @return The Real value lexographically converted as an @ref Boole
                virtual Boole GetBoole() const
                    { return Boole(Datum); }

                /// @brief Is this value representing a Null/Nil value.
                /// @return In most languages 0.0 is a valid value for Real Numbers, so any value in this is assumed to not be NULL. This should be overridden if different behavior is required.
                virtual Boole IsNull() const
                    { return false; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Real NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Real GetValue() const
                    { return Datum; }

                /// @brief Get data about this being a Real number.
                /// @return This will return an Integer containing GenericReal.
                virtual Integer GetTypeData() const
                    { return GenericReal; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<Real>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentGeneric<Real>

        /// @brief A Bool implementation of a ScriptArgument that is suitable for primitive types in most situations
        /// @details Scripting Languages that require algorithms specific to boolean values should add required functionality
        /// to a type derived from this.
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<Boole> : public iScriptArgument
        {
            protected:
                /// @brief The Bool actual data.
                Boole Datum;

            public:
                /// @brief To make working with this easier.
                typedef Boole Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Integer value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentGeneric(Boole InitialValue) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get the Bool as a String.
                /// @return The Bool value lexographically converted as a @ref String
                virtual String GetString() const
                    { return ToString(Datum); }

                /// @brief Get the Bool as a Whole.
                /// @return The Bool value lexographically converted as a @ref Whole
                virtual Whole GetWhole() const
                    { return Whole(Datum); }

                /// @brief Get the Bool as a Integer.
                /// @return The Bool value lexographically converted as an @ref Integer
                virtual Integer GetInteger() const
                    { return Integer(Datum); }

                /// @brief Get the Bool as a Integer.
                /// @return The Bool value lexographically converted as an @ref Integer
                virtual Real GetReal() const
                    { return Real(Datum); }

                /// @brief Get the Bool.
                /// @return The Bool.
                virtual Boole GetBoole() const
                    { return Datum; }

                /// @brief Is this value representing a Null/Nil value.
                /// @return In most languages false is a valid value for Bools, so any value in this is assumed to not be NULL. This should be overridden if different behavior is required.
                virtual Boole IsNull() const
                    { return false; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Boole NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Boole GetValue() const
                    { return Datum; }

                /// @brief Get data about this being a Bool
                /// @return This will return an Integer containing GenericBool.
                virtual Integer GetTypeData() const
                    { return GenericBool; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<Boole>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentGeneric<Bool>

        /// @brief A very simple type used to indicate to script argument templates that the argument represents
        class NullArgument {};

        /// @brief A Null implementation of a ScriptArgument that is suitable for primitive types in most situations
        /// @details This will likely need to be overidden to get the string varation correct.
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<NullArgument> : public iScriptArgument
        {
            protected:
                /// @brief The Bool actual data.
                NullArgument Datum;

            public:
                /// @brief To make working with this easier.
                typedef NullArgument Type;

                /// @brief Create an initialized Argument
                /// @param InitialValue The value to initialize the Argument Integer value with.
                /// @note Intentionally not explicit, this allow for passing convertable types directly to functions.
                ScriptArgumentGeneric(NullArgument InitialValue = NullArgument()) :
                    Datum(InitialValue)
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get Null as a String.
                /// @return The Bool value lexographically converted as a @ref String
                virtual String GetString() const
                    { return "Null"; }

                /// @brief Get Null as a Whole.
                /// @return The Bool value lexographically converted as a @ref Whole
                virtual Whole GetWhole() const
                    { return 0; }

                /// @brief Get Null as a Integer.
                /// @return The Bool value lexographically converted as an @ref Integer
                virtual Integer GetInteger() const
                    { return 0; }

                /// @brief Get Null as a Real.
                /// @return The Bool value lexographically converted as an @ref Integer
                virtual Real GetReal() const
                    { return 0.0; }

                /// @brief Get as a Bool.
                /// @return False.
                virtual Boole GetBoole() const
                    { return false; }

                /// @brief Is this value representing a Null/Nil value.
                /// @return In most languages false is a valid value for Bools, so any value in this is assumed to not be NULL. This should be overridden if different behavior is required.
                virtual Boole IsNull() const
                    { return true; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(NullArgument NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual NullArgument GetValue() const
                    { return Datum; }

                /// @brief Get data about this being a Bool
                /// @return This will return an Integer containing GenericBool.
                virtual Integer GetTypeData() const
                    { return GenericNull; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<NullArgument>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentGeneric<Bool>

    }//Scripting

    /// @brief Marks iScriptArgument for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::iScriptArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for iScriptArgument with be iScriptArgument itself
            typedef Scripting::iScriptArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::iScriptArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks ScriptArgumentGeneric<Integer> for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::ScriptArgumentGeneric<Integer> >
    {
        public:
            /// @brief The type that maintains the Reference count for ScriptArgumentGeneric<Integer> with be ScriptArgumentGeneric<Integer> itself
            typedef Scripting::ScriptArgumentGeneric<Integer> RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::ScriptArgumentGeneric<Integer> that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks ScriptArgumentGeneric<Whole> for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::ScriptArgumentGeneric<Whole> >
    {
        public:
            /// @brief The type that maintains the Reference count for ScriptArgumentGeneric<Whole> with be ScriptArgumentGeneric<Whole> itself
            typedef Scripting::ScriptArgumentGeneric<Whole> RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::ScriptArgumentGeneric<Whole> that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks ScriptArgumentGeneric<String> for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::ScriptArgumentGeneric<String> >
    {
        public:
            /// @brief The type that maintains the Reference count for ScriptArgumentGeneric<String> with be ScriptArgumentGeneric<String> itself
            typedef Scripting::ScriptArgumentGeneric<String> RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::ScriptArgumentGeneric<String> that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks ScriptArgumentGeneric<Real> for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::ScriptArgumentGeneric<Real> >
    {
        public:
            /// @brief The type that maintains the Reference count for ScriptArgumentGeneric<Real> with be ScriptArgumentGeneric<Real> itself
            typedef Scripting::ScriptArgumentGeneric<Real> RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::ScriptArgumentGeneric<Real> that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks ScriptArgumentGeneric<Real> for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::ScriptArgumentGeneric<Boole> >
    {
        public:
            /// @brief The type that maintains the Reference count for ScriptArgumentGeneric<Bool> with be ScriptArgumentGeneric<Bool> itself
            typedef Scripting::ScriptArgumentGeneric<Boole> RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::ScriptArgumentGeneric<Bool> that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks ScriptArgumentGeneric<Real> for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::ScriptArgumentGeneric<Scripting::NullArgument> >
    {
        public:
            /// @brief The type that maintains the Reference count for ScriptArgumentGeneric<Scripting::NullArgument> with be ScriptArgumentGeneric<Scripting::NullArgument> itself
            typedef Scripting::ScriptArgumentGeneric<Scripting::NullArgument> RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::ScriptArgumentGeneric<Scripting::NullArgument> that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };


}//Mezzanine



#endif // \_scriptargument_h
