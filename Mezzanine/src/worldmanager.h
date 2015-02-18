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
#ifndef _worldmanager_h
#define _worldmanager_h

#include "managerbase.h"
#include "enumerations.h"

namespace Mezzanine
{
    class World;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldManager
    /// @headerfile worldmanager.h
    /// @brief This is the base class for all managers that belong to a single world instance.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB WorldManager : public ManagerBase
    {
    protected:
        /// @internal
        /// @brief A pointer to the world that created this manager.
        World* ParentWorld;
        /// @internal
        /// @brief This stores whether or not processing for some or all objects in this manager has been paused.
        Boole OperationsPaused;
    public:
        /// @brief Class constructor.
        WorldManager(World * ParentWorld = NULL);
        /// @brief Class destructor.
        virtual ~WorldManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the world this manager belongs to.
        /// @return Returns a pointer to the world that created this manager.
        virtual World* GetWorld() const;

        /// @brief Sets the pause state of this manager, or has no effect depending on the value passed in.
        /// @param PL A bitfield describing the pause level being assigned to the parent world of this manager.
        virtual void Pause(const UInt32 PL) = 0;
        /// @brief Gets whether or not this manager is currently paused.
        /// @return Returns true if this manager has some or all of it's operations paused currently.
        virtual Boole IsPaused() const;

        /// @copydoc ManagerBase::Initialize()
        /// @exception If this is called while no valid world is set, this will throw an "INVALID_STATE_EXCEPTION".
        virtual void Initialize();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Sets the world this manager belongs to.
        /// @exception If this manager is already initialized, this method will throw an "INVALID_STATE_EXCEPTION".
        /// @note When hooking up to other subsystems this will be the world used to retrieve the other managers for those systems.
        /// @param Parent The world that this manager will belong to.
        virtual void _SetWorld(World* Parent);
    };//WorldManager
}//Mezzanine

#endif
