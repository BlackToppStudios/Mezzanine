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
#ifndef _scriptingmanager_h
#define _scriptingmanager_h

/// @file
/// @brief This file has the interfaces for Scripting managers and the associated dependency chain

#include "managerbase.h"
#include "Scripting/scriptargument.h"
#include "Scripting/script.h"
#include "singleton.h"

namespace Mezzanine
{
    namespace Scripting
    {

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief What a scripting manager should look like.
        /// @details This scripting manager implements no scripting language, but serves as
        /// model for what scripting managers.
        /// @n @n
        /// If a scripting language does not support compilation It should implement
        /// this
        ///////////////////////////////////////
        class MEZZ_LIB iScriptingManager : public ManagerBase
        {
            protected:

            public:
                /// @brief Class constructor.
                /// @details Sets Priority as required by single threaaded main loop
                iScriptingManager();

                /// @brief Class destructor.
                virtual ~iScriptingManager();

                ///////////////////////////////////////////////////////////////////////////////
                // Scripting Manager Core Functionality

                /// @brief Run a string containing script soure code.
                /// @param ScriptSource The source code of the script.
                /// @details In implementations it is expected that this will create a script
                /// object, execute it and return it.
                /// @return A Shared pointer to the created Script.
                virtual CountedPtr<iScript> Execute(const String& ScriptSource) = 0;

                /// @brief Run the script.
                /// @param ScriptToRun A shared ptr that points to a script to run.
                /// @details In Scripting langauge implementations this is expected to
                /// run (compile if needed) a script.
                virtual void Execute(CountedPtr<iScript>& ScriptToRun) = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc ManagerBase::Initialize()
                virtual void Initialize();

                /// @copydoc ManagerBase::Deinitialize()
                virtual void Deinitialize();

                ///////////////////////////////////////////////////////////////////////////////
                // Type Identifier Methods

                /// @copydoc ManagerBase::GetInterfaceType()
                virtual ManagerType GetInterfaceType() const;

                /// @copydoc ManagerBase::GetImplementationTypeName()
                virtual String GetImplementationTypeName() const = 0;
        }; //ScriptingManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief What a scripting manager should look like.
        /// @details This scripting manager implements no scripting language, but serves as
        /// model for what scripting managers
        ///////////////////////////////////////
        class MEZZ_LIB iScriptCompilationManager : public iScriptingManager
        {
            public:
                /// @brief Overidable deconstructor.
                virtual ~iScriptCompilationManager();

                /// @brief Compile a script.
                /// @param ScriptToCompile A script object to be compiled.
                virtual void Compile(CountedPtr<iScriptCompilable>& ScriptToCompile) = 0;

                /// @brief Comvert a string into a Script that is compiled and ready for use.
                /// @param SourceToCompile A string containing source code in the target language.
                /// @return The Script compiled and ready for use.
                virtual CountedPtr<iScriptCompilable> Compile(const String& SourceToCompile) = 0;
        };

    }// Scripting
}//Mezzanine

// possible languages Lua, Tcl, squirrel, Angelscript, falcon

#endif // \_scriptingmanager_h
