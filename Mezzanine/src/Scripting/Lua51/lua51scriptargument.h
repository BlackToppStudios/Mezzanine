// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
            const Integer LuaNil = GenericNull;

            /// @brief The ScriptArgumentGeneric<T> needs just a bit of Lua specific functionality so we add push/pop for the LuaScript to call.
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

                    /// @brief Virtual deconstructor
                    virtual ~Lua51IntegerArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51IntegerArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief A Real that can readily be passed into lua scripts
            class MEZZ_LIB Lua51RealArgument
                : public LuaArgument, public ScriptArgumentGeneric<Real>
            {
                public:
                    /// @brief Initializing constructor
                    /// @param InitialValue Deafaults to 0.0
                    Lua51RealArgument(Real InitialValue = 0.0)
                        : ScriptArgumentGeneric<Real>(InitialValue)
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    /// @brief Virtual deconstructor
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
                    /// @brief Initializing Constructor
                    /// @param InitialValue Defaults to 0.
                    Lua51WholeArgument(Whole InitialValue = 0)
                        : ScriptArgumentGeneric<Whole>(InitialValue)
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    /// @brief Virtual deconstructor
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

                    /// @brief Virtual deconstructor
                    virtual ~Lua51StringArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51StringArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief No special care is required for Bool Lua Arguments, so a simple typedef is
            /// used.
            class MEZZ_LIB Lua51BoolArgument
                : public LuaArgument, public ScriptArgumentGeneric<Boole>
            {
                public:
                    /// @brief Setting Constructor
                    /// @param InitialValue Defaults to false
                    Lua51BoolArgument(Boole InitialValue = false)
                        : ScriptArgumentGeneric<Boole>(InitialValue)
                    {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    /// @brief Virtual deconstructor
                    virtual ~Lua51BoolArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51BoolArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief Represents not much of anything but will insert and retrieves Nils from Lua51.
            class MEZZ_LIB Lua51NilArgument : public LuaArgument, public ScriptArgumentGeneric<NullArgument>
            {
                public:
                    Lua51NilArgument()
                        {}

                    virtual void Push(lua_State* TargetState) const;

                    virtual void Pop(lua_State* TargetState);

                    /// @brief Virtual deconstructor
                    virtual ~Lua51NilArgument() {}

                    /// @brief Get Nil as a String.
                    /// @return The string "Nil"
                    virtual String GetString() const
                        { return "Nil"; }

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51NilArgument* GetMostDerived()
                        { return this; }
            };

        } // Lua
    } // Scripting

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
