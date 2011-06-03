//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef actormanager_h
#define actormanager_h

#include "datatypes.h"
#include "managerbase.h"

namespace phys
{
    class ActorBase;
    class ActorRigid;
    class ActorSoft;
    class ActorCharacter;
    class ActorContainerBase;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorManager
    /// @headerfile actormanager.h
    /// @brief A manager responsible for the storage and management of all actors in use.
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB ActorManager : public ManagerBase
    {
        protected:
            ActorContainerBase* Actors;
            std::vector<ActorRigid*> RigidActors;
            std::vector<ActorSoft*> SoftActors;
            std::vector<ActorCharacter*> CharacterActors;
        public:
            /// @brief Class constructor.
            ActorManager();
            /// @brief Class destructor.
            virtual ~ActorManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Managing all actors

            /// @brief Gets an Actor by Index.
            /// @param Index The index of the actor you wish to retrieve.
            /// @return Returns a pointer to the actor at the specified index.
            virtual ActorBase* GetActor(const Whole& Index);
            /// @brief Gets an Actor by Name.
            /// @param Name The name of the actor you wish to retrieve.
            /// @return Returns a pointer to the actor of the specified name.
            virtual ActorBase* GetActor(const String& Name);
            /// @brief Gets the number of actors stored in this manager.
            /// @return Returns a whole representing the current actor count.
            virtual Whole GetNumActors();
            /// @brief Adds a pre-created actor to the manager.
            /// @details In some cases you may want to add and remove an actor from the world without destroying it and do some special
            /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
            /// function facilitates this. @n
            /// This function is also necessary for anyone inheriting from our actors to add their actors to the world.
            /// @param Actor The actor to be added to the manager.
            virtual void AddActor(ActorBase* Actor);
            /// @brief Removes an actor from this manager without destroying it.
            /// @details In some cases you may want to add and remove an actor from the world without destroying it and do some special
            /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
            /// function facilitates this. @n
            /// This function is also necessary for anyone inheriting from our actors to remove their actors from the world.
            /// @param Actor The actor to be removed from the manager.
            virtual void RemoveActor(ActorBase* Actor);
            /// @brief Removes all actors from this manager without destroying them.
            virtual void RemoveAllActors();
            /// @brief Destroys an actor.
            /// @param Actor The actor to be destroyed.
            virtual void DestroyActor(ActorBase* Actor);
            /// @brief Destroys all actors currently within this manager.
            virtual void DestroyAllActors();

            ///////////////////////////////////////////////////////////////////////////////
            // ActorRigid Management

            ///////////////////////////////////////////////////////////////////////////////
            // ActorSoft Management

            ///////////////////////////////////////////////////////////////////////////////
            // ActorCharacter Management

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the internal actor container storing all the actors.
            /// @return Returns a pointer to the Actor container being used by this manager.
            virtual ActorContainerBase* GetActorContainer();

            //Inherited from ManagerBase
            /// @brief Empty initializer that has been implemented from ManagerBase.
            virtual void Initialize();
            /// @brief Physics stepping during the main loop
            /// @details This increments the the physics world the required amount to keep it in sync with the Graphics/Timing.
            virtual void DoMainLoopItems();
            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::PhysicsManager
            virtual ManagerTypeName GetType() const;
    };//actormanager
}//phys

#endif
