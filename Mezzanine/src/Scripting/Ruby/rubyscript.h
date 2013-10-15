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
#ifndef _Ruby51script_h
#define _Rubyscript_h

#include "datatypes.h"

#ifdef MEZZLUA51

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
            class Ruby51ScriptingEngine;
            class Ruby51IntegerArgument;
            class Ruby51RealArgument;
            class Ruby51WholeArgument;
            class Ruby51StringArgument;
            class Ruby51BoolArgument;
            class Ruby51NilArgument;

            /// @brief An expanded version of the BinaryTools::BinaryBuffer to carry one tiny piece of metadata around with it.
            class FlaggedBuffer : public BinaryTools::BinaryBuffer
            {
                public:
                    /// @brief just a passthru to BinaryBuffer::operator=
                    /// @param RH The assiging binary buffer
                    /// @return This returns a reference to this for operator chaing and such.
                    FlaggedBuffer& operator= (const BinaryBuffer& RH)
                    {
                        BinaryBuffer::operator=(RH);
                        return *this;
                    }

                    /// @brief Used to indicate that this data has been loaded into Ruby.
                    bool Loaded;

                    /// @brief Constructor just set the extra flag to false.
                    FlaggedBuffer() : Loaded(false)
                        {}
            };

            ///////////////////////////////////////////////////////////////////////////////////////
            /// @brief This class is used to store a Ruby script and in its source and compiled state.
            /// @details To execute this script use @ref RubyScriptingEngine::compile
            class MEZZ_LIB Ruby51Script : public virtual Mezzanine::Scripting::iScriptCompilable, public virtual Mezzanine::Scripting::iScriptMultipleReturn
            {
                private:
                    /// @brief Makes passing internal data much easier and all Ruby51 are logically encapsulated as a single system still.
                    friend class Ruby51ScriptingEngine;

                    /// @brief A set of the arguments being passed into the Ruby script
                    ArgumentGroup Args;

                    /// @brief A set of all the values the Ruby script returned the last time it was executed.
                    ArgumentGroup Returns;

                    /// @brief This will contain the source of the script
                    String SourceCode;

                    /// @brief Stores the Name of the Ruby Script
                    String Name;

                    /// @brief If the script is compiled this will be used to store it.
                    FlaggedBuffer CompiledByteCode;

                    /// @brief If set this script just calls an existing lua function.
                    /// @todo Add a function to set/get this
                    Bool FunctionCall;

                public:
                    ///////////////////////////////////////////////////////////////////////////////////////
                    // RubyScript basics
                    /// @brief Simple constructor, creates a script that executes a no-op.
                    Ruby51Script();

                    /// @brief Compiling Constructor
                    /// @param SourceCode The source of the script to be used in this.
                    /// @param Compiler Defaults to a null pointer. If passed a null pointer this does nothing. If passed a valid RubyScriptingEngine then that engine is used to compile (but not run) this script.
                    Ruby51Script(const String& InitialSourceCode, Ruby51ScriptingEngine* Compiler=0, Bool JustAFunctionCall = false, String ScriptName = "?");

                    /// @brief Compiling Cosntructor without pointer
                    /// @param SourceCode The source of the script to be used in this.
                    /// @param
                    /// @param
                    Ruby51Script(const String& InitialSourceCode, Ruby51ScriptingEngine& Compiler, Bool JustAFunctionCall = false, String ScriptName = "?");

                    /// @brief Virtual destructor
                    virtual ~Ruby51Script();

                    ///////////////////////////////////////////////////////////////////////////////////////
                    // Arguments
                    /// @copydoc Mezzanine::Scripting::iScript::AddArgument
                    virtual void AddArgument(CountedPtr<iScriptArgument> Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A Ruby51IntegerArgument to pass into the script
                    virtual void AddArgument(Ruby51IntegerArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A Ruby51RealArgument to pass into the script
                    virtual void AddArgument(Ruby51RealArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A Ruby51WholeArgument to pass into the script
                    virtual void AddArgument(Ruby51WholeArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A Ruby51StringArgument to pass into the script
                    virtual void AddArgument(Ruby51StringArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A Ruby51BoolArgument to pass into the script
                    virtual void AddArgument(Ruby51BoolArgument Arg);

                    /// @brief Another overload to make adding arguments easier
                    /// @param Arg A Ruby51NilArgument to pass into the script
                    virtual void AddArgument(Ruby51NilArgument Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a Ruby51IntegerArgument
                    /// @param Arg An Integer to pass into the script
                    virtual void AddArgument(Integer Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a Ruby51RealArgument
                    /// @param Arg An Real to pass into the script
                    virtual void AddArgument(Real Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a Ruby51WholeArgument
                    /// @param Arg An Whole to pass into the script
                    virtual void AddArgument(Whole Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a Ruby51StringArgument
                    /// @param Arg An String to pass into the script
                    virtual void AddArgument(String Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a Ruby51StringArgument
                    /// @param Arg An Char8* to pass into the script
                    virtual void AddArgument(Char8* Arg);

                    /// @brief Another overload to make adding arguments easier, This one adds a Ruby51BoolArgument
                    /// @param Arg An Bool to pass into the script
                    virtual void AddArgument(Bool Arg);

                    /// @brief When called with no arguments this inserts a Ruby51nil
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

                    /// @copydoc SetSourceCode(const String&)
                    /// @param Return1 Something for containing the return from the script
                    virtual void SetSourceCode(const String& Code, CountedPtr<iScriptArgument> Return1);

                    /// @copydoc Mezzanine::Scripting::iScript::GetSourceCode
                    virtual String GetSourceCode() const;

                    /// @copydoc Mezzanine::Scripting::iScriptCompilable::SetByteCode
                    virtual void SetByteCode(BinaryTools::BinaryBuffer Code);

                    /// @copydoc Mezzanine::Scripting::iScriptCompilable::GetByteCode
                    virtual BinaryTools::BinaryBuffer GetByteCode() const;

                    /// @brief Get a reference to bytecode instead of a copy.
                    /// @return A BinaryTools::BinaryBuffer& containing either nothing or the Bytecode that would executed.
                    virtual FlaggedBuffer& GetByteCodeReference();

                    /// @copydoc Mezzanine::Scripting::iScriptCompilable::IsCompiled
                    virtual bool IsCompiled() const;

                    /// @brief This will compile the Ruby script
                    /// @param Compiler This will be used to compile the script, no safety checks are performed.
                    void Compile(Ruby51ScriptingEngine* Compiler);

                    /// @brief This will compile the Ruby script
                    /// @param Compiler This will be used to compile the script, no safety checks are performed.
                    void Compile(Ruby51ScriptingEngine& Compiler);

                    ///////////////////////////////////////////////////////////////////////////////////////
                    // Name

                    /// @brief Get the name of the current Script
                    /// @return A String containing the name
                    String GetName() const;

                    /// @brief change the name of the Script
                    /// @param NewName The new value for the scripts name
                    void SetName(const String &NewName);

                    ///////////////////////////////////////////////////////////////////////////////////////
                    // Returns
                    /// @copydoc Mezzanine::Scripting::iScriptMultipleReturn::GetReturnCount
                    virtual Whole GetReturnCount() const;

                    /// @copydoc Mezzanine::Scripting::iScriptMultipleReturn::AddReturn
                    virtual void AddReturn(CountedPtr<iScriptArgument> ReturnArg);

                    //void Ruby51Script::AddReturn(CountedPtr<Ruby51StringArgument> ReturnArg);

                    /// @copydoc Mezzanine::Scripting::iScriptMultipleReturn::GetAllReturns
                    virtual ArgumentGroup GetAllReturns() const;

                    ///////////////////////////////////////////////////////////////////////////////////////
                    // CountedPointer Inheritance
                public:
                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Ruby51Script* GetMostDerived()
                        { return this; }
            };
        } // Ruby
    } // Scripting

    /// @brief Marks Ruby51Script for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::Ruby::Ruby51Script>
    {
        public:
            /// @brief The type that maintains the Reference count for Ruby51Script with be Ruby51Script itself
            typedef Scripting::Ruby::Ruby51Script RefCountType;

            /// @brief Given a pointer to the raw object this will return a pointer to an initialized reference count
            /// @param Target A pointer to a Scripting::Ruby::Ruby51Script that will simply be returned
            /// @return This returns whatever was passed into target because it already is a valid Reference Counter
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief This uses dynamic casting when resolving casts inside the CountedPtr
            enum { IsCastable = CastDynamic };
    };
} // Mezzanine


#endif // MEZZLUA51
#endif // \_scriptingRuby_h
