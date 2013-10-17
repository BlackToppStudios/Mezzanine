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
#ifndef _Rubyscript_h
#define _Rubyscript_h

#include "datatypes.h"

#ifdef MEZZRUBY

#include "Scripting/scriptargument.h"
#include "Scripting/script.h"
#include "Scripting/scriptingmanager.h"

/// @file
/// @brief This file has the interface for the Ruby scripts.


namespace Mezzanine
{
    namespace Scripting
    {
        namespace Ruby
        {
            class RubyScriptingEngine;
            class RubyIntegerArgument;
            class RubyRealArgument;
            class RubyWholeArgument;
            class RubyStringArgument;
            class RubyBoolArgument;
            class RubyNilArgument;


            ///////////////////////////////////////////////////////////////////////////////////////
            /// @brief This class is used to store a Ruby script and in its source and compiled state.
            /// @details To execute this script use @ref RubyScriptingEngine::compile
            class MEZZ_LIB RubyScript : public virtual Mezzanine::Scripting::iScript
            {
                private:
                    /// @brief Makes passing internal data much easier and all Ruby are logically encapsulated as a single system still.
                    friend class RubyScriptingEngine;

                    /// @brief A set of the arguments being passed into the Ruby script
                    ArgumentGroup Args;

                    /// @brief A set of all the values the Ruby script returned the last time it was executed.
                    ArgumentGroup Returns;

                    /// @brief This will contain the source of the script
                    String SourceCode;

                    /// @brief Stores the Name of the Ruby Script
                    String Name;
                public:
                    ///////////////////////////////////////////////////////////////////////////////////////
                    // RubyScript basics
                    /// @brief Simple constructor, creates a script that executes a no-op.
                    RubyScript();

                    /// @brief Compiling Constructor
                    /// @param SourceCode The source of the script to be used in this.
                    /// @param Compiler Defaults to a null pointer. If passed a null pointer this does nothing. If passed a valid RubyScriptingEngine then that engine is used to compile (but not run) this script.
                    RubyScript(const String& InitialSourceCode, RubyScriptingEngine* Compiler=0, );

                    /// @brief Compiling Cosntructor without pointer
                    /// @param SourceCode The source of the script to be used in this.
                    RubyScript(const String& InitialSourceCode, RubyScriptingEngine& Compiler);

                    /// @brief Virtual destructor
                    virtual ~RubyScript();

                    ///////////////////////////////////////////////////////////////////////////////////////
                    // Arguments
                    /// @copydoc Mezzanine::Scripting::iScript::AddArgument
                    virtual void AddArgument(CountedPtr<iScriptArgument> Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A RubyIntegerArgument to pass into the script
                    virtual void AddArgument(RubyIntegerArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A RubyRealArgument to pass into the script
                    virtual void AddArgument(RubyRealArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A RubyWholeArgument to pass into the script
                    virtual void AddArgument(RubyWholeArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A RubyStringArgument to pass into the script
                    virtual void AddArgument(RubyStringArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A RubyBoolArgument to pass into the script
                    virtual void AddArgument(RubyBoolArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A RubyNilArgument to pass into the script
                    virtual void AddArgument(RubyNilArgument Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a RubyIntegerArgument
                    /// @param Arg An Integer to pass into the script
                    virtual void AddArgument(Integer Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a RubyRealArgument
                    /// @param Arg An Real to pass into the script
                    virtual void AddArgument(Real Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a RubyWholeArgument
                    /// @param Arg An Whole to pass into the script
                    virtual void AddArgument(Whole Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a RubyStringArgument
                    /// @param Arg An String to pass into the script
                    virtual void AddArgument(String Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a RubyStringArgument
                    /// @param Arg An Char8* to pass into the script
                    virtual void AddArgument(Char8* Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a RubyBoolArgument
                    /// @param Arg An Bool to pass into the script
                    virtual void AddArgument(Bool Arg);

                    /// @brief When called with no arguments this inserts a Rubynil
                    virtual void AddArgument();

                    /// @copydoc Mezzanine::Scripting::iScript::RemoveArgument
                    virtual void RemoveArgument(CountedPtr<iScriptArgument> Arg);

                    /// @copydoc Mezzanine::Scripting::iScript::RemoveArgument
                    virtual void RemoveArgument(Whole ArgNumber);

                    /// @copydoc Mezzanine::Scripting::iScript::GetArgumentCount
                    virtual Whole GetArgumentCount() const;

                    /// @copydoc Mezzanine::Scripting::iScript::ClearArguments
                    virtual void ClearArguments();

                    /// @copydoc Mezzanine::Scripting::iScript::GetArgument
                    virtual CountedPtr<iScriptArgument> GetArgument(Whole ArgNumber) const;

                    ///////////////////////////////////////////////////////////////////////////////////////
                    // Source code
                    /// @copydoc Mezzanine::Scripting::iScript::SetSourceCode
                    virtual void SetSourceCode(const String& Code);

                    /// @copydoc Mezzanine::Scripting::iScript::GetSourceCode
                    virtual String GetSourceCode() const;

                    ///////////////////////////////////////////////////////////////////////////////////////
                    // CountedPointer Inheritance
                public:
                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual RubyScript* GetMostDerived()
                        { return this; }
            };
        } // Ruby
    } // Scripting

    /// @brief Marks RubyScript for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::Ruby::RubyScript>
    {
        public:
            /// @brief The type that maintains the Reference count for RubyScript with be RubyScript itself
            typedef Scripting::Ruby::RubyScript RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::RubyScript that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };
} // Mezzanine


#endif // MEZZRUBY
#endif // \_scriptingRuby_h
