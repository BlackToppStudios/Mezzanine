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
#ifndef _rubyscriptargument_h
#define _rubyscriptargument_h

/// @file
/// @brief This file has the definition of the Script Arguments for Ruby

#include "datatypes.h"

#ifdef MEZZRUBY

#include "Scripting/scriptargument.h"
#include "Scripting/script.h"
#include "Scripting/scriptingmanager.h"

/// @brief Forward declaration to enable pointer parameters
class ruby_State;

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Ruby
        {
            /// @brief RubyIntegerArgument returns this value when checking GetTypeData() const.
            const Integer RubyInteger = GenericInteger;
            /// @brief RubyRealArgument returns this value when checking GetTypeData() const.
            const Integer RubyReal = GenericReal;
            /// @brief RubyWholeArgument returns this value when checking GetTypeData() const.
            const Integer RubyWhole = GenericWhole;
            /// @brief RubyStringArgument returns this value when checking GetTypeData() const.
            const Integer RubyString = GenericString;
            /// @brief RubyBoolArgument returns this value when checking GetTypeData() const.
            const Integer RubyBool = GenericBool;
            /// @brief RubyNilArgument returns this value when checking GetTypeData() const.
            const Integer RubyNil = GenericMax+1;

            /// @brief The ScriptArgumentGeneric<T> does a good enough job for actually passing data, but it needs just a bit of Ruby specific functionality
            class MEZZ_LIB RubyArgument
            {
                public:
                    /// @brief Handle the details of putting this data onto Ruby's Stack.
                    /// @param TargetState The state with the stack to push the data onto.
                    /// @return Whatever the Ruby return code of the first failing ruby call, or the last successful call.
                    virtual void Push(ruby_State* TargetState) const = 0;

                    /// @brief Handle the details of pulling data from Ruby's Stack into this.
                    /// @param TargetState The state with the stack to pull the data from.
                    /// @return Whatever the Ruby return code of the first failing ruby call, or the last successful call.
                    virtual void Pop(ruby_State* TargetState) = 0;

                    /// @brief Virtual deconstructor
                    virtual ~RubyArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief The implementations in the ScriptArgumentGeneric<Integer> will cover most of what this needs
            class MEZZ_LIB RubyIntegerArgument : public RubyArgument, public ScriptArgumentGeneric<Integer>
            {
                public:

                    /// @brief Implicit constructor
                    /// @param InitialValue Defaults to 0 and is the actual data to pass into Ruby
                    /// @note This is intentionally not explicit. This should make it easier to work with in situations where the creation of this is less important.
                    RubyIntegerArgument(Integer InitialValue = 0) : ScriptArgumentGeneric<Integer>(InitialValue)
                    {}

                    virtual void Push(ruby_State* TargetState) const;

                    virtual void Pop(ruby_State* TargetState);

                    virtual ~RubyIntegerArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyIntegerArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief A Real that can readily be passed into ruby scripts
            class MEZZ_LIB RubyRealArgument : public RubyArgument, public ScriptArgumentGeneric<Real>
            {
                public:
                    RubyRealArgument(Real InitialValue = 0.0) : ScriptArgumentGeneric<Real>(InitialValue)
                    {}

                    virtual void Push(ruby_State* TargetState) const;

                    virtual void Pop(ruby_State* TargetState);

                    virtual ~RubyRealArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyRealArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief No special care is required for Whole number Ruby Arguments, so a simple typedef is used.
            class MEZZ_LIB RubyWholeArgument : public RubyArgument, public ScriptArgumentGeneric<Whole>
            {
                public:
                    RubyWholeArgument(Whole InitialValue = 0) : ScriptArgumentGeneric<Whole>(InitialValue)
                    {}

                    virtual void Push(ruby_State* TargetState) const;

                    virtual void Pop(ruby_State* TargetState);

                    virtual ~RubyWholeArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyWholeArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief No special care is required for String Ruby Arguments, so a simple typedef is used.
            class MEZZ_LIB RubyStringArgument : public RubyArgument, public ScriptArgumentGeneric<String>
            {
                public:
                    RubyStringArgument(String InitialValue = String()) : ScriptArgumentGeneric<String>(InitialValue)
                    {}

                    virtual void Push(ruby_State* TargetState) const;

                    virtual void Pop(ruby_State* TargetState);

                    virtual ~RubyStringArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyStringArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief No special care is required for Bool Ruby Arguments, so a simple typedef is used.
            class MEZZ_LIB RubyBoolArgument : public RubyArgument, public ScriptArgumentGeneric<Bool>
            {
                public:
                    RubyBoolArgument(Bool InitialValue = false) : ScriptArgumentGeneric<Bool>(InitialValue)
                    {}

                    virtual void Push(ruby_State* TargetState) const;

                    virtual void Pop(ruby_State* TargetState);

                    virtual ~RubyBoolArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyBoolArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief A very simple class to allow a specialization of ScriptArgumentGeneric to correspond with Ruby's Nil.
            class MEZZ_LIB RubyNil {};

        } // Ruby

        /// @brief A RubyNil implementation of a ScriptArgument that is suitable for only for passing Nil into Ruby scripts
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<Mezzanine::Scripting::Ruby::RubyNil> : public iScriptArgument
        {
            protected:
                /// @brief For propriety here is an actual RubyNil, it does nothing and will likely be optimized out by any sane compiler
                Mezzanine::Scripting::Ruby::RubyNil Datum;

            public:
                /// @brief To make working with this easier.
                typedef Mezzanine::Scripting::Ruby::RubyNil Type;

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
                virtual void SetValue(Mezzanine::Scripting::Ruby::RubyNil NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Mezzanine::Scripting::Ruby::RubyNil GetValue() const
                    { return Datum; }

                /// @brief Get data about this being a RubyNil
                /// @return This will return an Integer containing RubyNil
                virtual Integer GetTypeData() const
                    { return Mezzanine::Scripting::Ruby::RubyNil; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer of the most derived pointing to this.
                virtual ScriptArgumentGeneric<Mezzanine::Scripting::Ruby::RubyNil>* GetMostDerived()
                    { return this; }
        }; //ScriptArgumentSpecific<RubyNil>

        namespace Ruby
        {
            /// @brief Represents not much of anything but will insert and retrieves Nils from Ruby.
            class MEZZ_LIB RubyNilArgument : public RubyArgument, public ScriptArgumentGeneric<RubyNil>
            {
                public:
                    RubyNilArgument() : ScriptArgumentGeneric<RubyNil>()
                    {}

                    virtual void Push(ruby_State* TargetState) const;

                    virtual void Pop(ruby_State* TargetState);

                    virtual ~RubyNilArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyNilArgument* GetMostDerived()
                        { return this; }
            };

        } // Ruby
    } // Scripting

    //

    /// @brief Marks RubyArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Ruby::RubyArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for RubyArgument with be RubyArgument itself
            typedef Scripting::Ruby::RubyArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::RubyArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks RubyIntegerArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Ruby::RubyIntegerArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for RubyIntegerArgument with be RubyIntegerArgument itself
            typedef Scripting::Ruby::RubyIntegerArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::RubyIntegerArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks RubyRealArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Ruby::RubyRealArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for RubyRealArgument with be RubyRealArgument itself
            typedef Scripting::Ruby::RubyRealArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::RubyRealArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks RubyWholeArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Ruby::RubyWholeArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for RubyWholeArgument with be RubyWholeArgument itself
            typedef Scripting::Ruby::RubyWholeArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::RubyWholeArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks RubyStringArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Ruby::RubyStringArgument>
    {
        public:
            /// @brief The type that RubyStringArgument the Reference count for RubyStringArgument with be RubyStringArgument itself
            typedef Scripting::Ruby::RubyStringArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::RubyStringArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks RubyBoolArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Ruby::RubyBoolArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for RubyBoolArgument with be RubyBoolArgument itself
            typedef Scripting::Ruby::RubyBoolArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::RubyBoolArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };

    /// @brief Marks RubyNilArgument for internal reference counting if a CountedPtr checks
    template <>
    class MEZZ_LIB ReferenceCountTraits <Scripting::Ruby::RubyNilArgument>
    {
        public:
            /// @brief The type that maintains the Reference count for RubyNilArgument with be RubyNilArgument itself
            typedef Scripting::Ruby::RubyNilArgument RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::RubyNilArgument that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };



} // Mezzanine


#endif // MEZZRUBY
#endif // \_scriptingruby_h
