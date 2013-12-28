// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _scriptworkunit_h
#define _scriptworkunit_h

/// @file
/// @brief This file has the interfaces for ScriptsWorkUnit

#include "script.h"
#include "countedptr.h"

#include "Threading/workunit.h"

namespace Mezzanine
{
    namespace Scripting
    {
        // Forward Declares
        class iScriptCompilable;
        class iScriptMultipleReturn;

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The interface for a container of script that can be executed each frame
        /// @details All the methods that all script wokunits for all languages must implement.
        /// These are minimalistic containers which will execute some of all of the scripts the container
        /// every frame.
        /// @n @n
        /// This uses multiple inheritance to minimize the amount of features a scripting langauge with need to
        /// implement.
        ///////////////////////////////////////
        class MEZZ_LIB iScriptWorkUnit : public Threading::DefaultWorkUnit
        {
            public:
                /// @brief This adds a script to list to be run each frame.
                /// @param SCriptToAdd A CountedPtr to a script
                virtual void AddScript(CountedPtr<iScript> ScriptToAdd) = 0;

                /// @brief Remove a Script.
                /// @param ArScriptToRemoveg A CountedPtr matching the one to be removed.
                virtual void RemoveScript(CountedPtr<iScript> ScriptToRemove) = 0;

                /// @brief Remove Script based on index.
                /// @details This removes the specified Script from the internal list. This should be treated as taking linear
                /// time, relative to the total count of svripts assigned to this workunit, to run.
                /// @param Index The number of the Script to be removed. This behaves similar to an array or vector as it starts counting at 0.
                virtual void RemoveScript(Whole Index) = 0;

                /// @brief How many Scripts have been added to this workunit
                /// @return A Whole containing the amount of iScripts passed in so far.
                virtual Whole GetScriptCount() const = 0;

                /// @brief Remove all the Scripts!!! http://imgur.com/PI8YiyG
                /// @details This should run in constant time. It still might be slower than removing and reading just one a few Scripts.
                virtual void ClearScripts() = 0;

                /// @brief Retrieve a Script previously passed in.
                /// @param Index The index of the passed parameter to retrun.
                /// @return A reference counted pointer to an iScript.
                virtual CountedPtr<iScript> GetScript(Whole Index) = 0;

                /// @brief Virtual deconstructor
                virtual ~iScriptWorkUnit() {}

        }; // iScript
    }


}//Mezzanine



#endif // \_script_h
