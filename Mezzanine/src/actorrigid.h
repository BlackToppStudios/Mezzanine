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
#ifndef _actorrigid_h
#define _actorrigid_h

#include "actorbase.h"
#include "actorphysicssettings.h"

///////////////////////////////////
// Actual code
namespace Mezzanine
{
    namespace Physics
    {
        class Constraint;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorRigid
    /// @headerfile actorrigid.h
    /// @brief This is the actor class for Rigid Objects.
    /// @details This class should be used to make any rigid object that can be moved as a
    /// result of force.  Most objects will fall into this catagory.  A few examples of a
    /// Rigid Object: Boxes, Car Frames, Chairs, etc.  For Semi Rigid bodies that are
    /// deformable, like jello, it is better to use ActorSoft.
    ///////////////////////////////////////
    class MEZZ_LIB ActorRigid : public ActorBase
    {
        protected:
            friend class Physics::Constraint;
            /// @brief Used to simulate the behavior of a btRigidBody
            btRigidBody* PhysicsRigidBody;

            /// @brief Creates a rigid object for the actor.
            /// @details Creates a rigid object to be placed in the physics world later. @n
            /// This is automaticly called by the Constructor and shouldn't be called manually.
            /// @param pmass "Real Mass" The mass of the object.
            virtual void CreateRigidObject(const Real& pmass);

        public:
            /// @brief Descriptive constructor.
            /// @details This constructor contains the basic information needed to make a Rigid Object. @n
            /// This class inherits from ActorBase.
            /// @param mass The mass the object will have in the World.
            /// @param name The name of the actor.
            /// @param file The 3d mesh file that contains the 3d model the actor will use.
            /// @param group The resource group where the 3d mesh and other related files can be found.
            ActorRigid(const Real& mass, const String& name, const String& file, const String& group);
            /// @brief Destructor.
            /// @details The class destructor.
            virtual ~ActorRigid();

            /// @brief Gets the physics settings class associated with this actor.
            /// @return Returns a pointer to the physics settings class in use by this actor.
            virtual ActorRigidPhysicsSettings* GetPhysicsSettings() const;

            /// @brief Restricts movement on the axis or axies of your choice.
            /// @details This function can lock or limit any and all axes you define.
            /// 0.0 means no linear movement on that axis.  1.0 means normal movement.
            /// @param Factor Vector3 containing the Factors for the 3 linear axes.
            virtual void SetLinearMovementFactor(const Vector3& Factor);
            /// @brief Gets the current linear factors being applied to this actor.
            /// @return Returns a Vector3 representing the factors on the 3 linear axes.
            virtual Vector3 GetLinearMovementFactor() const;

            /// @brief Restricts movement on the axis or axies of your choice.
            /// @details This function can lock or limit any and all axes you define.
            /// 0.0 means no angular movement on that axis.  1.0 means normal movement.
            /// @param Factor Vector3 containing the Factors for the 3 angular axes.
            virtual void SetAngularMovementFactor(const Vector3& Factor);
            /// @brief Gets the current angular factors being applied to this actor.
            /// @return Returns a Vector3 representing the factors on the 3 angular axes.
            virtual Vector3 GetAngularMovementFactor() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from WorldObject
            ///////////////////////////////////////
            /// @copydoc WorldObject::GetType()
            virtual WorldAndSceneObjectType GetType() const;
            /// @copydoc WorldObject::AddObjectToWorld()
            virtual void AddToWorld();
            /// @copydoc WorldObject::RemoveObjectFromWorld()
            virtual void RemoveFromWorld();
            /// @copydoc WorldObject::_Update()
            virtual void _Update();
            /// @copydoc WorldObject::_NotifyCollisionState(Collision* Col)
            virtual void _NotifyCollisionState(Collision* Col, const Collision::CollisionState& State);

            /// @internal
            /// @brief Get the Physics data raw from the physic subsystem
            /// @return Currently this returns a pointer to a btRigidBody.
            virtual btRigidBody* GetBulletObject();

#ifdef MEZZXML
            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

        protected:
            /// @internal
            /// @brief a Helper function that assembles strings and throws an exception
            /// @param Fail The item that failed.
            virtual void ThrowSerialError(const String& Fail) const;

        public:
            /// @copy ActorBase::GraphicsSettingsSerializableName() const;
            virtual String GraphicsSettingsSerializableName() const;
            /// @copy ActorBase::PhysicsSettingsSerializableName() const;
            virtual String PhysicsSettingsSerializableName() const;

            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ActorRigid"
            static String SerializableName();
#endif

    };
}

#ifdef MEZZXML
/// @brief Serialize an actor and send it to a stream
/// @param ActorToSerialize The actor serialize
/// @param stream the std::ostream to send the actors xml to.
/// @return The ostream after the new data has been inserted.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::ActorRigid& ActorToSerialize);

/// @brief Get an actor from an XML stream.
/// @param stream The stream to get it out of.
/// @param x The it you will get out of the stream.
/// @return This returns the input stream to allow operator chaining.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ActorRigid& x);

/// @brief Converts an XML Node into a functional in memory construct.
/// @param OneNode The xml node that contains the deserialize class instance.
/// @param x The class instance to overwrite witht the proto serialized version in the node.
void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::ActorRigid& x);
#endif  // \mezzxml

#endif
