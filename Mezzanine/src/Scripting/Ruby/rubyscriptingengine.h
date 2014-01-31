// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _rubyscriptingengine_h
#define _rubyscriptingengine_h

#include "datatypes.h"

#ifdef MEZZRUBY

#include "Scripting/scriptargument.h"
#include "Scripting/script.h"
#include "Scripting/scriptingmanager.h"

/// @file
/// @brief This file has the interface for the Ruby based implementation of the Scripting system.

/// @page RubyManual Mezzanine::Scripting Ruby Manual
/// The file that SWIG generated was made with the following command run on an ubuntu machine from the Mezzananine/src directory:
/// swig2.0 -c++ -v -Wall -ruby -importall -includeall -o Scripting/Ruby/scriptbindingruby.cpp mezzanine.h
/// swig2.0 -c++ -v -Wall -ruby -importall -includeall -DSWIG_UNSAFE -o Scripting/Ruby/scriptbindingrubyunsafe.cpp mezzanine.h
/// Need to document "Mezzanine.", nspaces, calling conventions
///
/// script class compilation
///
/// Unlike the Lua scripting manager this does not attempt to create
struct ruby_State;

namespace Mezzanine
{
    namespace Scripting
    {
        class iScriptCompilationManager;
        class RubyScript;

        /// @brief This contains the Ruby and Ruby52 resources.
        namespace Ruby
        {
            ///////////////////////////////////////////////////////////////////////////////////////
            /// @brief The workhorse of the Ruby scripting system. All scripts come here to be executed.
            class MEZZ_LIB RubyScriptingEngine : public Mezzanine::Scripting::iScriptCompilationManager
            {
                ///////////////////////////////////////////////////////////////////////////////////////
                // Internal Stuff first
                private:
                    // Makes passing internal data much easier and all Ruby are logically encapsulated as a single system still.
                    friend class RubyScript;

                public:

                ///////////////////////////////////////////////////////////////////////////////////////
                // Construction/Deconstruction
                    /// @brief Constructs a Scripting engine with a set of libraries preloaded.
                    explicit RubyScriptingEngine();

                    /// @brief Virtual Deconstructor
                    virtual ~RubyScriptingEngine();

                ///////////////////////////////////////////////////////////////////////////////////////
                // Execution
                    /// @brief Compile and execute a passed string.
                    /// @param ScriptSource A String containing the source code to be executed.
                    /// @details This will create a CountPtr to a RubyScript and assign both its Source and Byte code
                    virtual CountedPtr<iScript> Execute(const String& ScriptSource);

                    /// @brief Implements a required for iScriptManager, Calls Execute(CountedPtr<RubyScript>)
                    /// @param ScriptToRun A CountedPtr<iScript> to be run. This is cast to an CountedPtr<RubyScript> and called if possible.
                    /// @throw If this cannot be cast this throws a ParametersCastException.
                    /// @todo fill in the kind of exception thrown.
                    virtual void Execute(CountedPtr<iScript>& ScriptToRun);

                    /// @brief This will execute the passed script, compiling it if not present
                    /// @param ScriptToRun The script to execute.
                    /// @details If a bytecode is present on ScriptToRun then it is executed. Otherwise the Source is
                    /// compiled and the result is set as the bytecode and it is executed.
                    virtual void Execute(CountedPtr<RubyScript>& ScriptToRun);

                    /// @brief Performs the compilation on a script reference
                    /// @param ScriptToCompile A reference to the RubyScript to compile.
                    virtual void Execute(RubyScript& ScriptToRun);

                    /// @brief Performs the compilation on a raw pointer
                    /// @param ScriptToCompile A pointer to the RubyScript to compile.
                    virtual void Execute(RubyScript* ScriptToRun);

                ///////////////////////////////////////////////////////////////////////////////////////
                // For Inheritance
                    /// @copydoc ManagerBase::GetImplementationTypeName()
                    /// @return A String containing "RubyScriptingEngine".
                    virtual String GetImplementationTypeName() const;

            };

        } // Ruby
    } // Scripting
} // Mezzanine


#endif // MEZZRUBY
#endif // \_scriptingruby_h
