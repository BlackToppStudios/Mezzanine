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
#ifndef _lua51script_h
#define _lua51script_h

#include "datatypes.h"

#ifdef MEZZLUA51

#include "Scripting/scriptargument.h"
#include "Scripting/script.h"
#include "Scripting/scriptingmanager.h"

/// @file
/// @brief This file has the interface for the Lua scripts.


namespace Mezzanine
{
    namespace Scripting
    {
        namespace Lua
        {
            class Lua51ScriptingEngine;

            ///////////////////////////////////////////////////////////////////////////////////////
            /// @brief This class is used to store a Lua script and in its source and compiled state.
            /// @details To execute this script use @ref LuaScriptingEngine::compile
            class MEZZ_LIB Lua51Script : public virtual Mezzanine::Scripting::iScriptCompilable, public virtual Mezzanine::Scripting::iScriptMultipleReturn
            {
                private:
                    // Makes passing internal data much easier and all Lua51 are logically encapsulated as a single system still.
                    friend class Lua51ScriptingEngine;

                    /// @brief This will contain the source of the script
                    String SourceCode;

                    /// @brief If the script is compiled this will be used to store it.
                    BinaryTools::BinaryBuffer CompiledByteCode;

                    /// @brief This will compile the Lua script
                    /// @param Compiler This will be used to compile the script, no safety checks are performed.
                    void Compile(Lua51ScriptingEngine* Compiler);

                    /// @brief A set of the arguments being passed into the Lua script
                    ArgumentGroup Args;

                    /// @brief The Expected amount of return values after execution
                    Whole ReturnCount;

                    /// @brief A set of all the values the Lua script returned the last time it was executed.
                    ArgumentGroup Returns;

                public:
                    /// @brief Simple constructor, creates a script that executes a no-op.
                    Lua51Script();

                    /// @brief Compiling Constructor
                    /// @param SourceCode The source of the script to be used in this.
                    /// @param Compiler Defaults to a null pointer. If passed a null pointer this does nothing. If passed a valid LuaScriptingEngine then that engine is used to compile (but not run) this script.
                    Lua51Script(const String& InitialSourceCode, Lua51ScriptingEngine* Compiler=0, Whole CreationReturnCount=0);

                    /// @brief Virtual destructor
                    virtual ~Lua51Script();

                    /// @copydoc Mezzanine::Scripting::iScript::AddArgument
                    virtual void AddArgument(CountedPtr<iScriptArgument> Arg);

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

                    /// @copydoc Mezzanine::Scripting::iScript::SetSourceCode
                    virtual void SetSourceCode(const String& Code);

                    /// @copydoc SetSourceCode(const String&)
                    /// @param CodeReturnCount Set the possinle return count of this script
                    virtual void SetSourceCode(const String& Code, Whole CodeReturnCount);

                    /// @copydoc Mezzanine::Scripting::iScript::GetSourceCode
                    virtual String GetSourceCode() const;

                    /// @copydoc Mezzanine::Scripting::iScriptCompilable::SetByteCode
                    virtual void SetByteCode(BinaryTools::BinaryBuffer Code);

                    /// @copydoc Mezzanine::Scripting::iScriptCompilable::GetByteCode
                    virtual BinaryTools::BinaryBuffer GetByteCode() const;

                    /// @copydoc Mezzanine::Scripting::iScriptCompilable::IsCompiled
                    virtual bool IsCompiled() const;

                    /// @copydoc Mezzanine::Scripting::iScriptMultipleReturn::GetReturnCount
                    virtual Whole GetReturnCount() const;

                    /// @brief How many returns will the current source code return.
                    /// @details This can be set on construction or when loading new source code.
                    /// @return A whole containing the amount of returns the current Script will return after it is next executed.
                    virtual Whole GetTargetReturnCount() const;

                    /// @copydoc Mezzanine::Scripting::iScriptMultipleReturn::GetAllReturns
                    virtual ArgumentGroup GetAllReturns() const;

                    //For the CountedPointer Inheritance
                public:
                    /// @brief Get a pointer to the most Derived type of this class
                    /// @return A pointer of the most derived pointing to this.
                    virtual Lua51Script* GetMostDerived()
                        { return this; }
            };
        } // Lua
    } // Scripting

    /// @brief Marks Lua51Script for internal reference counting if a CountedPtr checks
    template <>
    class ReferenceCountTraits <Scripting::Lua::Lua51Script>
    {
        public:
            typedef Scripting::Lua::Lua51Script RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };

    };
} // Mezzanine


#endif // MEZZLUA51
#endif // \_scriptinglua_h
