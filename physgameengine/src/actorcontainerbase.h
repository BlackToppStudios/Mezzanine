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
#ifndef actorcontainerbase_h
#define actorcontainerbase_h

//Internal includes
#include "actorbase.h"
#include "world.h"

namespace phys
{


    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorContainerBase
    /// @headerfile actorcontainerbase.h
    /// @brief A base class to unify the interface for different kinds of containers for holding actors
    /// @details Containers for actors must implement atleast this interface(abstract base class) to
    /// be usable with the phys::World for tracking in game objects. There are several reasons why this
    /// will be useful. Our first thought was deriving from this and an STL container like vector or
    /// list. \n\n
    /// Since members of this class should be implementing or inheriting a proper container\n\n
    /// Currently, you cannot simply swap one member of this with another, in a running physworld. We are
    /// Looking into the feasibility of implementing this.
    class ActorContainerBase
    {
        private:
            World* ParentWorld;

        public:
            /// @brief This will add an Actor to this container and the world
            /// @details This will add an Actor to this container and the world, and handle the nitty gritty details
            /// of add this to physics subsystem and graphics subsystem. \n\n
            /// This will not add the Actor to any specific location in the ordering of the container.
            virtual void AddActor(ActorBase* ActorToAdd) = 0;

            virtual void RemoveActor(ActorBase* ActorToAdd) = 0;
            virtual void RemoveActorAtCursor(ActorBase* ActorToAdd) = 0;

            virtual Whole GetActorCount() const = 0;

            virtual void CursorToFirst() = 0;
            virtual void CursorToPrevious() = 0;
            virtual void CursorToNext() = 0;
            virtual void CursorToLast() = 0;

            virtual void AddActorBeforeCursor(ActorBase* ActorToAdd) = 0;
            virtual void AddActorAfterCursor(ActorBase* ActorToAdd) = 0;
            virtual ActorBase* GetAtCursor() const = 0;
            virtual ActorBase* GetFirst() const = 0;
            virtual ActorBase* GetLast() const = 0;

            virtual String GetType() const = 0;

    };
}

#endif
