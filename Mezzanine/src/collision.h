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
#ifndef _collision_h
#define _collision_h

#include "vector3.h"
#include "xml.h"

class btPersistentManifold;
class btBroadphasePair;
class btCollisionAlgorithm;
class btManifoldPoint;

namespace Mezzanine
{
    class WorldObject;
    class ActorBase;
    class CollisionDispatcher;
    struct CollisionInternalData;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Collision
    /// @headerfile collision.h
    /// @brief This is an event class used to track collsions in the physics world.
    /// @details This class will be used for tracking collisions in the physics world and will keep track of basic data related to the collision.
    /// This class stores the information in the form of contact points.  Often when a collision occurs there will be more then one place where
    /// the collision occured, this is a contact point.  Internally collisions only store up to a maximum of 4 contact points.  When querying for
    /// collision information, you have to provide the desired contact point index, and it must be valid.  If the requested index isn't valid an
    /// exception will be thrown.  So always make sure to verify with GetNumContactPoints().
    ///////////////////////////////////////
    class MEZZ_LIB Collision
    {
        public:
            /// @enum CollisionState
            /// @brief Enum specifying the state change occuring in the collision.
            enum CollisionState
            {
                Col_Begin,
                Col_Contacts_Updated,
                Col_End
            };
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
            friend class PhysicsManager;
            /// @brief The internal collision class this event is based on.
            btCollisionAlgorithm* InternalAlgo;
            /// @brief Array of manifolds that apply to this collision.
            CollisionInternalData* InternalData;
            /// @brief The first Object involved in the collision.
            WorldObject* ObjectA;
            /// @brief The second Object invovled in the collision.
            WorldObject* ObjectB;
            /// @brief This stores the distance of each contact point in this collision, for using to track updates.
            std::vector<Real> PenetrationDistances;
            /// @internal
            /// @brief Class Constructor.
            /// @details This will construct a basic event class with the minimum data needed.
            /// @param A The first Object involved in the collision.
            /// @param B The second Object invovled in the collision.
            /// @param PhysicsAlgo The internal algorithm used for generating collision data.
            Collision(WorldObject* A, WorldObject* B, btCollisionAlgorithm* PhysicsAlgo);
            /// @internal
            /// @brief Internal function responsible for fetching the appropriate contact point.
            btManifoldPoint& GetManifoldPoint(const Whole& Index);
            /// @internal
            /// @brief Updates the PenetrationDistances vector on this object.
            void UpdatePenetrationDistances();
        public:
            /// @brief Default Constructor
            Collision();
            /// @brief Copy Constructor.
            /// @param Other The other EventCollision to copy
            Collision(const Collision& Other);
            /// @brief Class Destructor.
            /// @details Basic Class Destructor.
            virtual ~Collision();

            /// @brief Sets the first Object this collision applies to.
            /// @warning Collision events can't/shouldn't have the bodies they apply to changed.  This function
            /// exists mostly just for the blank constructor when you need to set them afterward.  If you attempt
            /// to set this when the pointer is already set, it will log the event but otherwise silently fail.
            /// @param A The first Object in this event.
            virtual void SetObjectA(WorldObject* A);
            /// @brief Gets the first Object this collision applies to.
            /// @return Returns a pointer to the first Object in this event.
            virtual WorldObject* GetObjectA() const;
            /// @brief Sets the second Object this collision applies to.
            /// @warning Collision events can't/shouldn't have the bodies they apply to changed.  This function
            /// exists mostly just for the blank constructor when you need to set them afterward.  If you attempt
            /// to set this when the pointer is already set, it will log the event but otherwise silently fail.
            /// @param B The second Object in this event.
            virtual void SetObjectB(WorldObject* B);
            /// @brief Gets the second Object this collision applies to.
            /// @return Returns a pointer to the second Object in this event.
            virtual WorldObject* GetObjectB() const;

            /// @brief Gets the number of contact points this collision is storing.
            /// @return Returns the number of contact points that currently exist for this collision.
            virtual Whole GetNumContactPoints();
            /// @brief Gets the location in the world where the collision occured.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a vector3 containing the approximate world location of the collision.
            virtual Vector3 GetWorldLocation(const Whole& Point);
            /// @brief Gets the location in ObjectA's local space where the collision occured.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a vector3 with the point of the collision in ObjectA's local space.
            virtual Vector3 GetLocalALocation(const Whole& Point);
            /// @brief Gets the location in ObjectB's local space where the collision occured.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a vector3 with the point of the collision in ObjectB's local space.
            virtual Vector3 GetLocalBLocation(const Whole& Point);
            /// @brief GEts the collision normal for a contact point.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a vector3 representing the collision normal for a contact point.
            virtual Vector3 GetNormal(const Whole& Point);
            /// @brief Gets the amount of force of the collision.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a real representing the amount of force applied from the collision.
            virtual Real GetAppliedImpulse(const Whole& Point);
            /// @brief Gets the penetration depth of the collision.
            /// @remarks You should double check the return of this to verify that it is <0, sometimes a collision or contact point can be
            /// reported while there is no actual overlap depending on your physics setup.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a real representing the depth of penetration between the two objects in this collision.
            virtual Real GetDistance(const Whole& Point);
            /// @brief Gets the number of simulation steps the contact point has existed.
            /// @param Point The index of the contact point for this collision.
            /// @return Returns a Whole representing the amount of simulation steps a point has existed.
            virtual Whole GetAge(const Whole& Point);

            /// @brief Convenience function to see if the provided pair match the pair in this class.
            /// @param A The first object to be compared.  Will be checked against both objects in this collision.
            /// @param B The second object to be compared.  Will be checked against both objects in this collision.
            /// @return Returns a bool, true if the pairs match, false otherwise.
            virtual bool PairsMatch(WorldObject* A, WorldObject* B) const;
            /// @brief Updates this collisions contact point data if it needs updating.
            virtual void Update();
    };//Collision
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML

/// @brief Serializes the passed Mezzanine::Collision to XML
/// @param stream The ostream to send the xml to.
/// @param Col the Mezzanine::Collision to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Collision& Col);

/// @brief Deserialize a Mezzanine::Collision
/// @param stream The istream to get the xml from to (re)make the Mezzanine::Collision.
/// @param Col the Mezzanine::Collision to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::Collision that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Collision& Col);

/// @brief Set all values of a Mezzanine::Collision from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::Collision.
/// @param Col the Mezzanine::Collision to be reset.
/// @return This returns the XML::Node that was passed in.
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Collision& Col);

#endif // \MEZZXML


#endif
