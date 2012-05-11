//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _actorsoft_h
#define _actorsoft_h

#include "actorbase.h"
#include "actorphysicssettings.h"
#include "crossplatformexport.h"

///////////////////////////////////
// Forward Declares
//
class btSoftBody;
class btSoftBodyWorldInfo;
//typedef float btScalar;

namespace Ogre
{
    class ManualObject;
    class ManualObjectSection;
    class Vector3;
    class Vector2;
}

///////////////////////////////////
// Actual code
namespace Mezzanine
{
    namespace Internal{
        struct MeshInfo;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorSoft
    /// @headerfile actorsoft.h
    /// @brief This is the actor class for Soft Objects.
    /// @details This class should be used to make any soft object that, like ActorRigid,
    /// can be moved or manipulated as a result of force.  Examples of soft objects are: Paper,
    /// Rope, and Cloth.  Semi Rigid bodies that are still somewhat deformable, like Jello,
    /// should be made as a soft object.
    /// @warning Currently this is not fully implemented and will likely cause errors if used.
    ///////////////////////////////////////
    class MEZZ_LIB ActorSoft: public ActorBase {
        protected:
            ///@brief Used to simulate the functionality of a btSoftBody for use with the physics subsystem.
            btSoftBody* PhysicsSoftBody;
            /// @brief This class encapsulates physics specific configuration for this actor.
            ActorSoftPhysicsSettings* PhysicsSettings;
            /// @brief Creates a soft object for the actor.
            /// @details Creates a soft object to be placed in the physics world later. @n
            /// This is automatically called by the Constructor and shouldn't be called manually.
            /// @param mass The desired total mass of the soft object.
            void CreateSoftObject(Real mass);
            /// @brief Creates and configures a manual object for rendering.
            /// @details This function will create and configure an Ogre manual object based on the mesh provided, that will be updateable each frame.
            /// @param TheMesh The struct with all the appropriate mesh information to base the manual object on.
            void CreateManualMesh(Internal::MeshInfo &TheMesh);
            /// @brief Makes the actor visable.
            /// @details Adds the actor to all the nessessary graphics elements to make it visable on screen. @n
            /// This is automaticly called by the Worlds AddActor function and shouldn't ever need to be called manually.
            virtual void AttachToGraphics();
            /// @brief Makes the actor invisable.
            /// @details This is the inverse of the AttachToGraphics function.  This will effectively remove the object from the graphics world and make it no longer visable. @n
            /// This is automaticly called by the Worlds RemoveActor function and shouldn't ever need to be called manually.
            virtual void DetachFromGraphics();
            /// @brief Sets the location of the physics body.
            /// @details This will take a Vector3 and set the location of the actor within the physics world. @n
            /// This function is called on by the SetLocation function, and shouldn't be called manually.
            /// @param Location The Vector3 representing the location.
            virtual void SetBulletLocation(Vector3 Location);
            /// @brief Retrieves the location of the physics body.
            /// @details This function will retrieve the location of the object within the physics world.
            /// @return A Vector3 with the location in the physics system.
            virtual Vector3 GetBulletLocation() const;
            /// @brief Sets the orientation of the physics body.
            /// @details This will take a Quaternion and set the orientation of the actor within the physics world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the actor.
            virtual void SetBulletOrientation(Quaternion Rotation);

        public:
            /// @brief Constructor
            /// @param mass The mass of the soft body.
            /// @param name Name of the object
            /// @param file Mesh the soft body is stored in
            /// @param group The Resource group the mesh can be found in.
            ActorSoft(Real mass, String name, String file, String group);

            /// @brief Destructor.
            /// @details The class destructor.
            virtual ~ActorSoft();

            /// @brief Retrieves the name of the object.
            /// @return A String with the name of this Actor
            ConstString& GetName() const;

            /// @brief Gets the physics settings class associated with this actor.
            /// @return Returns a pointer to the physics settings class in use by this actor.
            virtual ActorSoftPhysicsSettings* GetPhysicsSettings();

            /// @brief Sets the scale of the actor.
            /// @details This function will alter the scaling/size of the actor with the given vector3.
            /// @param scale The vector3 by which to apply the scale.  Will scale each axis' accordingly.
            void SetActorScaling(Vector3 scale);

            /// @brief Sets the starting location of the actor.
            /// @details Calling this function after adding it to the World will have no effect. @n
            /// This function will set where the actor will be located in the World when it is first placed inside the world.
            /// @param Location The Vector3 representing the location.
            virtual void SetInitLocation(Vector3 Location);

            /// @brief Sets the starting orientation of the actor.
            /// @details Calling this function after adding it to the World will have no effect. @n
            /// This function will set where the actor is facing in the World when it is first placed inside the world.
            /// @param Orientation The Quaternion representing the Orientation.
            virtual void SetInitOrientation(Quaternion Orientation);

            /// @brief Manually sets the location of the actor.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param x Location on the X vector.
            /// @param y Location on the Y vector.
            /// @param z Location on the Z vector.
            virtual void SetLocation(Real x, Real y, Real z);

            /// @brief Manually sets the location of the actor.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param Place The Vector3 representing the location.
            virtual void SetLocation(Vector3 Place);

            /// @brief Retrieves the location of the object.
            /// @details This function will retrieve the location of the object within the world.
            /// @return A Vector3 with the actual locaiton of this Actor.
            virtual Vector3 GetLocation() const;

            /// @brief Sets the orientation of the actor.
            /// @details Sets the orientation of the actor via Quaternion parameters.
            /// @param x Where the X vector is rotated about.
            /// @param y Where the Y vector is rotated about.
            /// @param z Where the Z vector is rotated about.
            /// @param w How much to about the x, y, z.
            virtual void SetOrientation(Real x, Real y, Real z, Real w);

            /// @brief Sets the orientation of the actor.
            /// @details Sets the orientation of the actor via a Quaternion.
            /// @param Rotation The Quaternion representing the Rotation.
            virtual void SetOrientation(Quaternion Rotation);

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from WorldObject
            ///////////////////////////////////////
            /// @copydoc WorldObject::GetType()
            virtual WorldAndSceneObjectType GetType() const;
            /// @copydoc WorldObject::AddToWorld()
            virtual void AddToWorld();
            /// @copydoc WorldObject::RemoveFromWorld()
            virtual void RemoveFromWorld();
            /// @copydoc WorldObject::_Update()
            virtual void _Update();

            /// @internal
            /// @brief Get the Physics data raw from the physic subsystem
            /// @return Currently this returns a pointer to a btSoftBody.
            virtual btSoftBody* GetBulletObject();
    };
}
#endif
