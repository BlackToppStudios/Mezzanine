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
#ifndef eventcollision_h
#define eventcollision_h

#include "actorbase.h"
#include "crossplatformexport.h"
#include "datatypes.h"
#include "eventbase.h"
#include "xml.h"
#include "vector3.h"

class btPersistentManifold;

namespace phys {
    class CollisionDispatcher;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventCollision
    /// @headerfile eventcollision.h
    /// @brief This is an event class used to track collsions in the physics world.
    /// @details This class will be used for tracking collisions in the physics world and will keep track of basic data related to the collision.
    /// This class stores the information in the form of contact points.  Often when a collision occurs there will be more then one place where
    /// the collision occured, this is a contact point.  Internally collisions only store up to a maximum of 4 contact points.  When querying for
    /// collision information, you have to provide the desired contact point index, and it must be valid.  If the requested index isn't valid an
    /// exception will be thrown.  So always make sure it is with GetNumContactPoints().
    ///////////////////////////////////////
    class PHYS_LIB EventCollision : public EventBase
    {
        public:
            /// @enum CollisionType
            /// @brief Enum specifying what kind of collision this class is storing.
            enum CollisionType
            {
                Col_Actor_Actor,        ///< Specifies a collision between two Actors.
                Col_Actor_Terrain,      ///< Specifies a collision between an Actor and some Terrain.
                Col_Actor_AreaEffect,   ///< Specifies a collision between an Actor and an AreaRffect.
                Col_AreaEffect_Terrain  ///< Specifies a collision between an AreaEffect and some Terrain.
            };
        protected:
            friend class CollisionDispatcher;
            /// @brief The internal collision class this event is based on.
            btPersistentManifold* Manifold;
            /// @brief The first Actor involved in the collision.
            ActorBase* ActorA;
            /// @brief The second Actor invovled in the collision.
            ActorBase* ActorB;
        public:
            /// @brief Default Constructor
            EventCollision();
            /// @brief Class Constructor.
            /// @details This will construct a basic event class with the minimum data needed.
            /// @param actora The first Actor involved in the collision.
            /// @param actorb The second Actor invovled in the collision.
            EventCollision(ActorBase* actora, ActorBase* actorb);
            /// @brief Copy Constructor.
            /// @param Other The other EventCollision to copy
            EventCollision(const EventCollision& Other);
            /// @brief Class Destructor.
            /// @details Basic Class Destructor.
            virtual ~EventCollision();

            /// @brief Sets the first actor this collision applies to.
            /// @warning Collision events can't/shouldn't have the bodies they apply to changed.  This function
            /// exists mostly just for the blank constructor when you need to set them afterward.  If you attempt
            /// to set this when the pointer is already set, it will log the event but otherwise silently fail.
            /// @param A The first actor in this event.
            virtual void SetActorA(ActorBase* A);
            /// @brief Gets the first actor this collision applies to.
            /// @return Returns a pointer to the first actor in this event.
            virtual ActorBase* GetActorA() const;
            /// @brief Sets the second actor this collision applies to.
            /// @warning Collision events can't/shouldn't have the bodies they apply to changed.  This function
            /// exists mostly just for the blank constructor when you need to set them afterward.  If you attempt
            /// to set this when the pointer is already set, it will log the event but otherwise silently fail.
            /// @param B The second actor in this event.
            virtual void SetActorB(ActorBase* B);
            /// @brief Gets the second actor this collision applies to.
            /// @return Returns a pointer to the second actor in this event.
            virtual ActorBase* GetActorB() const;

            /// @brief Gets the number of contact points this collision is storing.
            /// @return Returns the number of contact points that currently exist for this collision.
            Whole GetNumContactPoints();
            /// @brief Gets the location in the world where the collision occured.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a vector3 containing the approximate world location of the collision.
            Vector3 GetWorldLocation(const Whole& Point);
            /// @brief Gets the location in ObjectA's local space where the collision occured.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a vector3 with the point of the collision in ObjectA's local space.
            Vector3 GetLocalALocation(const Whole& Point);
            /// @brief Gets the location in ObjectB's local space where the collision occured.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a vector3 with the point of the collision in ObjectB's local space.
            Vector3 GetLocalBLocation(const Whole& Point);
            /// @brief GEts the collision normal for a contact point.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a vector3 representing the collision normal for a contact point.
            Vector3 GetNormal(const Whole& Point);
            /// @brief Gets the amount of force of the collision.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a real representing the amount of force applied from the collision.
            Real GetAppliedImpulse(const Whole& Point);
            /// @brief Gets the number of simulation steps the contact point has existed.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a Whole representing the amount of simulation steps a point has existed.
            Whole GetAge(const Whole& Point);

            /// @brief This returns EventType::Collision.
            /// @details  This returns the kind of message this is, specifcally EventType::Collision.  This method is inherited from phys::Event. .
            virtual EventType GetType() const;
    };//Event Collision
}//phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML

/// @brief Serializes the passed phys::EventCollision to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::EventCollision to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::EventCollision& Ev);

/// @brief Deserialize a phys::EventCollision
/// @param stream The istream to get the xml from to (re)make the phys::EventCollision.
/// @param Ev the phys::EventCollision to be deserialized.
/// @return this returns the ostream, advanced past the phys::EventCollision that was recreated onto Ev.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::EventCollision& Ev);

/// @brief Set all values of a phys::EventCollision from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the phys::EventCollision.
/// @param Ev the phys::EventCollision to be reset.
/// @return This returns thexml::Node that was passed in.
void PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::EventCollision& Ev);

#endif // \PHYSXML


#endif
