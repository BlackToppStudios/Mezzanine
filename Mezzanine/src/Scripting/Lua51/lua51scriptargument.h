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
#ifndef _lua51scriptargument_h
#define _lua51scriptargument_h

/// @file
/// @brief This file has the definition of the Script Arguments for Lua 51

#include "datatypes.h"

#ifdef MEZZLUA51

#include "Scripting/scriptargument.h"
#include "Scripting/script.h"
#include "Scripting/scriptingmanager.h"

/// @brief Forward declaration to enable pointer parameters
class lua_State;

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Lua
        {
            /// @brief Lua51IntegerArgument returns this value when checking GetTypeData() const.
            const Integer LuaInteger = GenericInteger;
            /// @brief Lua51RealArgument returns this value when checking GetTypeData() const.
            const Integer LuaReal = GenericReal;
            /// @brief Lua51WholeArgument returns this value when checking GetTypeData() const.
            const Integer LuaWhole = GenericWhole;
            /// @brief Lua51StringArgument returns this value when checking GetTypeData() const.
            const Integer LuaString = GenericString;
            /// @brief Lua51BoolArgument returns this value when checking GetTypeData() const.
            const Integer LuaBool = GenericBool;
            /// @brief Lua51NilArgument returns this value when checking GetTypeData() const.
            const Integer LuaNil = GenericMax+1;

            /// @brief The ScriptArgumentGeneric<T> does a good enough job for actually passing data, but it needs just a bit of Lua specific functionality
            class MEZZ_LIB LuaArgument
            {
                public:
                    /// @brief Handle the details of putting this data onto Lua's Stack.
                    /// @param TargetState The state with the stack to push the data onto.
                    /// @return Whatever the Lua return code of the first failing lua call, or the last successful call.
                    virtual void Push(lua_State* TargetState) const = 0;

                    /// @brief Handle the details of pulling data from Lua's Stack into this.
                    /// @param TargetState The state with the stack to pull the data from.
                    /// @return Whatever the Lua return code of the first failing lua call, or the last successful call.
                    virtual void Pop(lua_State* TargetState) = 0;

                    /// @brief Virtual deconstructor
                    virtual ~LuaArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual LuaArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief The implementations in the ScriptArgumentGeneric<Integer> will cover most of what this needs
            class MEZZ_LIB Lua51IntegerArgument : public LuaArgument, public ScriptArgumentGeneric<Integer>
            {
                public:

                    /// @brief Implicit constructor
                    /// @param InitialValue Defaults to 0 and is the actual data to pass into Lua
                    /// @note This is intentionally not explicit. This should make it easier to work with in situations where the creation of this is less important.
                    Lua51IntegerArgument(Integer InitialValue = 0) : ScriptArgumentGeneric<Integer>(InitialValue)
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    virtual ~Lua51IntegerArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51IntegerArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief A Real that can readily be passed into lua scripts
            class MEZZ_LIB Lua51RealArgument : public LuaArgument, public ScriptArgumentGeneric<Real>
            {
                public:
                    Lua51RealArgument(Real InitialValue = 0.0) : ScriptArgumentGeneric<Real>(InitialValue)
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    virtual ~Lua51RealArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51RealArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief No special care is required for Whole number Lua Arguments, so a simple typedef is used.
            class MEZZ_LIB Lua51WholeArgument : public LuaArgument, public ScriptArgumentGeneric<Whole>
            {
                public:
                    Lua51WholeArgument(Whole InitialValue = 0) : ScriptArgumentGeneric<Whole>(InitialValue)
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    virtual ~Lua51WholeArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51WholeArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief No special care is required for String Lua Arguments, so a simple typedef is used.
            class MEZZ_LIB Lua51StringArgument : public LuaArgument, public ScriptArgumentGeneric<String>
            {
                public:
                    Lua51StringArgument(String InitialValue = String()) : ScriptArgumentGeneric<String>(InitialValue)
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    virtual ~Lua51StringArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51StringArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief No special care is required for Bool Lua Arguments, so a simple typedef is used.
            class MEZZ_LIB Lua51BoolArgument : public LuaArgument, public ScriptArgumentGeneric<Bool>
            {
                public:
                    Lua51BoolArgument(Bool InitialValue = false) : ScriptArgumentGeneric<Bool>(InitialValue)
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    virtual ~Lua51BoolArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51BoolArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief A very simple class to allow a specialization of ScriptArgumentGeneric to correspond with Lua's Nil.
            class MEZZ_LIB Lua51Nil {};

        } // Lua

        /// @brief A Lua51Nil implementation of a ScriptArgument that is suitable for only for passing Nil into Lua51 scripts
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<Mezzanine::Scripting::Lua::Lua51Nil> : public iScriptArgument
        {
            protected:
                /// @brief For propriety here is an actual Lua51Nil, it does nothing and will likely be optimized out by any sane compiler
                Mezzanine::Scripting::Lua::Lua51Nil Datum;

            public:
                /// @brief To make working with this easier.
                typedef Mezzanine::Scripting::Lua::Lua51Nil Type;

                /// @brief Create a Nil Argument
                ScriptArgumentGeneric()
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get "Nil".
                /// @return A string containing "Nil"
                virtual String GetString() const
                    { return String("Nil"); }

                /// @brief Get Nil as a Whole
                /// @return 0 because NaN does not work in C++
                virtual Whole GetWhole() const
                    { return 0; }

                /// @brief Get 0.
                /// @return 0 because NaN does not work in C++
                virtual Integer GetInteger() const
                    { return 0; }

                /// @brief Get the Nil as a 0.0.
                /// @return 0.0 because NaN does not work in C++
                virtual Real GetReal() const
                    { return 0.0; }

                /// @brief Get the Nil as a bool
                /// @return false.
                virtual Bool GetBool() const
                    { return false; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Mezzanine::Scripting::Lua::Lua51Nil NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Mezzanine::Scripting::Lua::Lua51Nil GetValue() const
                    { return Datum; }

                /// @brief Get data about this being a LuaNil
                /// @return This will return an Integer containing LuaNil
                virtual Integer GetTypeData() const
                    { return Mezzanine::Scripting::Lua::LuaNil; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<Mezzanine::Scripting::Lua::Lua51Nil>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentSpecific<Lua51Nil>

        namespace Lua
        {
            /// @brief Represents not much of anything but will insert and retrieves Nils from Lua51.
            class MEZZ_LIB Lua51NilArgument : public LuaArgument, public ScriptArgumentGeneric<Lua51Nil>
            {
                public:
                    Lua51NilArgument() : ScriptArgumentGeneric<Lua51Nil>()
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    virtual ~Lua51NilArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51NilArgument* GetMostDerived()
                        { return this; }
            };

        } // Lua
    } // Scripting

    //

    /// @brief Marks LuaArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Lua::LuaArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for LuaArgument with be LuaArgument itself
            typedef Scripting::Lua::LuaArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Lua::LuaArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks Lua51IntegerArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Lua::Lua51IntegerArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for Lua51IntegerArgument with be Lua51IntegerArgument itself
            typedef Scripting::Lua::Lua51IntegerArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Lua::Lua51IntegerArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks Lua51RealArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Lua::Lua51RealArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for Lua51RealArgument with be Lua51RealArgument itself
            typedef Scripting::Lua::Lua51RealArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Lua::Lua51RealArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks Lua51WholeArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Lua::Lua51WholeArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for Lua51WholeArgument with be Lua51WholeArgument itself
            typedef Scripting::Lua::Lua51WholeArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Lua::Lua51WholeArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks Lua51StringArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Lua::Lua51StringArgument>
    {
        public:
            /// @brief The type that Lua51StringArgument the Reference count for Lua51StringArgument with be Lua51StringArgument itself
            typedef Scripting::Lua::Lua51StringArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Lua::Lua51StringArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks Lua51BoolArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Lua::Lua51BoolArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for Lua51BoolArgument with be Lua51BoolArgument itself
            typedef Scripting::Lua::Lua51BoolArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Lua::Lua51BoolArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks Lua51NilArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Lua::Lua51NilArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for Lua51NilArgument with be Lua51NilArgument itself
            typedef Scripting::Lua::Lua51NilArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Lua::Lua51NilArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };



} // Mezzanine


#endif // MEZZLUA51
#endif // \_scriptinglua_h
