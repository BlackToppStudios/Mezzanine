//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _script_h
#define _script_h

#include "datatypes.h"
#include "scriptargument.h"
#include "smartptr.h"

namespace Mezzanine
{
    namespace Scripting
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief The interface for a script
        /// @details All the members that all script for all languages must implement.
        /// These are tiny pieces of data that can be run like miniature programs.
        ///////////////////////////////////////
        class MEZZ_LIB Script
        {
            public:
                /// @brief This adds an argument to be passed to the script.
                /// @details All arguments added with this are passed in FIFO order to the Script during or just
                /// before execution. This should normally run in constant time. Some scripting implementations may
                /// change the order arguments are passed if doing it another way mays more sense.
                /// @param Arg This accepts a pointer to a script argument and The script assumes responsibility for deleting the argument.
                virtual void AddArgument(ScriptArgument* Arg) = 0;

                /// @brief This adds an argument to be passed to the script.
                /// @details All arguments added with this are passed in FIFO order to the Script during or just
                /// before execution. This should normally run in constant time. Some scripting implementations may
                /// change the order arguments are passed if doing it another way mays more sense.
                /// @param Arg This accepts a CountedPtr to a script argument and The script shares responsibility with caller for deleting the argument.
                virtual void AddArgument(CountedPtr<ScriptArgument> Arg) = 0;

                /// @brief Remove an argument based on a CountedPtr to the script
                /// @detail This searches through the internal list and removes the first entry it finds matching this.
                /// This should be treated as taking linear time, relative to the total count of arguments assigned to this script, to run.
                /// This can be used with AddArgument to re-order the way parameters are passed into a script
                /// @param Arg A CountedPtr matching the one to be removed
                virtual void RemoveArgument(CountedPtr<ScriptArgument> Arg) = 0;

                /// @brief Remove a Script argument based on the order it will be passed into the Script at Execution.
                /// @detail This removes the  specified Argument from the internal list. This should be treated as taking linear
                /// time, relative to the total count of arguments assigned to this script, to run.
                /// @param ArgNumber The number of the Argument to be removed. This behaves similar to an array or vector as it starts counting at 0.
                virtual void RemoveArgument(Whole ArgNumber) = 0;

                /// @brief Remove all the ARGs!!! http://imgur.com/DJhw7
                /// @details This should run in constant time. It still might be slower than removing and readding just one a few arguments
                /// in simple cases
                virtual void ClearArguments() = 0;

                virtual Whole GetArgumentCount() const = 0;

                virtual CountedPtr<ScriptArgument> GetArgument(Whole ArgNumber) const = 0;

                virtual void SetCode(String Code) = 0;
                virtual String GetCode() const = 0;

                virtual bool IsCompiled() const
                    { return false; }
                virtual bool IsCompilable() const
                    { return false; }

                virtual bool IsAvailableAsString() const = 0;
                virtual bool IsAbleToReturnMultiple() const = 0;

                virtual CountedPtr<ScriptArgument> Execute() = 0;

        };

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief The interface for a script that can be compiled to bytecode
        /// @details All the members that all script for all languages must implement
        /// @n @n
        /// These are tiny pieces of data that can be run like miniature programs. In some cases
        /// they will be compiled to a bytecode that can swiftly be executed by the appropriate
        /// bytecode interpretter. This is faster than interpretting text, but slower than
        /// running native code. It is more flexible and safer than native code, because
        /// scripts may be provided by others than the original developers, and there are
        /// fewer security issues with a completely self contained language as opposed to
        /// actual native code.
        ///////////////////////////////////////
        class MEZZ_LIB ScriptCompilable : public Script
        {
            public:
                virtual void SetByteCode(String Code) = 0;
                virtual String GetByteCode() const = 0;

                virtual bool IsCompiled() const = 0;
                virtual bool IsCompilable() const
                    { return true; }

                virtual bool IsAvailableAsString() const = 0;

                virtual void Compile() = 0;
                virtual CountedPtr<ScriptArgument> Execute() = 0;
        };

    }


}//Mezzanine



#endif // \_script_h
