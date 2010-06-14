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

namespace Ogre
{
    class Node;
}

namespace phys
{


    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorContainerBase
    /// @headerfile actorcontainerbase.h
    /// @brief A base class to unify the interface for different kinds of containers for holding actors
    /// @details Containers for actors must implement atleast this interface(abstract base class) to
    /// be usable with the phys::World for tracking in game objects. There are several reasons why this
    /// will be useful. Our first thought was deriving from this and an STL container like vector or
    /// list. Members of this class should be implementing or inheriting a proper container\n\n
    /// The phys world will use one of these containers to store all of the actors for tracking purposes
    /// Since  \n\n
    /// Currently, you cannot simply swap one member of this with another, in a running phys::World. We are
    /// Looking into the feasibility of implementing this. \n\n
    /// However the is no reason an actor could be in multiple containers so this could be a viable work
    /// around for actor sorting and categorization at runtime. \n\n
    /// Because of this classes representation of a cursor only 1 thread at a time should use the cursor
    /// movement functions. For the container that the phys::World keeps it should be assume that the
    /// cursor is used. For other containers you should manage you container carefully and/or use another
    /// iteration method, such as STL iterators.
    class ActorContainerBase
    {
        private:
            /// @brief This is expected to be a pointer to the world this will be rendered with and built on
            World* ParentWorld;

        protected:
            /// @internal
            /// @brief Used to work around the scenenode of an Actor being private, so all derived Containers can access it.
            Ogre::Node* GetNode(ActorBase* actor);

        public:
            /// @brief Basic Constructor
            /// @details This just assigned the passed pointer to ParentWorld
            /// @param ParentWorld_ This is the world that will perform physics and graphical work on the actors in this container
            ActorContainerBase(World* ParentWorld_);

            /// @brief Destructor
            /// @details This really doesn't do anything, but if someone needs to overload it, it's here
            virtual ~ActorContainerBase();

            /// @brief This will add an Actor to this container and the world
            /// @details This will add an Actor to this container and the world, and handle the nitty gritty details
            /// of add this to physics subsystem and graphics subsystem. \n\n
            /// This will not add the Actor to any specific location in the ordering of the container.
            /// @param ActorToAdd This is a pointer to the actor to add
            virtual void AddActor(ActorBase* ActorToAdd) = 0;

            /// @brief This provides an easy way to access the last Actor added to this container
            /// @details For many containers this will simply return a pointer to the last actorl
            virtual ActorBase* LastActorAdded() = 0;

            /// @brief Remove an Actor
            /// @details Remove all references of the actor pointed from the container. Will throw an exception when attempting to remove
            /// and no match could be found.
            virtual void RemoveActor(ActorBase* ActorToRemove) = 0;

            /// @brief Removes the current actor
            /// @details This removes the actor the cursor at. Will throw an exception when attempting to remove from an empty container.
            /// Where the cursor goes is implementation dependent.
            virtual void RemoveActorAtCursor() = 0;

            /// @brief Returns how many actors this stores
            /// @return This returns a Whole number with the count of actors
            virtual Whole GetActorCount() const = 0;

            /// @brief This moves the cursor to the first item in the container
            /// @details This moves the cursor to the first item in the container change or return anything else.
            /// An exception will be throw if there are no valid items to move to with the cursor. There must be atleast one item to use any cursor
            /// moving functions.
            virtual void CursorToFirst() = 0;

            /// @brief This moves the cursor to the previous item in the container
            /// @details This moves the cursor to the previous item in the container, and if you started at the last item, you will
            /// visit every item in a properly implemented container, except for items that may have bee added during your traversal.
            /// It is also posible this could visit the same actor twice or more. When called from the first item this does nothing.
            /// An exception will be throw if there are no valid items to move to with the cursor. There must be atleast one item to use any cursor
            /// moving functions.
            virtual void CursorToPrevious() = 0;

            /// @brief This moves the cursor to the next item in the container
            /// @details This is the same as CursorToPrevious() except if you start from the begin you'll work your way to the end. If
            /// you are at the end, you'll stay their if you call this again
            virtual void CursorToNext() = 0;

            /// @brief This moves the cursor to the last item in the container
            /// @details This moves the cursor to the last item in the container change or return anything else. See CursorToFirst() for more details.
            virtual void CursorToLast() = 0;

            /// @brief This gets the actor at the cursor
            /// @details This gets the actor at the cursor, and will not move the cursor. If the cursor has not be set to a location, any valid
            /// actor in the container could be returned. Will throw an exception when attempting to get from an empty container.
            /// @return This returns a pointer to an ActorBase.
            virtual ActorBase* GetAtCursor() const = 0;

            /// @brief This gets the first actor in the container
            /// @details This and the actor the cursor points at after CursorToFirst() should match.
            /// @return This returns a pointer to an ActorBase. Will throw an exception when attempting to get from an empty container.
            virtual ActorBase* GetFirst() const = 0;

            /// @brief This gets the last actor in the container
            /// @details This and the actor the cursor points at after CursorToLast() should match.
            /// @return This returns a pointer to an ActorBase. Will throw an exception when attempting to get from an empty container.
            virtual ActorBase* GetLast() const = 0;

            /// @brief This finds an actor by searching for a graphics subsystem nodes
            /// @details This will iterate through each Actor in the container until it finds one with a matching Node
            /// @return This returns a pointer to and ActorBase that has a matching node
            virtual ActorBase* FindActor(Ogre::Node* GraphicsNode) = 0;

            /// @brief Which kind of container it this anyway.
            /// @details Since this interface could be used with any type of containers and innumerable 3rd party container implemention this
            /// can be used to more safely cast this container to a more specific type.
            /// @return This returns a phys::String
            virtual String GetType() const = 0;

    };
}

#endif
