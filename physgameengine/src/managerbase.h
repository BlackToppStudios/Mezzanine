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

#include "world.h"

///////////////////////////////////
// Actual code
namespace phys
{

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

            /// @brief A pointer to the World the actor will reside.
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
                UnknownType,        /// This is the type return if the derived manager failed to create a GetType() function
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
            ///
            ManagerBase(World* GameWorld_);

            virtual void Initialize() = 0;

            virtual World* GetGameWorld() const;

            virtual void SetGameWorld( World* GameWorld_ );

            virtual ManagerTypeName GetType() const;

    };// /ManagerBase
} // /phys



#endif
