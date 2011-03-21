//© Copyright 2010 BlackTopp Studios Inc.
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

namespace phys {
    /// @class EventCollision
    /// @headerfile eventcollision.h
    /// @brief This is an event class used to track collsions in the physics world.
    /// @details This class will be used for tracking collisions in the physics world and will keep track of basic data related to the collision.
    class PHYS_LIB EventCollision : public EventBase
    {
        public:
            /// @brief The location in the world where the collision occured.
            Vector3 WorldLocation;

            /// @brief The location in ActorA's local space where the collision occured.
            Vector3 LocalALocation;

            /// @brief The location in ActorB's local space where the collision occured.
            Vector3 LocalBLocation;

            /// @brief The amount of force of the collision.
            Real Impulse;

            /// @brief The first Actor involved in the collision.
            ActorBase* ActorA;

            /// @brief The second Actor invovled in the collision.
            ActorBase* ActorB;

            /// @brief Default Constructor
            EventCollision();

            /// @brief Class Constructor.
            /// @details This will construct a basic event class with the minimum data needed.
            /// @param actora The first Actor involved in the collision.
            /// @param actorb The second Actor invovled in the collision.
            /// @param localAlocation The location in ActorA's local space where the collision occured.
            /// @param localBlocation The location in ActorB's local space where the collision occured.
            /// @param worldlocation The location in the world where the collision occured.
            /// @param impulse The amount of force of the collision.
            EventCollision(ActorBase* actora, ActorBase* actorb, Vector3 localAlocation, Vector3 localBlocation, Vector3 worldlocation, Real impulse);

            /// @brief Copy Constructor.
            /// @param Other The other EventCollision to copy
            EventCollision(const EventCollision& Other);

            /// @brief Class Destructor.
            /// @details Basic Class Destructor.
            virtual ~EventCollision();

            /// @brief This returns EventType::Collision.
            /// @details  This returns the kind of message this is, specifcally EventType::Collision.  This method is inherited from phys::Event. .
            virtual EventType GetType() const;
    };
}

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
