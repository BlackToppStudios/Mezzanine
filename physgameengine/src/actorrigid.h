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
#ifndef _physactorrigid_h
#define _physactorrigid_h

#include "actorbase.h"
#include "actorphysicssettings.h"
#include "crossplatformexport.h"

///////////////////////////////////
// Actual code
namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorRigid
    /// @headerfile actorbase.h
    /// @brief This is the actor class for Rigid Objects.
    /// @details This class should be used to make any rigid object that can be moved as a
    /// result of force.  Most objects will fall into this catagory.  A few examples of a
    /// Rigid Object: Boxes, Car Frames, Chairs, etc.  For Semi Rigid bodies that are
    /// deformable, like jello, it is better to use ActorSoft.
    ///////////////////////////////////////
    class PHYS_LIB ActorRigid : public ActorBase {
        protected:
            friend class TypedConstraint;
            /// @brief Used to simulate the behavior of a btRigidBody
            btRigidBody* physrigidbody;

            /// @brief This class encapsulates physics specific configuration for this actor.
            ActorRigidPhysicsSettings* PhysicsSettings;

            /// @brief Creates a rigid object for the actor.
            /// @details Creates a rigid object to be placed in the physics world later. @n
            /// This is automaticly called by the Constructor and shouldn't be called manually.
            /// @param pmass "Real Mass" The mass of the object.
            virtual void CreateRigidObject(const Real& pmass);

            /// @brief The File that will store the main 3d model this works with.
            String ModelFile;

            /// @brief The resource group associated with the 3d model this Actor uses.
            String ModelGroup;

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

            /// @brief Retrieves the name of the object.
            /// @return This function will return the name of the object in a String
            virtual String GetName() const;

            /// @brief Creates a collision shape from mesh file.
            /// @details This function will read the location of every verticy in the mesh file and use that to
            /// construct a triangle mesh shape and attach it to this objects collision shape.  This shoiuld
            /// be used with only with Dynamic objects.
            /// @param Accuracy A value from 1 to 4. The higher the more accurate, but the more resource intensive
            /// @param UseAllSubmeshes Should Child meshes, if any, be used when approximating physics shapes
            virtual void CreateShapeFromMeshDynamic(short unsigned int Accuracy = 1, bool UseAllSubmeshes = false);

            /// @brief Creates a sphere collision shape.
            /// @details Will dynamically create a sphere collision shape from the loaded mesh file.
            virtual void CreateSphereShapeFromMesh();

            /// @brief Creates a collision shape from mesh file.
            /// @details This function will read the location of every verticy in the mesh file and use that to
            /// construct a triangle mesh shape and attach it to this objects collision shape.  This shoiuld
            /// be used with only with Static objects.
            /// @param UseAllSubmeshes If true, this will use the geometry of all submeshes of the model to make the shape.  Otherwise it'll only use the first submesh.
            virtual void CreateShapeFromMeshStatic(bool UseAllSubmeshes = false);

            /// @brief Gets the physics settings class associated with this actor.
            /// @return Returns a pointer to the physics settings class in use by this actor.
            virtual ActorRigidPhysicsSettings* GetPhysicsSettings();

            /// @brief Restricts movement on the axis or axies of your choice.
            /// @details This function can lock or limit any and all axes you define.
            /// 0.0 means no linear movement on that axis.  1.0 means normal movement.
            /// @param Factor Vector3 containing the Factors for the 3 linear axes.
            virtual void SetLinearMovementFactor(const Vector3& Factor);

            /// @brief Gets the current linear factors being applied to this actor.
            /// @return Returns a Vector3 representing the factors on the 3 linear axes.
            virtual Vector3 GetLinearMovementFactor();

            /// @brief Restricts movement on the axis or axies of your choice.
            /// @details This function can lock or limit any and all axes you define.
            /// 0.0 means no angular movement on that axis.  1.0 means normal movement.
            /// @param Factor Vector3 containing the Factors for the 3 angular axes.
            virtual void SetAngularMovementFactor(const Vector3& Factor);

            /// @brief Gets the current angular factors being applied to this actor.
            /// @return Returns a Vector3 representing the factors on the 3 angular axes.
            virtual Vector3 GetAngularMovementFactor();

            // Inherited from ActorBase
            virtual void AddObjectToWorld (World *TargetWorld);
            virtual void RemoveObjectFromWorld(World* TargetWorld);

            /// @internal
            /// @brief Get the Physics data raw from the physic subsystem
            /// @return Currently this returns a pointer to a btRigidBody.
            virtual btRigidBody* GetBulletObject();

///////////////////////////////////////////////////////////////////////////////
// Serialization
///////////////////////////////////////
#ifdef PHYSXML
        private:
            /// @internal
            /// @brief a Helper function that assembles strings and throws an exception
            /// @param Fail The item that failed.
            virtual void ThrowSerialError(const String& Fail) const;

        public:
            /// @brief Get all the this in an xml::Node.
            /// @param CurrentRoot The point in the XML hierarchy that all this vector3 should be appended to.
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;
#endif

    };
}

#ifdef PHYSXML
/// @brief Serialize an actor and send it to a stream
/// @param ActorToSerialize The actor serialize
/// @param stream the std::ostream to send the actors xml to.
/// @return The ostream after the new data has been inserted.
std::ostream& operator << (std::ostream& stream, const phys::ActorRigid& ActorToSerialize);
#endif  // \physxml

#endif
