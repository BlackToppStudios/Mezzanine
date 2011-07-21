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
#ifndef _areaeffect_h
#define _areaeffect_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "quaternion.h"
#include "vector3.h"
#include "colourvalue.h"
#include "enumerations.h"

#include <list>

class btCollisionShape;
class btPairCachingGhostObject;

namespace Ogre
{
    class SceneNode;
    class Entity;
    class MaterialPtr;
}

namespace phys{
    class ActorBase;
    class World;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class AreaEffect
    /// @headerfile areaeffect.h
    /// @brief This class is used to define area's in the world that have unique effects.
    /// @details Common uses for this class are for gravity fields, and explosions.  But can be made to do more. @n
    /// Note: This is a base class intended to be derived from.  This class cannot be created itself.  To make an
    /// AreaEffect class that does what you want it to, simple inherit from this class with an AE class of your own,
    /// and define the ApplyEffect() function to do what you want your effect to do.
    ///////////////////////////////////////
    class PHYS_LIB AreaEffect
    {
        friend class PhysicsManager;
        protected:
            /// @brief The kinds of shapes and AreaEffect can take.
            enum AEShapeType
            {
                AE_Sphere,      ///< Description Required
                AE_CylinderX,   ///< Description Required
                AE_CylinderY,   ///< Description Required
                AE_CylinderZ,   ///< Description Required
                AE_Box,         ///< Description Required
                AE_Custom,      ///< Description Required
                AE_Unassigned   ///< Description Required
            };
            /// @brief The name of the Area Effect.
            String Name;
            /// @brief The type of physics shape in use by this AE field.
            AEShapeType ShapeType;
            /// @brief Stores the shape of the AE field.
            btCollisionShape* Shape;
            /// @brief The object representing the AE field itself.
            btPairCachingGhostObject* Ghost;
            /// @brief Ogre node required to adding a graohical representation to the scene graph.
            Ogre::SceneNode* GraphicsNode;
            /// @brief The Ogre object being added to the scene graph.
            Ogre::Entity* GraphicsObject;
            /// @brief World pointer simply to enable the effects of this class to be as diverse as the engine features.
            World* TheWorld;
            /// @brief Container for actors within the field area.
            std::list < ActorBase* > OverlappingActors;
            /// @brief Container of actors that have been added since last frame.
            std::vector < ActorBase* > AddedActors;
            /// @brief Container of actors that have been removed since last frame.
            std::vector < ActorBase* > RemovedActors;
            /// @brief Constructor Function.
            /// @param Location The location of the AE field.
            virtual void CreateGhostObject(const Vector3& Location);
            /// @brief Creates a basic material in code using the provided colour.
            virtual Ogre::MaterialPtr CreateColouredMaterial(const ColourValue& Colour);
            /// @brief Convenience function for the common starting steps in making a graphics object.
            virtual void PreGraphicsMeshCreate();
            /// @brief Convenience function for the common final steps in making a graphics object.
            virtual void PostGraphicsMeshCreate(const String& GroupName);
            /// @brief Helper function for adding actors to relevant lists.
            virtual void AddActorToList(ActorBase* Actor);
            /// @brief Helper function for adding actors to relevant lists.
            virtual void RemoveActorFromList(ActorBase* Actor);
        public:
            /// @brief Constructor.
            /// @details Basic initialization constructor.
            /// @param name The name of the field.
            /// @param Location The location of the AE field.
            AreaEffect(const String &name, const Vector3& Location);
            /// @brief Destructor.
            /// @details Class destructor.
            virtual ~AreaEffect();
            /// @brief Defines and applies the effect of the field.
            /// @details When inheriting this class, this function is what defines the effect the field has. @n
            /// This function will be called on by the physics manager and shouldn't be called manually.
            virtual void ApplyEffect() = 0;
            /// @brief Updates the actors listed as within the field.
            /// @details This function is automatically called once every simulation step.  This shouldn't
            /// be called manually unless your usage of this class requires more then one update per step.
            virtual void UpdateActorList();
            /// @brief Creates a Sphere shape for the field.
            /// @param Radius The radius of the sphere you want to create.
            virtual void CreateSphereShape(const Real& Radius);
            /// @brief Creates a Cylinder shape for the field that is alligned on the X axis.
            /// @details When making the vector to be passed in, remember the layout should be as such: (width*0.5, radius, radius),
            /// with the second radius perpendicular to the first.
            /// @param HalfExtents The vector representing the size of the shape.
            virtual void CreateCylinderShapeX(const Vector3& HalfExtents);
            /// @brief Creates a Cylinder shape for the field that is alligned on the Y axis.
            /// @details When making the vector to be passed in, remember the layout should be as such: (radius, height*0.5, radius),
            /// with the second radius perpendicular to the first.
            /// @param HalfExtents The vector representing the size of the shape.
            virtual void CreateCylinderShapeY(const Vector3& HalfExtents);
            /// @brief Creates a Cylinder shape for the field that is alligned on the Z axis.
            /// @details When making the vector to be passed in, remember the layout should be as such: (radius, radius, length*0.5),
            /// with the second radius perpendicular to the first.
            /// @param HalfExtents The vector representing the size of the shape.
            virtual void CreateCylinderShapeZ(const Vector3& HalfExtents);
            /// @brief Creates a Box shape for the field.
            /// @details When making the vector to be passed in, remember to pass in only half values of what you want the actual
            /// size to be.
            /// @param HalfExtents The vector representing the size of the shape.
            virtual void CreateBoxShape(const Vector3& HalfExtents);
            /// @brief Creates a shape from a .mesh model for the field.
            /// @param Filename The name of the .mesh file to be used.
            /// @param Group The resource group where the mesh can be found.
            /// @param MakeVisible If true, this function will create a visual representation from the same mesh provided.
            virtual void CreateShapeFromMesh(const String& Filename, const String& Group, bool MakeVisible);
            /// @brief Sets the scale of the shape of the field.
            /// @details The default scale is 1.0.
            /// @param Scale The vector3 representing the scale you wish to apply to each axis of the field shape.
            virtual void ScaleFieldShape(const Vector3& Scale);
            /// @brief Gets the scale of the shape of the field.
            /// @details The default scale is 1.0.
            /// @return Returns the current scale applied to the fields shape.
            virtual Vector3 GetFieldShapeScale() const;
            /// @brief Sets the origin for the area effect.
            /// @details In most cases you won't want to call this, with the exception of when you want a field to follow/track an actor.
            /// @param Location The updated location of the origin for the field.
            virtual void SetLocation(const Vector3& Location);
            /// @brief Gets the origin for the area effect.
            /// @details This function is particularly useful when making fields such as gravity wells, that have continuous effects centering on one location.
            /// @return Returns the vector3 representing the location of the area effect.
            virtual Vector3 GetLocation() const;
            /// @brief Sets the orientation of the area effect.
            /// @param Rotation The Quaternion representing the Rotation.
            virtual void SetOrientation(const Quaternion& Rotation);
            /// @brief Gets the orientation of the area effect.
            /// @return Returns a quaternion representing the rotation of the area effect.
            virtual Quaternion GetOrientation();
            /// @brief Gets the Area Effects name.
            /// @return Returns the name of the Area Effect.
            virtual ConstString& GetName() const;
            /// @brief Gets the current shape type in use by this class.
            /// @return Returns an enum value representing the type of shape being used by this class.
            virtual AreaEffect::AEShapeType GetShapeType() const;
            /// @brief Checks to see if the ghost object is currently in the world.
            /// @return Returns a bool indication whether or not this object is currently in the world.
            virtual bool IsInWorld() const;
            /// @brief Sets(or un-sets) this AE as a static object.
            /// @details Default: true.
            /// @param Static Whether or not to set this as static.
            virtual void SetStatic(bool Static);
            /// @brief Gets whether or not this AR is static.
            /// @return Returns a bool indicating if this object is static.
            virtual bool IsStatic() const;
            /// @brief Creates a sphere mesh based on the physics shape for this area effect.
            /// @details This function will create a material script on the fly for you to use with your AR field.
            /// @param Colour The colour to put into the custom created material script.  This will be the final colour of the graphics object.
            /// @param Rings The number of horizontal rings the sphere is to be comprised of.
            /// This along with the segments parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
            /// @param Segments The number of vertical rings the sphere is to be comprised of.
            /// This along with the rings parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
            virtual void CreateGraphicsSphere(const ColourValue& Colour, const Whole Rings = 16, const Whole Segments = 16);
            /// @brief Creates a sphere mesh based on the physics shape for this area effect.
            /// @param MaterialName The name of the material script to be applied to the graphics object that is created.  Must be valid.
            /// @param Rings The number of horizontal rings the sphere is to be comprised of.
            /// This along with the segments parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
            /// @param Segments The number of vertical rings the sphere is to be comprised of.
            /// This along with the rings parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
            virtual void CreateGraphicsSphere(const String& MaterialName, const Whole Rings = 16, const Whole Segments = 16);
            /// @brief Creates a cylinder mesh based on the physics shape for this area effect.
            /// @details This function will create a material script on the fly for you to use with your AR field.
            /// @param Colour The colour to put into the custom created material script.  This will be the final colour of the graphics object.
            virtual void CreateGraphicsCylinder(const ColourValue& Colour);
            /// @brief Creates a cylinder mesh based on the physics shape for this area effect.
            /// @param MaterialName The name of the material script to be applied to the graphics object that is created.  Must be valid.
            virtual void CreateGraphicsCylinder(const String& MaterialName);
            /// @brief Creates a box mesh based on the physics shape for this area effect.
            /// @details This function will create a material script on the fly for you to use with your AR field.
            /// @param Colour The colour to put into the custom created material script.  This will be the final colour of the graphics object.
            virtual void CreateGraphicsBox(const ColourValue& Colour);
            /// @brief Creates a box mesh based on the physics shape for this area effect.
            /// @param MaterialName The name of the material script to be applied to the graphics object that is created.  Must be valid.
            virtual void CreateGraphicsBox(const String& MaterialName);
            /// @brief Gets the list of actors within this field.
            /// @return Returns the list of actors contained within this field.
            virtual std::list<ActorBase*>& GetOverlappingActors();
            /// @brief Gets the list of actors that have been added to the list since the last simulation step.
            /// @return Returns the vector storing all the actors that have been added to the list since the last simulation step.
            virtual std::vector<ActorBase*>& GetAddedActors();
            /// @brief Gets the list of actors that have been removed from the list since the last simulation step.
            /// @return Returns the vector storing all the actors that have been removed from the list since the last simulation step.
            virtual std::vector<ActorBase*>& GetRemovedActors();
    };//areaeffect

    ///////////////////////////////////////////////////////////////////////////////
    /// @class TestAE
    /// @headerfile areaeffect.h
    /// @brief This is a dummy class to test if the AE field works.  Details will be output to the log.
    ///////////////////////////////////////
    class PHYS_LIB TestAE : public AreaEffect
    {
        public:
            /// @brief Constructor.
            /// @details Basic initialization constructor.
            /// @param name The name of the field.
            /// @param Location The location of the AE field.
            TestAE(const String& name, const Vector3& Location);
            /// @brief Destructor.
            /// @details Class destructor.
            virtual ~TestAE();
            /// @brief Applies the effect this field has to object inside.
            /// @details This function defines the behavior for the class.
            virtual void ApplyEffect();
    };//testAE

    ///////////////////////////////////////////////////////////////////////////////
    /// @class GravityField
    /// @headerfile areaeffect.h
    /// @brief This is a gravity field implementation of the AreaEffect class.
    /// @details This class is not a gravity well, where gravity is pulling to one point.  Instead this class uniformly
    /// pulls gravity in one direction that is different from the world gravity.
    ///////////////////////////////////////
    class PHYS_LIB GravityField : public AreaEffect
    {
        protected:
            /// @brief The stored value for this fields gravity.
            Vector3 Grav;
        public:
            /// @brief Constructor.
            /// @details Basic initialization constructor.
            /// @param name The name of the field.
            /// @param Location The location of the AE field.
            GravityField(const String &name, const Vector3& Location);
            /// @brief Destructor.
            /// @details Class destructor.
            virtual ~GravityField();
            /// @brief Applies the effect this field has to object inside.
            /// @details This function defines the behavior for the class.
            virtual void ApplyEffect();
            /// @brief Sets the gravity force for this field.
            /// @details Sets the strength and direction of gravity this field will have.
            /// @param Gravity The vector3 representing the force and direction of gravity this field will have.
            virtual void SetFieldGravity(const Vector3& Gravity);
            /// @brief Gets the gravity of this field.
            /// @details Gets the strength and direction of gravity this field has.
            /// @return Returns a vector3 representing the force and direction of gravity this field has.
            virtual Vector3 GetFieldGravity() const;
    };//GravityField

    ///////////////////////////////////////////////////////////////////////////////
    /// @class GravityWell
    /// @headerfile areaeffect.h
    /// @brief This is a gravity well implementation of the AreaEffect class.
    /// @details This class is not a gravity field, where gravity only pulls in one direction.  Instead this class will always
    /// pull objects inside it towards the field center.  @n
    /// This class works best with sphere's, but can be made to work with any shape.
    ///////////////////////////////////////
    class PHYS_LIB GravityWell : public AreaEffect
    {
        protected:
            /// @brief The amount of force exerted on other objects
            Real Strength;
            /// @brief Should world gravity ne ignored
            bool AllowWorldGrav;
            /// @brief How does gravity
            phys::AttenuationStyle AttenStyle;
            /// @brief how much does the Gravity attenuate.
            Real AttenAmount;
        public:
            /// @brief Constructor.
            /// @details Basic initialization constructor.
            /// @param name The name of the field.
            /// @param Location The location of the AE field.
            GravityWell(const String &name, const Vector3& Location);
            /// @brief Destructor.
            /// @details Class destructor.
            virtual ~GravityWell();
            /// @brief Applies the effect this field has to object inside.
            /// @details This function defines the behavior for the class.
            virtual void ApplyEffect();
            /// @brief Sets the strenth of the field.
            /// @details The direction of the field is based on the current position of the object in the field.
            /// Once that direction is calculated it will be multiplied by this value to determine the force the field will apply to the object.
            /// @param FieldStrength The strength the field will have when exerting force onto other objects.
            virtual void SetFieldStrength(const Real& FieldStrength);
            /// @brief Gets the strength of the field.
            /// @return Returns a Real representing the value that is being multiplied by the direction to determine force appied to objects.
            virtual Real GetFieldStrength() const;
            /// @brief Sets whether or not world gravity should be removed for objects in this field.
            /// @remarks Changing this value while the field is in the world and active is not recommended.
            /// @param WorldGravity If true, then forces exerted by this field will be added to the world gravity, otherwise
            /// world gravity for objects inside will be set to zero.
            virtual void SetAllowWorldGravity(bool WorldGravity);
            /// @brief Gets whether or not world gravity is is removed for objects inside this field.
            /// @return Returns a bool indicating whether objects inside are affected by world gravity.
            virtual bool GetAllowWorldGravity() const;
            /// @brief Sets the attenuation for this field.
            /// @param Amount The amount of force that is dropped off per 1 unit of distance objects are from the AE center.
            /// @param Style The style of attenuation to apply, see the AttenuationStyle enum for more details.
            virtual void SetAttenuation(const Real& Amount, const phys::AttenuationStyle& Style);
            /// @brief Gets the Style of attenuation applied.
            /// @details See the AttenuationStyle enum for more details.
            /// @return Returns the style of attenuation currently being used by this field.
            virtual phys::AttenuationStyle GetAttenuationStyle() const;
            /// @brief Gets the amount force is attenuated over distance.
            /// @details See SetAttenuation() for more details.
            /// @return Returns a Real value
            virtual Real GetAttenuationAmount() const;
    };//GravityWell

    ///////////////////////////////////////////////////////////////////////////////
    /// @class FieldOfForce
    /// @headerfile areaeffect.h
    /// @brief This is field that applies force in a direction, and doesn't tamper with gravity.
    /// @details This class is similiar to a gravity well in that it can attenuate, but different in
    /// that the direction is constant, the source of force(for calculating attenuation) can be outside the
    /// field itself, and the direction is constant.  @n
    /// Placing the source of attenuation inside the field will cause the object to accelerate as it gets close
    /// to the source, and then will be applied less force(but in the same direction) as it moves from the source.
    /// This behavior makes this class good for creating a booster-like AE.
    ///////////////////////////////////////
    class PHYS_LIB FieldOfForce : public AreaEffect
    {
        protected:
            /// @brief The amount of force exerted on other objects
            Real Strength;
            /// @brief The direction the force is applied.
            Vector3 Direction;
            /// @brief How gravity weakens over distance, if at all.
            phys::AttenuationStyle AttenStyle;
            /// @brief How much the Gravity weakens over distance.
            Real AttenAmount;
            /// @brief The user defined source if enabled.
            Vector3 AttenSource;
        public:
            /// @brief Class Constructor.
            /// @param name The name of the field.
            /// @param Location The location of the AE field.
            FieldOfForce(const String &name, const Vector3& Location);
            /// @brief Class Destructor
            virtual ~FieldOfForce();
            /// @brief Applies the effect this field has to object inside.
            /// @details This function defines the behavior for the class.
            virtual void ApplyEffect();
            /// @brief Sets the strenth of the field.
            /// @param FieldStrength The strength the field will have when exerting force onto other objects.
            virtual void SetFieldStrength(const Real& FieldStrength);
            /// @brief Gets the strength of the field.
            /// @return Returns a Real representing the value that is being multiplied by the direction to determine force appied to objects.
            virtual Real GetFieldStrength() const;
            /// @brief Sets the direction force is to be applied within this field.
            /// @param ForceDirection A vector3 representing the direction force is to be applied.
            virtual void SetDirectionOfForce(const Vector3& ForceDirection);
            /// @brief Gets the currenly set direction force is to be applied.
            /// @return Returns a vector3 representing the direction of force in this field.
            virtual Vector3 GetDirectionOfForce();
            /// @brief Sets the attenuation for this field.
            /// @param Amount The amount of force that is dropped off per 1 unit of distance objects are from the AE source.
            /// @param Style The style of attenuation to apply, see the AttenuationStyle enum for more details.
            /// @param Source A vector3 representing the source of force to use when calculating attenuation.
            virtual void SetAttenuation(const Real& Amount, const phys::AttenuationStyle& Style, const Vector3& Source);
            /// @brief Gets the Style of attenuation applied.
            /// @details See the AttenuationStyle enum for more details.
            /// @return Returns the style of attenuation currently being used by this field.
            virtual phys::AttenuationStyle GetAttenuationStyle() const;
            /// @brief Gets the amount force is attenuated over distance.
            /// @details See SetAttenuation() for more details.
            /// @return Returns a Real value
            virtual Real GetAttenuationAmount() const;
            /// @brief Gets the source of the force for calculating attenuation.
            /// @return Returns a Vector3 representing the source of the attenuating force.
            virtual Vector3 GetAttenuationSource() const;
    };//feildofforce
}//phys

#endif
