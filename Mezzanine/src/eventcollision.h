//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef eventcollision_h
#define eventcollision_h

#include "eventbase.h"
#include "XML/xml.h"
#include "vector3.h"

namespace Mezzanine
{
    class ActorBase;
    class Collision;
    class CollisionDispatcher;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventCollision
    /// @headerfile eventcollision.h
    /// @brief This is an event class used to track collsions in the physics world.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB EventCollision : public EventBase
    {
        protected:
            friend class CollisionDispatcher;
            /// @brief The internal collision class this event is based on.
            Mezzanine::Collision* ObjectCollision;
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
            /// @param PhysicsManifold The internal manifold used for querying collision data.
            EventCollision(ActorBase* actora, ActorBase* actorb, Mezzanine::Collision* Col);
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

            /// @brief This returns EventType::Collision.
            /// @details  This returns the kind of message this is, specifcally EventType::Collision.  This method is inherited from Mezzanine::Event. .
            virtual EventType GetType() const;
    };//Event Collision
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
/// @brief Serializes the passed Mezzanine::EventCollision to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::EventCollision to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::EventCollision& Ev);

/// @brief Deserialize a Mezzanine::EventCollision
/// @param stream The istream to get the xml from to (re)make the Mezzanine::EventCollision.
/// @param Ev the Mezzanine::EventCollision to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::EventCollision that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventCollision& Ev);

/// @brief Set all values of a Mezzanine::EventCollision from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::EventCollision.
/// @param Ev the Mezzanine::EventCollision to be reset.
/// @return This returns theXML::Node that was passed in.
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::EventCollision& Ev);


#endif
