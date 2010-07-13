//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _managerbase_h
#define _managerbase_h

#include "worldgetset.h"

namespace phys
{
    class World;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ManagerBase
    /// @headerfile managerbase.h
    /// @brief This is the base class from which all the World Managers inherit.
    /// @details This creates a base set of functions that Managers are all
    /// expected to implement.
    ///////////////////////////////////////
    class ManagerBase
    {
        protected:
            /// @internal
            /// @brief The actual pointer to the world
            World* GameWorld;

        public:

            /// @enum ManagerTypeName
            /// @brief A listing of Manager TypeNames
            /// @details These will be returned by ManagerBase::GetType(), and will allow
            /// code using this to determine what type of Manager class they are working with
            /// and use this information to more safely cast to the correct manager if needed.
            enum ManagerTypeName
            {
                ActorContainerBase,
                CallBackManager,
                CameraManager,
                EventManager,
                GraphicsManager,
                PhysicsManager,
                SoundManager,
                UserCreated         /// This is what User created managers that do not derive from any other managers are expected to use to prevent confusion with game internals
            };

            /// @brief Default Constructor
            /// @details This creates a default manager without a valid reference to the game world. The
            /// lack of such a reference could cause issues or change behavior of a Manager class. A Manager
            /// Must be given the pointer to the world it is suppposed to work with prior to the calling
            /// of ManagerBase::Initialize(). This will most likely be the constructor used when creating
            /// a Manager to pass into the constructor of a new phys::World.
            ManagerBase();

            /// @brief Simple Constructor
            /// @details This is the prefered constructor. This is used to "Attach" a manager to a phys::world. This is expected to
            /// configure everything that this manager will need except for items requiring integration with other managers.
            /// @param GameWorld_ This is the phys::World this Manager is expected to work with.
            ManagerBase(World* GameWorld_);

            /// @brief Deconstructor
            /// @details This is actuall
            virtual ~ManagerBase();

            /// @brief Configure Items requiring other Managers
            /// @details If you are using the phys::World this is called when phys::World::GameInit() is called. It is expected that
            /// by the time this is called either ManagerBase::ManagerBase(World*) or ManagerBase::SetGameWorld(World*) will have been called.
            /// This is where all configuration that requires atleast one other manager on the phys::World to exist.\n\n
            /// Yes we know it is spelled wrong, but are Zs cooler anyway.
            virtual void Initialize() = 0;

            /// @brief This gets the World that this manager is working with.
            /// @return This returns a phys::World* that is the same as the one store in this Manager
            virtual World* GetGameWorld() const;

            /// @brief This sets the phys::World that this Manager works with.
            /// @details It is expected that this won't change very much, and for some managers changing this at the wrong time could
            /// cause some very fundamental problems. In fact, managers should be designed so that they
            /// can swapped  out. For example swapping out event managers could allow for a swift re-mapping of game controls when
            /// a game switches modes.\n\n
            /// For managers that can be moved it is expected that this function will change the pointer on the phys::World for the
            /// appropriate manager to point to this manager. This simplifies the calls that will naturally come next. To detach a
            /// Manager from the world pass this method a NULL pointer. If the manager cannot be removed or detached it should
            /// throw and exception using World::LogAndThrow, and it must not fail to attach to a world (that means it must internally
            /// handle all exception raised by the attaching process or throw an "Unrecoverable Error")
            virtual void SetGameWorld( World* GameWorld_ );

            /// @brief This returns the type of a manager.
            /// @details This is intended to make using and casting from Manager base easier. With this is is possible to cast from
            /// ManagerBase to the correct Manager Type.
            /// @return This returns a ManagerTypeName to identify what this can be safely cast to.
            virtual ManagerTypeName GetType() const = 0;

    };// /ManagerBase
} // /phys



#endif
