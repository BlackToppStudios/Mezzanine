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
            const Integer RubyNil = GenericNull;

            /// @brief A base type that all Ruby Arguments with use.
            /// @details Ruby does of its type assessment at runtime, so it makes little senses to heavily invest in a typing system.
            /// The type are for C++ development convenience, but Ruby only cares about its own pointers, since convertible pointer
            /// can store any native we can use it with Ruby's VALUE type. This still allows to create implicitly cast argument types
            /// for use with the Ruby Scripting system.
            typedef ScriptArgumentGeneric<ConvertiblePointer> RubyArgumentBase;

            /// @brief Ruby stores its values a in a very specific way so this must completely overloaded to make it work
            class MEZZ_LIB RubyArgument : public RubyArgumentBase
            {
                public:

                    RubyArgument(ConvertiblePointer InitialValue=0) : RubyArgumentBase(InitialValue)
                        {}

                    /// @brief Virtual deconstructor
                    virtual ~RubyArgument()
                        {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyArgument* GetMostDerived()
                        { return this; }

                    /// @brief Get the Argument as a String, slow default implementation.
                    /// @return
                    virtual String GetString() const;

                    /// @brief Get the Argument as a Whole, slow default implementation.
                    /// @return
                    virtual Whole GetWhole() const;

                    /// @brief Get the Argument as a Integer, slow default implementation.
                    /// @return
                    virtual Integer GetInteger() const;

                    /// @brief Get the Argument as a Real, slow default implementation.
                    /// @return
                    virtual Real GetReal() const;

                    /// @brief Get the Argument as a Bool
                    /// @return
                    virtual Boole GetBool() const;

                    /// @brief Provide an overload point to change assignment that operators will use.
                    /// @param
                    virtual void SetValue(ConvertiblePointer NewValue);

                    /// @brief Get the raw primitive to value.
                    /// @return
                    virtual ConvertiblePointer GetValue() const;
            };

            /// @brief
            class MEZZ_LIB RubyIntegerArgument : public RubyArgument
            {
                public:

                    /// @brief Implicit constructor
                    /// @param InitialValue Defaults to 0 and is the actual data to pass into Ruby
                    /// @note This is intentionally not explicit. This should make it easier to work with in situations where the creation of this is less important.
                    RubyIntegerArgument(Integer InitialValue = 0);

                    /// @brief Virtual deconstructor
                    virtual ~RubyIntegerArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyIntegerArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief 
            class MEZZ_LIB RubyRealArgument : public RubyArgument
            {
                public:
                    RubyRealArgument(Real InitialValue = 0.0);

                    /// @brief Virtual deconstructor
                    virtual ~RubyRealArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyRealArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief
            class MEZZ_LIB RubyWholeArgument : public RubyArgument
            {
                public:
                    RubyWholeArgument(Whole InitialValue = 0);

                    /// @brief Virtual deconstructor
                    virtual ~RubyWholeArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyWholeArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief
            class MEZZ_LIB RubyStringArgument : public RubyArgument
            {
                public:
                    RubyStringArgument(String InitialValue = String());

                    /// @brief Virtual deconstructor
                    virtual ~RubyStringArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyStringArgument* GetMostDerived()
                        { return this; }
            };

            /// @brief
            class MEZZ_LIB RubyBoolArgument : public RubyArgument
            {
                public:
                    RubyBoolArgument(Boole InitialValue = false);

                    /// @brief Virtual deconstructor
                    virtual ~RubyBoolArgument() {}

                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyBoolArgument* GetMostDerived()
                        { return this; }
            };

            typedef NullArgument NilArgument;

            /// @brief
            class MEZZ_LIB RubyNilArgument : public RubyArgument
            {
                public:
                    RubyNilArgument(NilArgument InitialValue = NilArgument());

                    /// @brief Virtual deconstructor
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
