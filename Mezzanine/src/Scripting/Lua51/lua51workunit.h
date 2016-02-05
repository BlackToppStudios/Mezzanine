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
#ifndef _lua51workunit_h
#define _lua51workunit_h

#include "datatypes.h"

#ifdef MEZZLUA51

#include "Scripting/Lua51/lua51script.h"
#include "Scripting/scriptworkunit.h"

#include "Threading/workunit.h"

#include <vector>

/// @file
/// @brief This file has the declaration for a workunit that can execute lua script every frame


namespace Mezzanine
{
    namespace Scripting
    {
        namespace Lua
        {
            // foraward declarations
            class Lua51ScriptingEngine;

            /// @brief This is a simple Container of script that will execute every script it is
            /// given in order each frame.
            /// @details This WorkUnit starts with no dependencies, the developer using this must
            /// set those, (see  @ref Threading::DefaultWorkUnit::AddDependency for details). This
            /// is not automatically added to the WorkScheduler on the Entresol, and must be added
            /// manually.
            /// @n @n
            /// Internally this uses an std::vector to store scripts and each frame it will iterate
            /// over them and execute them one at a time. This exposes iterators and a few convience
            /// function to make the script manageable.
            /// @n @n
            /// All scripts are stored in CountedPtr to allow for shared ownership.
            class MEZZ_LIB Lua51WorkUnit : public Scripting::iScriptWorkUnit
            {
                private:
                    /// @internal
                    /// @brief Stores the pointers to each script to run.
                    std::vector<CountedPtr<Lua51Script> > ScriptsToRun;
                    /// @brief A basic iterator.
                    typedef std::vector<CountedPtr<Lua51Script> >::iterator iterator;
                    /// @brief A read only iterator .
                    typedef std::vector<CountedPtr<Lua51Script> >::const_iterator const_iterator;

                    /// @brief Used to track where to run the scripts
                    Lua51ScriptingEngine* LuaRuntime;
                public:

                    /// @brief Create a Lua51WorkUnit
                    /// @param TargetRuntime The Lua runtime to execute Scripts against.
                    Lua51WorkUnit(Lua51ScriptingEngine* TargetRuntime);
                    /// @brief Virtual deconstructor
                    virtual ~Lua51WorkUnit();

                    /// @brief Adds a script to be run once each frame.
                    /// @param ScriptToAdd A CountedPtr to a script that should be run each Frame.
                    /// @note Consider this as Invalidating all iterators to this container.
                    void push_back(CountedPtr<Lua51Script> ScriptToAdd);
                    /// @brief Adds a script similar to push_back.
                    /// @param ScriptToAdd A iScript to add.
                    virtual void AddScript(CountedPtr<iScript> ScriptToAdd);
                    /// @brief Adds a script similar to push_back (Actually calls it).
                    /// @param ScriptToAdd A Lua51Script to add.
                    virtual void AddScript(CountedPtr<Lua51Script> ScriptToAdd);
                    /// @brief Add a script by providing it source code.
                    /// @param Source The Lua source code to add.
                    virtual void AddScript(String Source);


                    /// @brief Get an Iterator to a script from the counted pointer
                    /// @param Target a CountedPtr to convert into an iterator
                    /// @return If the Script has been added this returns its iterator, otherwise it returns and end() iterator.
                    /// @note Searches in Linear time, likely useless for external use, it is used internally.
                    iterator find(CountedPtr<Lua51Script> Target);
                    /// @brief Get an const_iterator to a script from the counted pointer
                    /// @param Target a CountedPtr to convert into an const_iterator
                    /// @return If the Script has been added this returns its const_iterator, otherwise it returns and end() const_iterator.
                    /// @note Searches in Linear time, likely useless for external use, it is used internally.
                    const_iterator find(CountedPtr<Lua51Script> Target) const;

                    /// @brief Erase The first found Script use that pointer
                    /// @param Target A CountedPtr to a script that has already been added. If the script has not been added this fails silently.
                    /// @note Consider this as Invalidating all iterators to this container. This takes linear time to find the Pointer in the
                    /// Container, then linear time to erase the entry for each Script after the target script.
                    void erase(CountedPtr<Lua51Script> Target);
                    /// @brief Remove the target script from the container
                    /// @param Target The script to remvoe.
                    /// @note This takes linear time to erase the entry for each Script after the target script or constant time if it is the last Script
                    void erase(iterator Target);
                    /// @brief Similar to calling erase
                    /// @param ScriptToRemove A CountedPtr to the script to remove
                    virtual void RemoveScript(CountedPtr<iScript> ScriptToRemove);
                    /// @brief The Same as calling erase and passing a CountedPtr to a script
                    /// @param ScriptToRemove A CountedPtr to the script to remove
                    virtual void RemoveScript(CountedPtr<Lua51Script> ScriptToRemove);
                    /// @brief Remove a script by index.
                    /// @param Index The index of the Script to Remove.
                    virtual void RemoveScript(Whole Index);

                    /// @brief Get an iterator to the first script.
                    /// @return An iterator.
                    iterator begin();
                    /// @brief Get a const_iterator to the first script.
                    /// @return A const_iterator.
                    const_iterator begin() const;

                    /// @brief Get an iterator one past the last script.
                    /// @return An iterator
                    iterator end();
                    /// @brief Get a const_iterator one past the last script.
                    /// @return A const_iterator
                    const_iterator end() const;

                    /// @brief How many Scripts have been added to this workunit
                    /// @return A Whole containing the amount of arguments passed in so far.
                    virtual Whole GetScriptCount() const;

                    /// @brief Remove all the Scripts from this workunit
                    /// @details This should run in constant time. It still might be slower than removing and reading just one a few Scripts.
                    virtual void ClearScripts();

                    /// @brief Retrieve a Script previously passed in.
                    /// @param Index The index of the passed parameter to retrun.
                    /// @return A reference counted pointer to a iScript.
                    virtual CountedPtr<iScript> GetScript(Whole Index);
                    /// @brief Retrieve a Script previously passed in.
                    /// @param Index The index of the passed parameter to retrun.
                    /// @return A reference counted pointer to a Lua51Script.
                    virtual CountedPtr<Lua51Script> GetLua51Script(Whole Index) const;

                    /// @brief Runs all scripts that have been added to this work unit
                    virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
            };
        } // Lua
    } // Scripting

} // Mezzanine


#endif // MEZZLUA51
#endif // \ Include gaurd
